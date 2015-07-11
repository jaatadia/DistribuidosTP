/* 
 * File:   InterfazMuseoAdministrador.cpp
 * Author: knoppix
 * 
 * Created on June 16, 2015, 11:38 AM
 */

#include "InterfazMuseoAdministrador.h"

#include "../Common/Logger.h"
#include "../Common/semaforo.h"
#include <sys/types.h>
#include <sys/shm.h>
#include <stdlib.h>

#include "Constantes.h"

InterfazMuseoAdministrador::InterfazMuseoAdministrador() {
    Logger::logg("Obteniendo el mutex del estado del museo");
    if ( (mutexEstado = getsem(MUSEO_FILE_IPC,MUTEX_MUSEO,PERMISOS)) == -1){
        Logger::loggError("No se pudo encontrar el mutex");
        exit(1);
    };
    
    Logger::logg("Obteniendo el museo (shm)");
    int shmid;
    if( (shmid = shmget(ftok(MUSEO_FILE_IPC,MUSEO), sizeof(Museo),PERMISOS)) == -1 ){
        Logger::loggError("Error al encontrar la memoria compartida");
        exit(1);   
    }
    
    Logger::logg("Uniendose al museo");
    if ( (myMuseum = (Museo*) shmat(shmid,0,0)) == (Museo*) -1 ){
        Logger::loggError("Error al atachearse a la memoria compartida");
        exit(1);   
    }
    
}


void InterfazMuseoAdministrador::abrir(){
    Logger::logg("Abriendo el museo");
    if(p(mutexEstado)==-1){
        Logger::logg("Error al obteren el mutex");
        exit(1);
    }
    
    myMuseum->estaAbierto=true;    
    
    if(v(mutexEstado)==-1){
        myMuseum->estaAbierto=false;    
        Logger::loggError("Error al liberar el mutex");
        exit(1);   
    }    
}

void InterfazMuseoAdministrador::cerrar(){
    
    Logger::logg("Esperando el control sobre el estado");
    if(p(mutexEstado)==-1){
        Logger::loggError("Error al tomar el mutex");
        exit(1);   
    }
    
    Logger::logg("Cerrando el museo");
    myMuseum->estaAbierto=false;    
    
    if(v(mutexEstado)==-1){
        Logger::loggError("Error al liberar el mutex");
    }    
    
}

InterfazMuseoAdministrador::~InterfazMuseoAdministrador() {
    if ( shmdt(myMuseum) == -1 ){
        Logger::loggError("Error al desatachearse a la memoria compartida");
    }
}

