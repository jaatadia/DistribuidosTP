/* 
 * File:   InterfazPuertaLocker.cpp
 * Author: knoppix
 * 
 * Created on June 16, 2015, 12:30 PM
 */

#include <sys/msg.h>

#include "InterfazPuertaLocker.h"
#include <stdlib.h>
#include "../Common/Logger.h"
#include "../Common/MensajeAPuerta.h"


InterfazPuertaLocker::InterfazPuertaLocker(int nroPuerta) {
    this->nroPuerta=nroPuerta;
    Logger::logg("Buscando la cola de recepcion del Locker");
    if( (colaEntrada = msgget(ftok(PUERTA_FILE_IPC,COLA_LOCKER),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de recepcion del Locker");
        exit(1);   
    }
    
    Logger::logg("Buscando la cola de devolucion del Locker");
    if( (colaRespuesta = msgget(ftok(PUERTA_FILE_IPC,COLA_LOCKER_RESPUESTA),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de devolucion del Locker");
        exit(1);   
    }

}

void InterfazPuertaLocker::recivirPedido(Pedido& pedido){
    MensajeAPuerta msg;
    if( (msgrcv(colaEntrada,&msg,sizeof(MensajeAPuerta)-sizeof(long),nroPuerta,0)) == -1){
        Logger::loggError("Error leer mensaje de entrada");
        exit(1);   
    }
    pedido.tipo=msg.tipo;
    pedido.puerta=msg.mensaje;
    pedido.pertenenciaOTarjeta=msg.pertenenciasOTarjeta;
}

void InterfazPuertaLocker::responderDeposito(int tarjeta){
    MensajeAPuerta msg;
    msg.destinatario=2*nroPuerta;//TODO revisar 
    msg.tipo=TIPO_DEPOSITO;
    msg.mensaje= (tarjeta!=-1) ? MENSAJE_PASAR:MENSAJE_NO_PASAR;
    msg.pertenenciasOTarjeta=tarjeta;
    if( (msgsnd(colaRespuesta,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)) == -1){
        Logger::loggError("Error al responder el mensaje de deposito");
        exit(1);   
    }
}

void InterfazPuertaLocker::responderExtraccion(int pertenencia){
    MensajeAPuerta msg;
    msg.destinatario=(2*nroPuerta)-1;//TODO revisar 
    msg.tipo=TIPO_RETIRO;
    msg.mensaje= (pertenencia!=-1) ? MENSAJE_PASAR:MENSAJE_NO_PASAR;
    msg.pertenenciasOTarjeta=pertenencia;
    if( (msgsnd(colaRespuesta,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)) == -1){
        Logger::loggError("Error al responder el mensaje de extraccion");
        exit(1);   
    }
}

InterfazPuertaLocker::~InterfazPuertaLocker() {
}

