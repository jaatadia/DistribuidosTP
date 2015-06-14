/* 
 * File:   InterfazPersonaEntrada.cpp
 * Author: knoppix
 * 
 * Created on June 14, 2015, 4:31 PM
 */

#include "InterfazPersonaEntrada.h"

InterfazPersonaEntrada::InterfazPersonaEntrada(int numeroPuerta) {
    
    this->numero=numeroPuerta;
    
    //busco las colas
    Logger::logg("Buscando la cola de entrada");
    if( (colaEntrada = msgget(ftok(DIRECTORIO_IPC,PUERTA_FILA),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de entrada");
        exit(1);   
    }

    Logger::logg("Buscando la cola de respuesta");
    if( (colaRespuesta = msgget(ftok(DIRECTORIO_IPC,PUERTA_RESP),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de respuesta");
        exit(1);   
    }
}

InterfazPersonaEntrada::~InterfazPersonaEntrada() {    
}

void InterfazPersonaEntrada::tomarPersona(Persona& persona){
    
    Logger::logg("Esperando persona");
    MensajeAPuerta msg;
    if( (msgrcv(colaEntrada,&msg,sizeof(MensajeAPuerta)-sizeof(long),numero,0)) == -1){
        Logger::loggError("Error leer mensaje de entrada");
        exit(1);   
    }
    request = msg; 
    
    persona.idPersona = msg.mensaje;
    persona.tipoPersona = msg.tipo;
    
}
    
void InterfazPersonaEntrada::responder(int idPersona,bool puedePasar){
    
    MensajeAPuerta msg;
    msg.destinatario  = request.mensaje;
    msg.mensaje = (puedePasar) ? MENSAJE_PASAR : MENSAJE_NO_PASAR;
    if(msgsnd(colaRespuesta,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
        Logger::loggError("Error responder a la persona");
        exit(1);   
    }
}
    
void InterfazPersonaEntrada::responderInvestigador(int idInvestigador,int tarjeta){
    
    MensajeAPuerta msg;
    msg.destinatario  = request.mensaje;
    msg.mensaje = (tarjeta) ? MENSAJE_PASAR : MENSAJE_NO_PASAR;
    msg.pertenenciasOTarjeta = tarjeta;
    if(msgsnd(colaRespuesta,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
        Logger::loggError("Error responder a la persona");
        exit(1);   
    }
    
}
    
