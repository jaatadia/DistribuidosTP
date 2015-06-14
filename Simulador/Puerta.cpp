/* 
 * File:   Puerta.cpp
 * Author: knoppix
 *
 * Created on March 26, 2015, 2:00 PM
 */

#include <stdlib.h>
#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <sstream>

#include "Logger.h"
#include "Simulador.h"
#include "semaforo.h"

using namespace std;


#define PUERTA_ID "Puerta"

//en argv[1] el numero de puerta
int main(int argc, char** argv) {
    
    if (argc != 2 ){
        Logger::startLog(LOGGER_DEFAULT_PATH,PUERTA_ID);
        Logger::loggError("No se pasaron los parametros correctos 1: numero de puerta");
        exit(1);
    }
    
    int numero = atoi(argv[1]);
    string puertaid=string(PUERTA_ID)+argv[1];
    Logger::startLog(LOGGER_DEFAULT_PATH,puertaid.c_str());
    
    Logger::logg("Obteniendo el mutex");
    int mutexEstado,mutexLugar,mutexPersonas;
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
    Museo* myMuseum;
    if ( (myMuseum = (Museo*) shmat(shmid,NULL,0)) == (Museo*) -1 ){
        Logger::loggError("Error al atachearse a la memoria compartida");
        exit(1);   
    }
   
    //busco las colas
    Logger::logg("Buscando la cola de entrada");
    int colaEntrada,colaRespuesta;
    if( (colaEntrada = msgget(ftok(DIRECTORIO_IPC,PUERTA_FILA + DESP * numero),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de entrada");
        exit(1);   
    }

    Logger::logg("Buscando la cola de respuesta");
    if( (colaRespuesta = msgget(ftok(DIRECTORIO_IPC,PUERTA_RESP + DESP * numero),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de respuesta");
        exit(1);   
    }
  
    while(true){
        int resultado=MENSAJE_PASAR;
        
        Logger::logg("Esperando persona");
        Mensaje msg;
        if( (msgrcv(colaEntrada,&msg,sizeof(Mensaje)-sizeof(long),0,0)) == -1){
            Logger::loggError("Error leer mensaje de entrada");
            exit(1);   
        }
        
        Logger::logg("Recibi persona, esperando el estado del museo");
        if (p(mutexEstado)==-1){
            Logger::loggError("Error al obtener el mutex de estado");
            exit(1);   
        }
        
        if(!myMuseum->estaAbierto){
            Logger::logg("El Museo esta cerrado");
            resultado=MENSAJE_NO_PASAR;
        }else{
            //si no devuelvo este mutex se produce un deadlock con la puerta de salida
            Logger::logg("Devolviendo el mutex sobre el estado");
            if(v(mutexEstado)==-1){
                Logger::loggError("Error al devolver el mutex de estado del museo");
                exit(1);   
            }
                        
            Logger::logg("Esperando un lugar");
            if(p(mutexLugar)==-1){
                Logger::loggError("Error al obtener el mutex de lugar");
                exit(1);   
            }
        
            Logger::logg("Recibi persona, esperando el estado del museo");
            if (p(mutexEstado)==-1){
                Logger::loggError("Error al obtener el mutex de estado");
                exit(1);   
            }
            
            if(!myMuseum->estaAbierto){
                Logger::logg("El Museo esta cerrado");
                resultado=MENSAJE_NO_PASAR;
                if(v(mutexLugar)==-1){
                    Logger::loggError("Error al devolver el mutex de lugar");
                    exit(1);   
                }
            }else{    

                Logger::logg("Esperando exclusion mutua sobre el contador");
                if(p(mutexPersonas)==-1){
                    Logger::loggError("Error al obtener el mutex de cantidad de personas");
                    exit(1);   
                }
                myMuseum->personasAdentro++;
                std::stringstream ss;
                ss<<myMuseum->personasAdentro;
                Logger::logg(string("Entro la persona ahora hay ")+ss.str());
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
        }
        
        Logger::logg("Devolviendo el mutex sobre el estado");
        if(v(mutexEstado)==-1){
            Logger::loggError("Error al devolver el mutex de estado del museo");
            exit(1);   
        }
        
        msg.destinatario  = msg.mensaje;
        msg.mensaje = resultado;
        if(msgsnd(colaRespuesta,&msg,sizeof(Mensaje)-sizeof(long),0)==-1){
            Logger::loggError("Error responder a la persona");
            exit(1);   
        }
        
    }

}
