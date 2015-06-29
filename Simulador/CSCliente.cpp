/* 
 * File:   CE.cpp
 * Author: knoppix
 *
 * Created on June 29, 2015, 12:30 AM
 */

#include <cstdlib>
#include <sys/msg.h>

#include "enviar.cpp"
#include "Logger.h"
#include "Simulador.h"
#include "MensajeAPuerta.h"


using namespace std;

//argv[1] fd socket
//argv[2] fd cola
//argv[3] kill

#define ID "CSCliente"

int main(int argc, char** argv) {
    
    if(argc<4){
        printf("Mal uso 1: keyCola 2:fdSocket 3:pidKill");
        return -1;
    }
    
    int cola = atoi(argv[1]);
    int socket = atoi(argv[1]);
    int pidkill = atoi(argv[3]);
    
    Logger::startLog(LOGGER_DEFAULT_PATH,ID);
    
    if( (cola = msgget(cola,0660)) == -1){
        Logger::loggError("Error al encontrar la cola del cliente");
        exit(1);   
    }

    while(true){
        MensajeAPuerta msg;
        
        if(msgrcv(cola,&msg,sizeof(MensajeAPuerta)-sizeof(long),0,0)==-1){
            Logger::loggError("Error al escribir el mensaje ");
            exit(1);
        }
        
        if(enviar(socket,&msg,sizeof(MensajeAPuerta))<0){
            Logger::loggError("Error al recibir el mensaje ");
            exit(1);
        };
        
        
    }
    
    Logger::closeLogger();
    kill(pidkill,SIGUSR1);
    
    return 0;
}

