/* 
 * File:   CE.cpp
 * Author: knoppix
 *
 * Created on June 29, 2015, 12:30 AM
 */

#include <cstdlib>
#include <sys/msg.h>

#include "../Common/enviar.cpp"
#include "../Common/recibir.cpp"
#include "../Common/Logger.h"
#include "../Common/MensajeAPuerta.h"

#include "Constantes.h"


using namespace std;

#define ID "NameServer"

//argv[1] fd socket

int main(int argc, char** argv) {
    
    if(argc<4){
        printf("Mal uso 1: fd socket");
        return -1;
    }
    
    int socket = atoi(argv[1]);
 
    while(true){
        MensajeAPuerta msg;
        
        if(recibir(socket,&msg,sizeof(MensajeAPuerta))<0){
            Logger::loggError("Error al recibir el mensaje ");
            exit(1);
        };
        
        msg.destinatario = 10;//TODO cambiar
        
        if(enviar(socket,&msg,sizeof(MensajeAPuerta))<0){
            Logger::loggError("Error al recibir el mensaje ");
            exit(1);
        };
        
    }
    
    Logger::closeLogger();
    
    return 0;
}

