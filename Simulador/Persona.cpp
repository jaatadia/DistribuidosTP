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
    
    stringstream ss;
    ss<<entrada;
    
    
    //busco las colas
    Logger::logg("Buscando la cola de entrada nro "+ss.str());
    int colaEntrada,colaEntradaRespuesta;
    if( (colaEntrada = msgget(ftok(DIRECTORIO_IPC,PUERTA_FILA + DESP * entrada),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de entrada nro " +ss.str());
        exit(1);   
    }

    Logger::logg("Buscando la cola de respuesta"+ss.str());
    if( (colaEntradaRespuesta = msgget(ftok(DIRECTORIO_IPC,PUERTA_RESP + DESP * entrada),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de respuesta nro "+ss.str());
        exit(1);   
    }
    
    ss.str("");
    ss<<salida;
    //busco las colas de salida
    Logger::logg("Buscando la cola de entrada nro "+ss.str());
    if( (colaSalida = msgget(ftok(DIRECTORIO_IPC,PUERTA_SALIDA_FILA + DESP * salida),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de entrada nro " +ss.str());
        exit(1);   
    }

    Logger::logg("Buscando la cola de respuesta"+ss.str());
    if( (colaSalidaRespuesta = msgget(ftok(DIRECTORIO_IPC,PUERTA_SALIDA_RESP + DESP * salida),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de respuesta nro "+ss.str());
        exit(1);   
    }
    
    Mensaje msg;
    msg.destinatario=1;
    msg.mensaje=getpid();
    Logger::logg("Enviando mensaje");
    if(msgsnd(colaEntrada,&msg,sizeof(Mensaje)-sizeof(long),0)==-1){
        Logger::loggError("Error al escribir el mensaje "+ss.str());
        exit(1);
    }
    
    Logger::logg("Esperando respuesta");
    if(msgrcv(colaEntradaRespuesta,&msg,sizeof(Mensaje)-sizeof(long),getpid(),0)==-1){
        Logger::loggError("Error al leer el mensaje ");
        exit(1);
    }
    
    if(msg.mensaje==MENSAJE_NO_PASAR){
        Logger::logg("El museo esta cerrado me voy");
        return 0;
    }
    
    
    int childpid;
    std::stringstream pid; pid<<":"<<getpid();
    if( ( childpid = fork() ) < 0 ){
        Logger::loggError("Error al forkearse");
        exit(1);   
    }else if(childpid==0){
        execlp(PATH_WAKER_EXEC,NAME_WAKER_EXEC,(PERSONA_ID+pid.str()).c_str(),COLA_MATAR_PERSONAS_STR,(char*)NULL);
        Logger::loggError("Error al generar el waker");
        exit(1);
    }
    


    
    Logger::logg("Entre al museo");
    usleep(dormir);
    
    kill(childpid,SIGUSR1);
    int exitStatus;
    wait(&exitStatus);
    
    msg.mensaje=getpid();
    Logger::logg("Enviando mensaje para salir");
    if(msgsnd(colaSalida,&msg,sizeof(Mensaje)-sizeof(long),0)==-1){
        Logger::loggError("Error al escribir el mensaje "+ss.str());
        exit(1);
    }
    
    Logger::logg("Esperando respuesta");
    if(msgrcv(colaSalidaRespuesta,&msg,sizeof(Mensaje)-sizeof(long),getpid(),0)==-1){
        Logger::loggError("Error al leer el mensaje ");
        exit(1);
    }
    
    Logger::logg("Sali del museo");
    
    return 0;
}

