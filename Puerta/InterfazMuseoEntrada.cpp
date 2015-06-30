/* 
 * File:   InterfazMuseo.cpp
 * Author: knoppix
 * 
 * Created on June 14, 2015, 5:05 PM
 */

#include "InterfazMuseoEntrada.h"
#include "../Common/semaforo.h"
#include "../Common/Logger.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>
#include <stdlib.h>
#include <sstream>


InterfazMuseoEntrada::InterfazMuseoEntrada() {
    
    Logger::logg("Obteniendo el semaforo para blokeo");
    if ( (semLugar = getsem(PUERTA_FILE_IPC,SEM_LUGAR,PERMISOS)) == -1){
        Logger::loggError("No se pudo encontrar el semaforo para bloqueo");
        exit(1);
    };
    
    Logger::logg("Obteniendo el mutex para el museo");
    if ( (mutexMuseo = getsem(PUERTA_FILE_IPC,MUTEX_MUSEO,PERMISOS)) == -1){
        Logger::loggError("No se pudo encontrar el mutex del museo");
        exit(1);
    };
    
    Logger::logg("Obteniendo el museo");
    int shmid;
    if( (shmid = shmget(ftok(PUERTA_FILE_IPC,MUSEO), sizeof(Museo),PERMISOS)) == -1 ){
        Logger::loggError("Error al encontrar la memoria compartida");
        exit(1);   
    }
    
    Logger::logg("Uniendose al museo");
    if ( (myMuseum = (Museo*) shmat(shmid,0,0)) == (Museo*) -1 ){
        Logger::loggError("Error al atachearse a la memoria compartida");
        exit(1);   
    }
}

bool InterfazMuseoEntrada::entrar(){
        
    Logger::logg("Esperando un lugar");
    if(p(semLugar)==-1){
        Logger::loggError("Error al obtener el mutex de lugar");
        exit(1);   
    }

    Logger::logg("hay lugar en el museo, esperando el estado del museo");
    if (p(mutexMuseo)==-1){
        Logger::loggError("Error al obtener el mutex de estado");
        exit(1);   
    }

    if(!myMuseum->estaAbierto){
        Logger::logg("El Museo esta cerrado");
        if(v(semLugar)==-1){
            Logger::loggError("Error al devolver el mutex de lugar");
            exit(1);   
        }
        if(v(mutexMuseo)==-1){
            Logger::loggError("Error al devolver el mutex de Estado");
            exit(1);   
        }
        return false;

    }else{    
        myMuseum->personasAdentro++;
        std::stringstream ss;
        ss<<myMuseum->personasAdentro;
        Logger::logg(std::string("Entro la persona ahora hay ")+ss.str());
        
        if(!(myMuseum->personasAdentro==myMuseum->museoMax)){
            Logger::logg("Sigue habiendo lugar, devuelvo el mutex de lugar");
            if(v(semLugar)==-1){
                Logger::loggError("Error al devolver el mutex de lugar");
                exit(1);   
            }
        }else{
            ss.str("");ss<<myMuseum->museoMax;
            Logger::logg("El museo se lleno, maximo: "+ss.str());
        }
    }

    Logger::logg("Devolviendo el mutex sobre el Museo");
    if(v(mutexMuseo)==-1){
        Logger::loggError("Error al devolver el mutex de estado del museo");
        exit(1);   
    }
    return true;
}

InterfazMuseoEntrada::~InterfazMuseoEntrada() {
    Logger::logg("Desuniendose de la memoria compartida");
    if(shmdt(myMuseum)==-1){
        Logger::loggError("Error al desatachearse de la memoria compartida");
        exit(1);   
    }
}

