/* 
 * File:   InterfazMuseo.cpp
 * Author: knoppix
 * 
 * Created on June 14, 2015, 5:05 PM
 */
#include "InterfazMuseoSalida.h"

#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>
#include <stdlib.h>
#include <sstream>

#include "../Common/semaforo.h"
#include "../Common/Logger.h"

#include "Constantes.h"


InterfazMuseoSalida::InterfazMuseoSalida() {
    
    Logger::logg("Obteniendo los mutex de estado de museo, lugar y cant de personas");
    if ( (mutexMuseo = getsem(MUSEO_FILE_IPC,MUTEX_MUSEO,PERMISOS)) == -1){
        Logger::loggError("No se pudo encontrar el mutex del museo");
        exit(1);
    };
    
    if ( (semLugar = getsem(MUSEO_FILE_IPC,SEM_LUGAR,PERMISOS)) == -1){
        Logger::loggError("No se pudo encontrar el mutex de lugar");
        exit(1);
    };
    
    Logger::logg("Obteniendo el museo");
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

bool InterfazMuseoSalida::salir(){
        
    Logger::logg("esperando el estado del museo");
    if (p(mutexMuseo)==-1){
        Logger::loggError("Error al obtener el mutex de estado");
        exit(1);   
    }
    
   
    if(myMuseum->personasAdentro==myMuseum->museoMax){
        if(v(semLugar)==-1){
            Logger::loggError("Error al liberar el lugar");
            exit(1);       
        }
    }

    myMuseum->personasAdentro=myMuseum->personasAdentro-1;
    std::stringstream ss;
    ss<<myMuseum->personasAdentro;
    Logger::logg(std::string("Ahora hay ")+ss.str()+" personas adentro del museo");
   
    Logger::logg("Devolviendo el mutex sobre el estado");
    if(v(mutexMuseo)==-1){
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

