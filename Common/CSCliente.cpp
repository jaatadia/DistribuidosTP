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
#include "MensajeAPuerta.h"


using namespace std;

//argv[1] id
//argv[2] fd socket
//argv[3] fd cola
//argv[4] kill

#define ID "CSCliente"

int main(int argc, char** argv) {
    
    if(argc<4){
        printf("Mal uso 1:id 1: keyCola 2:fdSocket 3:pidKill");
        return -1;
    }
    
    int id = atoi(argv[1]);
    int cola = atoi(argv[2]);
    int socket = atoi(argv[3]);
    int pidkill = atoi(argv[4]);
    
    Logger::startLog(LOGGER_DEFAULT_PATH,ID);
    
    if( (cola = msgget(cola,0660)) == -1){
        Logger::loggError("Error al encontrar la cola del cliente");
        exit(1);   
    }

    bool continuaComunicacion=true;
    while(continuaComunicacion){
        MensajeAPuerta msg;
        
        Logger::logg("Esperando mensaje sobre la cola");
        if(msgrcv(cola,&msg,sizeof(MensajeAPuerta)-sizeof(long),id,0)==-1){
            Logger::loggError("Error al escribir el mensaje ");
            exit(1);
        }
       
        msg.myType=msg.destino;
        
        Logger::logg("Reenvio mensaje por el socket");
        if(enviar(socket,&msg,sizeof(MensajeAPuerta))<=0){
            Logger::loggError("Error al recibir el mensaje ");
            exit(1);
        };
        
        continuaComunicacion=(msg.mensaje!=MENSAJE_END_COMMUNICATION);
    }
    
    close(socket);
    Logger::closeLogger();
    kill(pidkill,SIGUSR1);
    
    return 0;
}

