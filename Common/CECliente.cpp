/* 
 * File:   CE.cpp
 * Author: knoppix
 *
 * Created on June 29, 2015, 12:30 AM
 */

#include <cstdlib>
#include <sys/msg.h>

#include "enviar.cpp"
#include "recibir.cpp"
#include "Logger.h"
#include "MensajeAPuerta.h"


#define ID "CECliente"
using namespace std;

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
    
    long id = atoi(argv[1]);
    int cola = atoi(argv[2]);
    int socket = atoi(argv[3]);
    //int pidkill = atoi(argv[4]);
    
    if( (cola = msgget(cola,0660)) == -1){
        Logger::loggError("Error al encontrar la cola del cliente");
        exit(1);   
    }
    
    Logger::logg(string("Enviando mi ID: ")+argv[1]);
    MensajeAPuerta msg;
    msg.destinatario=id;
    if(enviar(socket,&msg,sizeof(MensajeAPuerta))<0){
        Logger::loggError("Error al recibir el mensaje ");
        exit(1);
    };
    
    while(true){
        
        Logger::logg("Esperando mensaje sobre el socket");
        if(recibir(socket,&msg,sizeof(MensajeAPuerta))<=0){
            Logger::loggError("Error al recibir el mensaje ");
            exit(1);
        };
        
        Logger::logg("Reenviando sobre la cola");
        if(msgsnd(cola,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
            Logger::loggError("Error al escribir el mensaje ");
            exit(1);
        }
    }
    
    Logger::closeLogger();
    
    return 0;
}

