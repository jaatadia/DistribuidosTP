/* 
 * File:   InterfazMuseo.cpp
 * Author: knoppix
 * 
 * Created on June 14, 2015, 5:05 PM
 */

#include "InterfazMuseoSalida.h"
#include "Simulador.h"
#include "semaforo.h"
#include "Logger.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>
#include <stdlib.h>
#include <sstream>


InterfazMuseoSalida::InterfazMuseoSalida() {
    
    Logger::logg("Obteniendo los mutex de estado de museo, lugar y cant de personas");
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

bool InterfazMuseoSalida::salir(){
        
    Logger::logg("esperando el estado del museo");
    if (p(mutexEstado)==-1){
        Logger::loggError("Error al obtener el mutex de estado");
        exit(1);   
    }

    Logger::logg("esperando el mutex sobre la cantidad de cosas");
    if (p(mutexPersonas)==-1){
        Logger::loggError("Error al obtener el mutex de la cantidad de cosas");
        exit(1);   
    }
    
   
    if(myMuseum->personasAdentro==myMuseum->museoMax){
        if(v(mutexLugar)==-1){
            Logger::loggError("Error al liberar el lugar");
            exit(1);       
        }
    }
    //TODO en el diagrama primero decrementa y despues libera el mutex!! -F es lo mismo cambia la condicion del if -J
    myMuseum->personasAdentro=myMuseum->personasAdentro-1;
    std::stringstream ss;
    ss<<myMuseum->personasAdentro;
    Logger::logg(std::string("Ahora hay ")+ss.str()+" personas adentro del museo");
   
    
    Logger::logg("Devolviendo el mutex");
        if(v(mutexPersonas)==-1){
            Logger::loggError("Error al devolver el mutex de cantidad de cantidad de personas");
            exit(1);   
        }
    
    Logger::logg("Devolviendo el mutex sobre el estado");
    if(v(mutexEstado)==-1){
        Logger::loggError("Error al devolver el mutex de estado del museo");
        exit(1);   
    }
    return true;
}

InterfazMuseoSalida::~InterfazMuseoSalida() {
    Logger::logg("Desuniendose de la memoria compartida");
    if(shmdt(myMuseum)==-1){
        Logger::loggError("Error al desatachearse de la memoria compartida");
        exit(1);   
    }
}

