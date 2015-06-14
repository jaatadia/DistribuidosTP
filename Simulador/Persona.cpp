/* 
 * File:   persona.cpp
 * Author: knoppix
 *
 * Created on March 31, 2015, 4:50 PM
 */

#include <cstdlib>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sstream>
#include <unistd.h>
#include <signal.h>
#include "Logger.h"
#include "Simulador.h"
#include "InterfazPuerta.h"
#include <sys/wait.h>

using namespace std;

/*
 * 
 */
#define PERSONA_ID "Persona"

int  colaSalida;
int  colaSalidaRespuesta;

void myHandler(int signal){
    Logger::logg("Me avisaron que salga");
    Mensaje msg;
    msg.destinatario=1;
    msg.mensaje=getpid();
    Logger::logg("Enviando mensaje para salir");
    if(msgsnd(colaSalida,&msg,sizeof(Mensaje)-sizeof(long),0)==-1){
        Logger::loggError("Error al escribir el mensaje ");
        exit(1);
    }
    
    Logger::logg("Esperando respuesta");
    if(msgrcv(colaSalidaRespuesta,&msg,sizeof(Mensaje)-sizeof(long),getpid(),0)==-1){
        Logger::loggError("Error al leer el mensaje ");
        exit(1);
    }
    
    Logger::logg("Sali del museo");
    exit(1);
}



//argv[1] que puerta debe usar para entrar,argv[2] tiempo a dormir, argv[3] puerta para slir
int main(int argc, char** argv) {
    Logger::startLog(LOGGER_DEFAULT_PATH,PERSONA_ID);
    
    struct sigaction oldHandler;
    struct sigaction newHandler;
    newHandler.sa_handler=myHandler;
    newHandler.sa_flags=0;
    sigfillset(&(newHandler.sa_mask));
    
    if(sigaction(SIGUSR1,&newHandler,&oldHandler) == -1){
        Logger::loggError("Error al encontrar asignar el signal handler a la persona");
        exit(1);   
    }
    
    if (argc != 4 ){
        Logger::loggError("No se pasaron los parametros correctos 1: numero de puerta, 2: tiempo a dormri, 3: puerta para salir");
        exit(1);
    }
    
    
    int entrada = atoi(argv[1]);
    int dormir = atoi(argv[2]);
    int salida = atoi(argv[3]);
    

    InterfazPuerta puerta;
    puerta.entrar(entrada);
    puerta.pasear(dormir);
    puerta.salir(salida);
    
    return 0;
}

