/* 
 * File:   InterfazPersonaSalida.cpp
 * Author: knoppix
 * 
 * Created on June 14, 2015, 11:56 PM
 */

#include "InterfazPersonaSalida.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../Common/semaforo.h"
#include "../Common/MensajeAPuerta.h"

InterfazPersonaSalida::InterfazPersonaSalida(int numeroPuerta) {
    
    this->numero = numeroPuerta;
    
    //busco las colas
    Logger::logg("Buscando la cola de salida");
    if( (colaSalida = msgget(ftok(PUERTA_FILE_IPC,COLA_PUERTA_SALIDA_NORMAL),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de entrada");
        exit(1);   
    }

    Logger::logg("Buscando la cola de salida prioritaria");
    if( (colaSalidaPrioritaria = msgget(ftok(PUERTA_FILE_IPC,COLA_PUERTA_SALIDA_PRIORITARIA),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de salida prioritaria");
        exit(1);   
    }

    Logger::logg("Buscando la cola de respuesta");
    if( (colaSalidaRespuesta = msgget(ftok(PUERTA_FILE_IPC,COLA_PUERTA_SALIDA_RESPUESTA),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de respuesta");
        exit(1);   
    }
        
    //obtiene mutexs
    Logger::logg("Obtengo mutex de si hay personas y de los contadores de colas");
    semPuertaEsperando = getsem(PUERTA_FILE_IPC,SEM_PUERTA_ESPERANDO+(numeroPuerta-1)*DESP,PERMISOS);
    mutexColas = getsem(PUERTA_FILE_IPC,MUTEX_COLAS+(numeroPuerta-1)*DESP,PERMISOS);

    //busca y une a la memoria compartida
    Logger::logg("buscando la memoria compartida para la puerta");
    int shmid;
    if( (shmid = shmget(ftok(PUERTA_FILE_IPC,CONTADOR_COLAS+(numeroPuerta-1)*DESP), sizeof(ColasPuertas),PERMISOS)) == -1 ){
        Logger::loggError("Error al obtener la memoria compartida");
        exit(1);   
    }

    Logger::logg("Uniendose a la memoria compartida de contadores de colas");
    if ( (contador = (ColasPuertas*) shmat(shmid,NULL,0)) == (ColasPuertas*) -1 ){
        Logger::loggError("Error al atachearse a la memoria compartida");
        exit(1);   
    }
    
    //TODO pedir id
    static char broker[18];
    static char nroPuerta[18];
    static char id[18];
    static char colaEntrada[18];
    static char colaSalida[18];
    sprintf(broker,"broker");//TODO cambiar esto
    sprintf(nroPuerta,"%d",numeroPuerta);
    sprintf(id,"%d",(numeroPuerta*2)-1);//TODO PEDIR id
    sprintf(colaEntrada,"%d",ftok(PUERTA_FILE_IPC,COLA_PUERTA_SALIDA));
    sprintf(colaSalida,"%d",ftok(PUERTA_FILE_IPC,COLA_PUERTA_SALIDA_RESPUESTA));
    
    int childpid;
    if ((childpid=fork())<0){
        Logger::loggError("Error al crear el conectador ");
        exit(1);   
    }else if (childpid == 0){
        execlp(PATH_CONECTADOR_EXEC,NAME_CONECTADOR_EXEC,broker,id,colaEntrada,colaSalida,(char*)NULL);
        Logger::loggError("Error al cargar la imagen de ejecutable del Conectador");
        exit(1);
    }
    
    Logger::logg("Esperando que las conexiones se realizen");
    int status;
    wait(&status);
    if(WEXITSTATUS(status)!=0){
        Logger::loggError("Error al crear las conexiones");
        exit(1);   
    }
    
    if ((childpid=fork())<0){
        Logger::loggError("Error al crear el Encolador ");
        exit(1);   
    }else if (childpid == 0){
        execlp(PATH_ENCOLADOR_EXEC,NAME_ENCOLADOR_EXEC,nroPuerta,id,(char*)NULL);
        Logger::loggError("Error al cargar la imagen de ejecutable del Encolador");
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

    Logger::logg("Espero a que haya personas");
    if(p(semPuertaEsperando)==-1){
        Logger::loggError("Error al obtener el mutex de personas esperando para salir");
        exit(1);   
    }
    
    Logger::logg("Espero mutex de la gente esperando");
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
        if(v(semPuertaEsperando)==-1){
            Logger::loggError("Error al liberar la puerta de la espera");
            exit(1);   
        }
        Logger::logg("Liberado mutex de que hay personas");
    }
    
    persona.idPersona = msg.mensaje;
    persona.tipoPersona = msg.tipo;
    persona.pertenencias = msg.pertenenciasOTarjeta;
    
    Logger::logg("Libero mutex de la gente esperando");
    if(v(mutexColas)==-1){
        Logger::loggError("Error al liberar el mutex de el contador de las colas");
        exit(1);   
    }
}

void InterfazPersonaSalida::responder(int idPersona,bool puedePasar){

    MensajeAPuerta msg;
    msg.destinatario  = idPersona;
    msg.mensaje = (puedePasar) ? MENSAJE_PASAR : MENSAJE_NO_PASAR;
    if(msgsnd(colaSalidaRespuesta,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
        Logger::loggError("Error al responder a la persona");
        exit(1);   
    }
    (puedePasar) ? Logger::logg("Respondido a la persona que puede salir del museo") : Logger::logg("Respondido a la persona que NO puede salir del museo");
}

void InterfazPersonaSalida::responderInvestigador(int idInvestigador,int pertenencias){

    MensajeAPuerta msg;
    msg.destinatario  = idInvestigador;
    msg.mensaje = (pertenencias!=-1) ? MENSAJE_PASAR : MENSAJE_NO_PASAR;
    msg.pertenenciasOTarjeta = pertenencias;
    if(msgsnd(colaSalidaRespuesta,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
        Logger::loggError("Error al responder al investigador");
        exit(1);   
    }
    (pertenencias!=-1) ? Logger::logg("Respondido al investigador que puede salir del museo") : Logger::logg("Respondido al investigador que NO puede salir del museo");
}   
