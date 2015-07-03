/* 
 * File:   InterfazLocker.cpp
 * Author: knoppix
 * 
 * Created on June 16, 2015, 1:05 PM
 */

#include <sys/msg.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "../Common/Logger.h"
#include "../Common/MensajeAPuerta.h"

#include "InterfazLocker.h"
#include "Constantes.h"



InterfazLocker::InterfazLocker(int nroPuerta) {
    this->nroPuerta=nroPuerta;
    Logger::logg("Buscando la cola de recepcion del Locker");
    if( (colaEntrada = msgget(ftok(PUERTA_FILE_IPC,COLA_LOCKER),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de recepcion del Locker");
        exit(1);   
    }
    
    Logger::logg("Buscando la cola de respuesta deposito del Locker");
    if( (colaRespuesta = msgget(ftok(PUERTA_FILE_IPC,COLA_LOCKER_RESPUESTA),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de respuesta del Locker");
        exit(1);   
    }
    
    
}

int InterfazLocker::guardarPertenencia(int pertenencia){
    MensajeAPuerta msg;
    msg.myType=nroPuerta;
    msg.origen=getpid();//aca no es necesario el id por que el locker esta en la misma pc
    msg.destino=nroPuerta;
    msg.tipo=TIPO_DEPOSITO;
    msg.pertenenciasOTarjeta=pertenencia;
    if(msgsnd(colaEntrada,&msg,sizeof(msg)-sizeof(long),0)==-1){
        Logger::loggError("Error al enviarPeticion");
        exit(1);   
    }
    
    if(msgrcv(colaRespuesta,&msg,sizeof(msg)-sizeof(long),getpid(),0)==-1){
        Logger::loggError("Error al recibir respuesta");
        exit(1);   
    }
    return (msg.mensaje==MENSAJE_NO_PASAR) ? -1:msg.pertenenciasOTarjeta;
}

int InterfazLocker::tomarPertenencia(int tarjeta){
    MensajeAPuerta msg;
    msg.myType=nroPuerta;
    msg.origen=getpid();//aca no es necesario poner id por que el locker esta en la mimsa pc
    msg.destino=nroPuerta;
    msg.tipo=TIPO_RETIRO;
    msg.pertenenciasOTarjeta=tarjeta;
    if(msgsnd(colaEntrada,&msg,sizeof(msg)-sizeof(long),0)==-1){
        Logger::loggError("Error al enviarPeticion");
        exit(1);   
    }
    
    if(msgrcv(colaRespuesta,&msg,sizeof(msg)-sizeof(long),getpid(),0)==-1){
        Logger::loggError("Error al recibir respuesta");
        exit(1);   
    }
    return (msg.mensaje==MENSAJE_PASAR) ? msg.pertenenciasOTarjeta:-1;   
}

InterfazLocker::~InterfazLocker() {
}

