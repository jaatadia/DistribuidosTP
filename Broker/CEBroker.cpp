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
#include "../Common/MensajeAPuerta.h"

#include "Constantes.h"


using namespace std;

#define ID "CEBroker"

//argv[1] fd socket
//argv[2] fd cola
//argv[3] kill
int main(int argc, char** argv) {
    
    if(argc<4){
        printf("Mal uso 1: keyCola 2:fdSocket 3:pidKill");
        return -1;
    }
    
    int cola = atoi(argv[1]);
    int socket = atoi(argv[2]);
    int pidkill = atoi(argv[3]);
    
    Logger::startLog(BROKER_LOGGER_DEFAULT_PATH,ID);
    
    if( (cola = msgget(cola,0660)) == -1){
        Logger::loggError("Error al encontrar la cola del broker");
        exit(1);   
    }
    
    while(true){
        MensajeAPuerta msg;
        
        Logger::logg("Esperando mensaje");
        if(recibir(socket,&msg,sizeof(MensajeAPuerta))<0){
            Logger::loggError("Error al recibir el mensaje ");
            exit(1);
        };
        
        Logger::logg("Reenviando mensaje");
        if(msgsnd(cola,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
            Logger::loggError("Error al escribir el mensaje ");
            exit(1);
        }
    }
    
    Logger::closeLogger();
    kill(pidkill,SIGUSR1);
    
    return 0;
}

