/* 
 * File:   CE.cpp
 * Author: knoppix
 *
 * Created on June 29, 2015, 12:30 AM
 */

#include <cstdlib>
#include <sys/msg.h>

#include "../Common/recibir.cpp"
#include "../Common/Logger.h"
#include "../Common/Museo.h"

#include "Constantes.h"


using namespace std;

#define ID "CEBrokerSHM"

//argv[1] fd socket
//argv[2] fd cola
//argv[3] kill
int main(int argc, char** argv) {
    
    if(argc!=3){
        printf("Mal uso 1: keyCola 2:fdSocket");
        return -1;
    }
    
    int cola = atoi(argv[1]);
    int socket = atoi(argv[2]);
    
    Logger::startLog(BROKER_LOGGER_DEFAULT_PATH,ID);
    
    if( (cola = msgget(cola,0660)) == -1){
        Logger::loggError("Error al encontrar la cola del broker");
        exit(1);   
    }
    
    while(true){
        Museo msg;
        
        Logger::logg("Esperando mensaje");
        if(recibir(socket,&msg,sizeof(Museo))<=0){
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
        //if(msg.mensaje==MENSAJE_END_COMMUNICATION){break;}
    }
    
    close(socket);
    Logger::closeLogger();
    
    return 0;
}

