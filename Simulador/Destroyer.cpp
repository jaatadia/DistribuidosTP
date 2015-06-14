/* 
 * File:   Destroyer.cpp
 * Author: knoppix
 *
 * Created on March 28, 2015, 4:42 PM
 */

#include <cstdlib>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sstream>

#include "Logger.h"
#include "semaforo.h"
#include "Simulador.h"
#include "Parser.h"

using namespace std;

/*
 * 
 */

void killIPCs(){
    
    Logger::logg("Eliminando el semaforo de Mutex");
    int semid;
    if ( (semid= getsem(MUTEX_ESTADO))==-1){
        Logger::loggError("Error obteniendo el semaforo de Mutex");
    }else if (elisem(semid) == -1){
        Logger::loggError("Error eliminando el semaforo de Mutex");
    }
    
    Logger::logg("Eliminando el semaforo de Mutex");
    if ( (semid= getsem(MUTEX_LUGAR))==-1){
        Logger::loggError("Error obteniendo el semaforo de Mutex");
    }else if (elisem(semid) == -1){
        Logger::loggError("Error eliminando el semaforo de Mutex");
    }
   
    Logger::logg("Eliminando el semaforo de Mutex");
    if ( (semid= getsem(MUTEX_CONTADOR))==-1){
        Logger::loggError("Error obteniendo el semaforo de Mutex");
    }else if (elisem(semid) == -1){
        Logger::loggError("Error eliminando el semaforo de Mutex");
    }
    
    Logger::logg("Eliminando la memoria compartida");
    int memid;
    if ( (memid = shmget(ftok(DIRECTORIO_IPC,MUSEO),sizeof(Museo),PERMISOS)) == -1){
        Logger::loggError("Error obteniendo la memoria compartida");
    }else if ( shmctl(memid,IPC_RMID,NULL) == -1){
        Logger::loggError("Error eliminando la memoria compartida");
    } 
    
    int result;
    if( (result = Parser::getIntParam(MUSEO_PUERTAS)) < 0){
        Logger::loggError("Error obteniendo la cantidad de puertas");
    }else{
        for (int i=0;i<result;i++){
            int cola;
            std::stringstream ss;
            ss<<i;
            
            Logger::logg("Buscando la cola de entrada nro "+ss.str());
            if( (cola = msgget(ftok(DIRECTORIO_IPC,PUERTA_FILA + DESP * i),PERMISOS)) == -1){
                Logger::loggError("Error al encontrar la cola de entrada numero " + ss.str());
            }else{
                if(msgctl(cola,IPC_RMID,NULL) == -1){
                    Logger::loggError("Error al eliminar la cola de entrada numero " + ss.str());
                }
            }
            
            Logger::logg("Buscando la cola de respuesta nro "+ss.str());
            if( (cola = msgget(ftok(DIRECTORIO_IPC,PUERTA_RESP + DESP * i),PERMISOS)) == -1){
                Logger::loggError("Error al encontrar la cola de respuesta numero " + ss.str());
            }else{
                if(msgctl(cola,IPC_RMID,NULL) == -1){
                    Logger::loggError("Error al eliminar la cola de respuesta numero " + ss.str());
                }
            }
            
            Logger::logg("Buscando la cola de salida nro "+ss.str());
            if( (cola = msgget(ftok(DIRECTORIO_IPC,PUERTA_SALIDA_FILA + DESP * i),PERMISOS)) == -1){
                Logger::loggError("Error al encontrar la cola de salida numero " + ss.str());
            }else{
                if(msgctl(cola,IPC_RMID,NULL) == -1){
                    Logger::loggError("Error al eliminar la cola de salida numero " + ss.str());
                }
            }
            
            Logger::logg("Buscando la cola de respuesta salida nro "+ss.str());
            if( (cola = msgget(ftok(DIRECTORIO_IPC,PUERTA_SALIDA_RESP + DESP * i),PERMISOS)) == -1){
                Logger::loggError("Error al encontrar la cola de salida respuesta numero " + ss.str());
            }else{
                if(msgctl(cola,IPC_RMID,NULL) == -1){
                    Logger::loggError("Error al eliminar la cola de salida respuesta numero " + ss.str());
                }
            }
            
        }
    } 
    
    
    Logger::logg("Buscando la cola de matar personas");
    int cola;
    if( (cola = msgget(ftok(DIRECTORIO_IPC,COLA_MATAR_PERSONAS),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de matar personas ");
    }else{
        if(msgctl(cola,IPC_RMID,NULL) == -1){
            Logger::loggError("Error al eliminar la cola de matar personas");
        }
    }
    
}




#define DESTROYER_ID "Destroyer"
int main(int argc, char** argv) {
    
    Logger::startLog(LOGGER_DEFAULT_PATH,DESTROYER_ID);
    Parser::setPath(MUSEO_CONF);
    
    killIPCs();
    
    Logger::closeLogger();
}

