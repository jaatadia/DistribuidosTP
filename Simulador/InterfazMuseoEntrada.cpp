/* 
 * File:   InterfazMuseo.cpp
 * Author: knoppix
 * 
 * Created on June 14, 2015, 5:05 PM
 */

#include "InterfazMuseoEntrada.h"
#include "Simulador.h"
#include "semaforo.h"
#include "Logger.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>
#include <stdlib.h>
#include <sstream>


InterfazMuseoEntrada::InterfazMuseoEntrada() {
    
    Logger::logg("Obteniendo el mutex");
    if ( (mutexEstado = getsem(MUTEX_ESTADO)) == -1){
        Logger::loggError("No se pudo encontrar el mutex de estado");
        exit(1);
    };
    
    if ( (mutexLugar = getsem(MUTEX_LUGAR)) == -1){
        Logger::loggError("No se pudo encontrar el mutex de lugar");
        exit(1);
    };
    
    if ( (mutexPersonas = getsem(MUTEX_CONTADOR)) == -1){
        Logger::loggError("No se pudo encontrar el mutex del contador");
        exit(1);
    };
    
    Logger::logg("Obteniendo el museo");
    int shmid;
    if( (shmid = shmget(ftok(DIRECTORIO_IPC,MUSEO), sizeof(Museo),PERMISOS)) == -1 ){
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
    if(p(mutexLugar)==-1){
        Logger::loggError("Error al obtener el mutex de lugar");
        exit(1);   
    }

    Logger::logg("hay lugar en el museo, esperando el estado del museo");
    if (p(mutexEstado)==-1){
        Logger::loggError("Error al obtener el mutex de estado");
        exit(1);   
    }

    if(!myMuseum->estaAbierto){
        Logger::logg("El Museo esta cerrado");
        if(v(mutexLugar)==-1){
            Logger::loggError("Error al devolver el mutex de lugar");
            exit(1);   
        }
        if(v(mutexEstado)==-1){
            Logger::loggError("Error al devolver el mutex de Estado");
            exit(1);   
        }
        return false;

    }else{    
        Logger::logg("Esperando exclusion mutua sobre el contador");
        if(p(mutexPersonas)==-1){
            Logger::loggError("Error al obtener el mutex de cantidad de personas");
            exit(1);   
        }
        
        myMuseum->personasAdentro++;
        std::stringstream ss;
        ss<<myMuseum->personasAdentro;
        Logger::logg(std::string("Entro la persona ahora hay ")+ss.str());
        
        if(!(myMuseum->personasAdentro==myMuseum->museoMax)){
            Logger::logg("Sigue habiendo lugar, devuelvo el mutex de lugar");
            if(v(mutexLugar)==-1){
                Logger::loggError("Error al devolver el mutex de lugar");
                exit(1);   
            }
        }else{
            ss.str("");ss<<myMuseum->museoMax;
            Logger::logg("El museo se lleno, maximo: "+ss.str());
        }
        Logger::logg("Devolviendo el mutex");
        if(v(mutexPersonas)==-1){
            Logger::loggError("Error al devolver el mutex de cantidad de cantidad de personas");
            exit(1);   
        }
    }

    Logger::logg("Devolviendo el mutex sobre el estado");
    if(v(mutexEstado)==-1){
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

