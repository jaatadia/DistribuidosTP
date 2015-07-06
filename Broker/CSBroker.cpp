/* 
 * File:   CE.cpp
 * Author: knoppix
 *
 * Created on June 29, 2015, 12:30 AM
 */

#include <cstdlib>
#include <sys/msg.h>
#include <signal.h>
#include <unistd.h>

#include "../Common/enviar.cpp"
#include "../Common/recibir.cpp"
#include "../Common/Logger.h"
#include "../Common/MensajeAPuerta.h"
#include "Constantes.h"

#define ID "CSBroker"
using namespace std;

static int mySocket;

//argv[1] fd socket
//argv[2] fd cola
//argv[3] kill
int main(int argc, char** argv) {
    
    if(argc!=3){
        printf("Mal uso 1: keyCola 2:fdSocket");
        return -1;
    }
    
    int cola = atoi(argv[1]);
    mySocket = atoi(argv[2]);
    
    Logger::startLog(BROKER_LOGGER_DEFAULT_PATH,ID);
    
    if( (cola = msgget(cola,0660)) == -1){
        Logger::loggError("Error al encontrar la cola del broker");
        exit(1);   
    }

    Logger::logg("Esperando ID a leer");
    MensajeAPuerta msg;
    if(recibir(mySocket,&msg,sizeof(MensajeAPuerta))<=0){
        Logger::loggError("Error al recibir el mensaje ");
        exit(1);
    };
    long id = msg.myType;
    
    char idNumber[20];
    sprintf(idNumber,"%ld",id);
    Logger::logg(string("ID recibido: ")+idNumber);
    
    while(true){
        
        Logger::logg("Esperando un mensaje");
        if(msgrcv(cola,&msg,sizeof(MensajeAPuerta)-sizeof(long),id,0)==-1){
            Logger::loggError("Error al escribir el mensaje ");
            exit(1);
        }
        
        if(msg.mensaje==MENSAJE_END_COMMUNICATION){break;}
        
        char origen[14];
        char destino[14];
        sprintf(origen,"%ld",msg.origen);
        sprintf(destino,"%ld",msg.destino);
        Logger::logg(string("Reenviando mensaje recibido de: ")+origen+" sobre el socket hacia: "+destino);
        if(enviar(mySocket,&msg,sizeof(MensajeAPuerta))<=0){
            Logger::loggError("Error al recibir el mensaje ");
            exit(1);
        };
        
        
    }
    
    Logger::logg("Terminando conexion");
    close(mySocket);
    Logger::closeLogger();

    
    return 0;
}

