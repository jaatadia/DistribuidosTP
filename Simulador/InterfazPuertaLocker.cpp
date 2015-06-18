/* 
 * File:   InterfazPuertaLocker.cpp
 * Author: knoppix
 * 
 * Created on June 16, 2015, 12:30 PM
 */

#include <sys/msg.h>

#include "InterfazPuertaLocker.h"
#include "Logger.h"
#include <stdlib.h>
#include "Simulador.h"

InterfazPuertaLocker::InterfazPuertaLocker() {
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

}

void InterfazPuertaLocker::recivirPedido(Pedido& pedido){
    MensajeAPuerta msg;
    if( (msgrcv(colaEntrada,&msg,sizeof(MensajeAPuerta)-sizeof(long),0,0)) == -1){
        Logger::loggError("Error leer mensaje de entrada");
        exit(1);   
    }
    pedido.tipo=msg.tipo;
    pedido.puerta=msg.mensaje;
    pedido.pertenenciaOTarjeta=msg.pertenenciasOTarjeta;
}

void InterfazPuertaLocker::responderDeposito(int puerta, int tarjeta){
    MensajeAPuerta msg;
    msg.tipo=TIPO_DEPOSITO;
    msg.mensaje= (tarjeta!=-1) ? MENSAJE_PASAR:MENSAJE_NO_PASAR;
    msg.pertenenciasOTarjeta=tarjeta;
    if( (msgsnd(colaRespuestaDeposito,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)) == -1){
        Logger::loggError("Error al responder el mensaje de deposito");
        exit(1);   
    }
}

void InterfazPuertaLocker::responderExtraccion(int puerta, int pertenencia){
    MensajeAPuerta msg;
    msg.tipo=TIPO_RETIRO;
    msg.mensaje= (pertenencia!=-1) ? MENSAJE_PASAR:MENSAJE_NO_PASAR;
    msg.pertenenciasOTarjeta=pertenencia;
    if( (msgsnd(colaRespuestaExtraccion,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)) == -1){
        Logger::loggError("Error al responder el mensaje de extraccion");
        exit(1);   
    }
}

InterfazPuertaLocker::~InterfazPuertaLocker() {
}

