/* 
 * File:   InterfazPersonaSalida.cpp
 * Author: knoppix
 * 
 * Created on June 14, 2015, 11:56 PM
 */

#include "InterfazPersonaSalida.h"

InterfazPersonaSalida::InterfazPersonaSalida(int numeroPuerta) {
    
    this->numero = numeroPuerta;
    
    //busco las colas
    Logger::logg("Buscando la cola de salida");
    if( (colaSalida = msgget(ftok(DIRECTORIO_IPC,PUERTA_SALIDA_FILA),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de entrada");
        exit(1);   
    }

    Logger::logg("Buscando la cola de salida prioritaria");
    if( (colaSalidaPrioritaria = msgget(ftok(DIRECTORIO_IPC,PUERTA_SALIDA_FILA_PRIORITARIA),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de salida prioritaria");
        exit(1);   
    }

    Logger::logg("Buscando la cola de respuesta");
    if( (colaSalidaRespuesta = msgget(ftok(DIRECTORIO_IPC,PUERTA_SALIDA_RESP),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de respuesta");
        exit(1);   
    }
        
    //obtiene mutexs
    mutexPuertaEsperando = getsem(MUTEX_PUERTA_ESPERANDO+(numeroPuerta-1)*DESP);
    mutexColas = getsem(MUTEX_CONTADOR_COLAS_PUERTAS+(numeroPuerta-1)*DESP);

    //busca y une a la memoria compartida
    Logger::logg("buscando la memoria compartida para la puerta");
    int shmid;
    if( (shmid = shmget(ftok(DIRECTORIO_IPC,CONTADOR_COLAS_PUERTAS+(numeroPuerta-1)*DESP), sizeof(ColasPuertas),PERMISOS)) == -1 ){
        Logger::loggError("Error al obtener la memoria compartida");
        exit(1);   
    }

    Logger::logg("Uniendose a la memoria compartida");
    ColasPuertas* contador;
    if ( (contador = (ColasPuertas*) shmat(shmid,NULL,0)) == (ColasPuertas*) -1 ){
        Logger::loggError("Error al atachearse a la memoria compartida");
        exit(1);   
    }
}

InterfazPersonaSalida::~InterfazPersonaSalida() {
    
    Logger::logg("Desuniendose de la memoria compartida");
    if(shmdt(contador)==-1){
        Logger::loggError("Error al desatachearse de la memoria compartida");
        exit(1);   
    }
}

void InterfazPersonaSalida::tomarPersona(Persona& persona){
    //TODO
    if(p(mutexPuertaEsperando)==-1){
        Logger::loggError("Error al obtener el mutex de personas esperando para salir");
        exit(1);   
    }
    
    if(p(mutexColas)==-1){
        Logger::loggError("Error al obtener el mutex de el contador de las colas");
        exit(1);   
    }
    
    MensajeAPuerta msg;
    if(contador->investigadores == 0){
        Logger::logg("Tomo persona para salir");
        if( (msgrcv(colaSalida,&msg,sizeof(MensajeAPuerta)-sizeof(long),numero,0)) == -1){
            Logger::loggError("Error al leer mensaje de salida de persona");
            exit(1);   
        }
        contador->personasNormales=contador->personasNormales-1;
    }else{
        Logger::logg("Tomo investigador para salir");
        if( (msgrcv(colaSalidaPrioritaria,&msg,sizeof(MensajeAPuerta)-sizeof(long),numero,0)) == -1){
            Logger::loggError("Error al leer mensaje de salida de investigador");
            exit(1);   
        }
        contador->investigadores=contador->investigadores-1;
    }
    
    if((contador->personasNormales!=0)||(contador->investigadores!=0)){
        if(v(mutexPuertaEsperando)==-1){
            Logger::loggError("Error al liberar la puerta de la espera");
            exit(1);   
        }
    }
    
    persona.idPersona = msg.mensaje;
    persona.tipoPersona = msg.tipo;
    
     if(v(mutexColas)==-1){
        Logger::loggError("Error al liberar el mutex de el contador de las colas");
        exit(1);   
    }
}

void InterfazPersonaSalida::responder(int idPersona,bool puedePasar){
    //TODO
    /*MensajeAPuerta msg;
    msg.destinatario  = idPersona;
    msg.mensaje = MENSAJE_PASAR;
    if(msgsnd(colaRespuesta,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
        Logger::loggError("Error responder a la persona");
        exit(1);   
    }*/
}

void InterfazPersonaSalida::responderInvestigador(int idInvestigador,int pertenencias){
    //TODO
    /*MensajeAPuerta msg;
    msg.destinatario  = idInvestigador;
    msg.mensaje = (pertenencias) ? MENSAJE_PASAR : MENSAJE_NO_PASAR;
    msg.pertenenciasOTarjeta = pertenencias;
    if(msgsnd(colaRespuesta,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
        Logger::loggError("Error responder a la persona");
        exit(1);   
    }*/
}