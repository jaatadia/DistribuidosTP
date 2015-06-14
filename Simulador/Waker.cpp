/* 
 * File:   Waker.cpp
 * Author: knoppix
 *
 * Created on March 31, 2015, 2:20 PM
 */
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <signal.h>
#include "Logger.h"
#include "Simulador.h"

using namespace std;

/*
 * 
 */


#define WAKER_ID "Waker"


//argv[1] parametro de ID, argv[2] parametro para el ftok de la cola
int main(int argc, char** argv) {
    
    int cola;
    Logger::startLog(LOGGER_DEFAULT_PATH,(string(WAKER_ID)+"_"+argv[1]).c_str());
    if( (cola = msgget(ftok(DIRECTORIO_IPC,atoi(argv[2])),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de espera");
        exit(1);   
    }
    
    Mensaje msg;
    if(msgrcv(cola,&msg,sizeof(Mensaje)-sizeof(long),0,0)==-1){
        Logger::loggError("No recibi mensaje de terminacion");
    }else{
        Logger::logg("Recibi mensaje de terminacion, terminando proceso padre");
        kill(getppid(),SIGUSR1);
    }
    
    return 0;
}

