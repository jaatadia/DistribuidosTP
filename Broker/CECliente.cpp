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
#include "Constantes.h" //TODO sacar
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
    
    Logger::startLog(BROKER_LOGGER_DEFAULT_PATH,ID);
    
    long id = atoi(argv[1]);
    int cola = atoi(argv[2]);
    int socket = atoi(argv[3]);
    int pidkill = atoi(argv[4]);
    
    if( (cola = msgget(cola,0660)) == -1){
        Logger::loggError("Error al encontrar la cola del cliente");
        exit(1);   
    }
    
    MensajeAPuerta msg;
    msg.destinatario=id;
    if(enviar(socket,&msg,sizeof(MensajeAPuerta))<0){
        Logger::loggError("Error al recibir el mensaje ");
        exit(1);
    };
    
    while(true){
        
        if(recibir(socket,&msg,sizeof(MensajeAPuerta))<0){
            Logger::loggError("Error al recibir el mensaje ");
            exit(1);
        };
        
        if(msgsnd(cola,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
            Logger::loggError("Error al escribir el mensaje ");
            exit(1);
        }
    }
    
    Logger::closeLogger();
    kill(pidkill,SIGUSR1);
    
    return 0;
}

