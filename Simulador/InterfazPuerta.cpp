/* 
 * File:   InterfazPuerta.cpp
 * Author: knoppix
 * 
 * Created on June 14, 2015, 2:58 PM
 */

#include "InterfazPuerta.h"

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

    InterfazPuerta::InterfazPuerta(){
        
    }

    // devuelve si pudo entrar o no 
    bool InterfazPuerta::entrar(int numeroPuerta){
        //TODO 
        
    stringstream ss;
    ss<<numeroPuerta;

    //busco las colas
    Logger::logg("Buscando la cola de entrada nro "+ss.str());
    int colaEntrada,colaEntradaRespuesta;
    if( (colaEntrada = msgget(ftok(DIRECTORIO_IPC,PUERTA_FILA),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de entrada nro " +ss.str());
        exit(1);   
    }

    Logger::logg("Buscando la cola de respuesta"+ss.str());
    if( (colaEntradaRespuesta = msgget(ftok(DIRECTORIO_IPC,PUERTA_RESP),PERMISOS)) == -1){
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
    }
        
        
        return (msg.mensaje==MENSAJE_PASAR);
        
    }
    
    //devuelve -1 si no pudo entrar, o la tarjeta en caso contrario
    int InterfazPuerta::entrarInvestigador(int numeroPuerta,int pertenencias){
        //TODO
        return -1;
    }
    
    //pasea por tanto tiempo o hasta que le digan que salga
    void InterfazPuerta::pasear(int milisegundos){
        //TODO
        usleep(milisegundos);
    }
    
    //true pudo salir, false no
    bool InterfazPuerta::salir(int numeroPuerta){
    Mensaje msg;
    msg.destinatario=1;
    msg.mensaje=getpid();

    
    stringstream ss;
    ss<<numeroPuerta;

    int colaSalida;
    int colaSalidaRespuesta;
    //busco las colas de salida
    Logger::logg("Buscando la cola de salida");
    if( (colaSalida = msgget(ftok(DIRECTORIO_IPC,PUERTA_SALIDA_FILA),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de entrada nro " +ss.str());
        exit(1);   
    }

    Logger::logg("Buscando la cola de respuesta");
    if( (colaSalidaRespuesta = msgget(ftok(DIRECTORIO_IPC,PUERTA_SALIDA_RESP),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de respuesta nro "+ss.str());
        exit(1);   
    }
        
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

        return false;
    }
    
    
    //devuelve -1 en caso de que no sea la puerta correcta o las pertenencias
    int InterfazPuerta::salirInvestigador(int numeroPuerta,int tarjeta){
        //TODO
        return false;
    }
    
    InterfazPuerta::~InterfazPuerta(){
        
    }

