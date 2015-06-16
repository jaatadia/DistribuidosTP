/* 
 * File:   InterfazLocker.cpp
 * Author: knoppix
 * 
 * Created on June 16, 2015, 1:05 PM
 */

#include <sys/msg.h>

#include "InterfazLocker.h"
#include "Simulador.h"
#include "Logger.h"
#include <stdlib.h>

InterfazLocker::InterfazLocker(int nroPuerta) {
    this->nroPuerta=nroPuerta;
    Logger::logg("Buscando la cola de recepcion del Locker");
    if( (colaEntrada = msgget(ftok(DIRECTORIO_IPC,COLA_LOCKER),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de recepcion del Locker");
        exit(1);   
    }
    
    Logger::logg("Buscando la cola de respuesta del Locker");
    if( (colaRespuestaDeposito = msgget(ftok(DIRECTORIO_IPC,COLA_LOCKER_RESPUESTA_DEPOSITO),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de respuesta del Locker");
        exit(1);   
    }
    
    Logger::logg("Buscando la cola de respuesta del Locker");
    if( (colaRespuestaExtraccion = msgget(ftok(DIRECTORIO_IPC,COLA_LOCKER_RESPUESTA_EXTRACCION),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de respuesta del Locker");
        exit(1);   
    }
    
    //TODO Agregar una tercera cola para leer respuestas de un tipo de un lado y otras del otro (crearla en el initializer)
}

int InterfazLocker::guardarPertenencia(int pertenencia){
    MensajeAPuerta msg;
    msg.destinatario=1;
    msg.tipo=TIPO_DEPOSITO;
    msg.pertenenciasOTarjeta=pertenencia;
    if(msgsnd(colaEntrada,&msg,sizeof(msg)-sizeof(long),0)==-1){
        Logger::loggError("Error al enviarPeticion");
        exit(1);   
    }
    
    if(msgrcv(colaRespuestaDeposito,&msg,sizeof(msg)-sizeof(long),nroPuerta,0)==-1){
        Logger::loggError("Error al recibir respuesta");
        exit(1);   
    }
    return (msg.mensaje==MENSAJE_PASAR) ? -1:msg.pertenenciasOTarjeta;
}

int InterfazLocker::tomarPertenencia(int tarjeta){
    MensajeAPuerta msg;
    msg.destinatario=1;
    msg.tipo=TIPO_RETIRO;
    msg.pertenenciasOTarjeta=tarjeta;
    if(msgsnd(colaEntrada,&msg,sizeof(msg)-sizeof(long),0)==-1){
        Logger::loggError("Error al enviarPeticion");
        exit(1);   
    }
    
    if(msgrcv(colaRespuestaExtraccion,&msg,sizeof(msg)-sizeof(long),nroPuerta,0)==-1){
        Logger::loggError("Error al recibir respuesta");
        exit(1);   
    }
    return (msg.mensaje==MENSAJE_PASAR) ? -1:msg.pertenenciasOTarjeta;   
}

InterfazLocker::~InterfazLocker() {
}

