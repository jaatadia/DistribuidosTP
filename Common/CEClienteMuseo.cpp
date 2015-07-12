/* 
 * File:   CE.cpp
 * Author: knoppix
 *
 * Created on June 29, 2015, 12:30 AM
 */

#include <cstdlib>
#include <sys/msg.h>
#include <unistd.h>
#include <signal.h>

#include "enviar.cpp"
#include "recibir.cpp"
#include "Logger.h"
#include "Museo.h"

#define ID "CECliente"
using namespace std;

static int mySocket;

void myHandler(int sigNum){
    Logger::logg("Terminando conexion");
    close(mySocket);
    Logger::closeLogger();
    exit(1);
}
//argv[1] id
//argv[2] fd socket
//argv[3] fd cola
//argv[4] kill

int main(int argc, char** argv) {
    
    if(argc<5){
        printf("Mal uso 1:id 2: keyCola 3:fdSocket 4:pidKill");
        return -1;
    }
    
    Logger::startLog(LOGGER_DEFAULT_PATH,ID);
    
    struct sigaction oldHandler;
    struct sigaction newHandler;
    newHandler.sa_handler=myHandler;
    newHandler.sa_flags=0;
    sigfillset(&(newHandler.sa_mask));
    
    if(sigaction(SIGUSR1,&newHandler,&oldHandler) == -1){
        Logger::loggError("Error al encontrar asignar el signal handler de CECliente");
        exit(1);   
    }
    
    
    
    long id = atoi(argv[1]);
    int cola = atoi(argv[2]);
    mySocket = atoi(argv[3]);
    //int pidkill = atoi(argv[4]);
    
    if( (cola = msgget(cola,0660)) == -1){
        Logger::loggError("Error al encontrar la cola del cliente");
        exit(1);   
    }
    
    Logger::logg(string("Enviando mi ID: ")+argv[1]);
    Museo msg;
    msg.myType=id;
    if(enviar(mySocket,&msg,sizeof(Museo))<0){
        Logger::loggError("Error al recibir el mensaje ");
        exit(1);
    };
    
    while(true){
        
        Logger::logg("Esperando mensaje sobre el socket");
        if(recibir(mySocket,&msg,sizeof(Museo))<=0){
            Logger::loggError("Error al recibir el mensaje ");
            exit(1);
        };
        
        char origen[14];
        char destino[14];
        sprintf(origen,"%ld",msg.origen);
        sprintf(destino,"%ld",msg.destino);
        Logger::logg(string("Reenviando mensaje recibido de: ")+origen+" sobre la cola hacia: "+destino);
        if(msgsnd(cola,&msg,sizeof(Museo)-sizeof(long),0)==-1){
            Logger::loggError("Error al escribir el mensaje ");
            exit(1);
        }
    }
    
    
    
    return 0;
}

