/* 
 * File:   InterfazPersonaSalida.cpp
 * Author: knoppix
 * 
 * Created on June 14, 2015, 11:56 PM
 */

#include "InterfazPersonaSalida.h"

InterfazPersonaSalida::InterfazPersonaSalida() {
    //busco las colas
    Logger::logg("Buscando la cola de entrada");
    int colaEntrada,colaRespuesta;
    if( (colaEntrada = msgget(ftok(DIRECTORIO_IPC,PUERTA_SALIDA_FILA),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de entrada");
        exit(1);   
    }

    Logger::logg("Buscando la cola de respuesta");
    if( (colaRespuesta = msgget(ftok(DIRECTORIO_IPC,PUERTA_SALIDA_RESP),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de respuesta");
        exit(1);   
    }    
}

InterfazPersonaSalida::~InterfazPersonaSalida() {
}

void InterfazPersonaSalida::tomarPersona(Persona& persona){
    //TODO
    Logger::logg("Esperando persona");
    MensajeAPuerta msg;
    if( (msgrcv(colaEntrada,&msg,sizeof(MensajeAPuerta)-sizeof(long),numero,0)) == -1){
        Logger::loggError("Error al leer mensaje de salida");
        exit(1);   
    }
    
    persona.idPersona = msg.mensaje;
    persona.tipoPersona = msg.tipo;
}

void InterfazPersonaSalida::responder(int idPersona,bool puedePasar){
    //TODO
    MensajeAPuerta msg;
    msg.destinatario  = idPersona;
    msg.mensaje = MENSAJE_PASAR;
    if(msgsnd(colaRespuesta,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
        Logger::loggError("Error responder a la persona");
        exit(1);   
    }
}

void InterfazPersonaSalida::responderInvestigador(int idInvestigador,int pertenencias){
    //TODO
    MensajeAPuerta msg;
    msg.destinatario  = idInvestigador;
    msg.mensaje = (pertenencias) ? MENSAJE_PASAR : MENSAJE_NO_PASAR;
    msg.pertenenciasOTarjeta = pertenencias;
    if(msgsnd(colaRespuesta,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
        Logger::loggError("Error responder a la persona");
        exit(1);   
    }
}

void InterfazPersonaSalida::disminuirEsperando(bool normal,int nroPuerta){

    //obtiene mutex
    int mutexPuertaEsperando = getsem(MUTEX_PUERTA_ESPERANDO+(nroPuerta-1)*DESP);
    int mutexColas = getsem(MUTEX_CONTADOR_COLAS_PUERTAS+(nroPuerta-1)*DESP);


    Logger::logg("buscando la memoria compartida para InterfazPersonaSalida");
    int shmid;
    if( (shmid = shmget(ftok(DIRECTORIO_IPC,CONTADOR_COLAS_PUERTAS+(nroPuerta-1)*DESP), sizeof(int),PERMISOS)) == -1 ){
        Logger::loggError("Error al obtener la memoria compartida");
        exit(1);   
    }

    Logger::logg("Uniendose a la memoria compartida");
    int* contadorNormal;
    if ( (contadorNormal = (int*) shmat(shmid,NULL,0)) == (int*) -1 ){
        Logger::loggError("Error al atachearse a la memoria compartida");
        exit(1);   
    }


    if(p(colaNormal)==-1){
        Logger::loggError("Error al obtener el mutex de la cola normal");
        exit(1);   
    }

    if(p(colaPrioritaria)==-1){
        Logger::loggError("Error al obtener el mutex de la cola prioritaria");
        exit(1);   
    }
    if((*contadorNormal==0)&&(*contadorPrioritario==0)){
        if(v(puertaEsperando)==-1){
            Logger::loggError("Error al liberar la puerta de la espera");
            exit(1);   
        }
    }

    if(normal){
        *contadorNormal=*contadorNormal-1;
    }else{
        *contadorPrioritario=*contadorPrioritario-1;
    }

    Logger::logg("Desuniendose de la memoria compartida");
    if(shmdt(contadorNormal)==-1){
        Logger::loggError("Error al desatachearse de la memoria compartida");
        exit(1);   
    }    

    Logger::logg("Desuniendose de la memoria compartida");
    if(shmdt(contadorPrioritario)==-1){
        Logger::loggError("Error al desatachearse de la memoria compartida");
        exit(1);   
    }

}
