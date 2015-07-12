/* 
 * File:   PuertaInitializer.cpp
 * Author: knoppix
 *
 * Created on June 29, 2015, 6:07 PM
 */

#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>


#include "../Common/semaforo.h"
#include "../Common/Logger.h"

#include "Constantes.h"

using namespace std;

#define ID "PuertaInitializer"

void crearCarpetas(){
    string commandoCrear=string("sudo mkdir --mode=0777 -p ")+PUERTA_DIRECTORIO_IPC;
    string commandoArchivo=string("sudo touch ")+PUERTA_FILE_IPC;
    Logger::logg(string("Creando carpeta de IPCs: ")+PUERTA_DIRECTORIO_IPC);
    Logger::logg(string("Creando archivo de IPCs: ")+PUERTA_FILE_IPC);
    system(commandoCrear.c_str());
    system(commandoArchivo.c_str());
}

void crearColas(){

    int cola;
    //creo las colas para la shm
    if( (cola = msgget(ftok(PUERTA_FILE_IPC,COLA_SHM),IPC_CREAT|PERMISOS)) == -1){
        Logger::loggError("Error al crear la cola de shm");
        exit(1);   
    }
    
    if( (cola = msgget(ftok(PUERTA_FILE_IPC,COLA_SHM_RESPUESTA),IPC_CREAT|PERMISOS)) == -1){
        Logger::loggError("Error al crear la cola de shm");
        exit(1);   
    }
    
    //creo las colas para la puerta
    Logger::logg("Creando la cola de entrada");
    if( (cola = msgget(ftok(PUERTA_FILE_IPC,COLA_PUERTA_ENTRADA),IPC_CREAT|PERMISOS)) == -1){
        Logger::loggError("Error al crear la cola de entrada");
        exit(1);   
    }

    Logger::logg("Creando la cola de respuesta de entrada");
    if( (cola = msgget(ftok(PUERTA_FILE_IPC,COLA_PUERTA_ENTRADA_RESPUESTA),IPC_CREAT|PERMISOS)) == -1){
        Logger::loggError("Error al crear la cola de respuesta de entrada");
        exit(1);   
    }    

    //creo las colas para la puerta de salida
    
    Logger::logg("Creando la cola para el recibidor de personas de salida");
    if( (cola = msgget(ftok(PUERTA_FILE_IPC,COLA_PUERTA_SALIDA),IPC_CREAT|PERMISOS)) == -1){
        Logger::loggError("Error al crear la cola de salida");
        exit(1);   
    }
    
    
    Logger::logg("Creando la cola de salida");
    if( (cola = msgget(ftok(PUERTA_FILE_IPC,COLA_PUERTA_SALIDA_NORMAL),IPC_CREAT|PERMISOS)) == -1){
        Logger::loggError("Error al crear la cola de salida");
        exit(1);   
    }

    //creo las colas para la puerta de salida prioritarias
    Logger::logg("Creando la cola de salida prioritaria");
    if( (cola = msgget(ftok(PUERTA_FILE_IPC,COLA_PUERTA_SALIDA_PRIORITARIA),IPC_CREAT|PERMISOS)) == -1){
        Logger::loggError("Error al crear la cola de salida");
        exit(1);   
    }
    
    Logger::logg("Creando la cola de respuesta de salida");
    if( (cola = msgget(ftok(PUERTA_FILE_IPC,COLA_PUERTA_SALIDA_RESPUESTA),IPC_CREAT|PERMISOS)) == -1){
        Logger::loggError("Error al crear la cola de respuesta de salida");
        exit(1);   
    }
    
    Logger::logg(string("Creando cola para recepcion de pedidos del locker"));
    if( (cola = msgget(ftok(PUERTA_FILE_IPC,COLA_LOCKER),IPC_CREAT|PERMISOS)) == -1){
        Logger::loggError("Error al crear la cola para el locker");
        exit(1);   
    }
    
    Logger::logg(string("Creando cola para respuesta de pedidos de extracciones del locker"));
    if( (cola = msgget(ftok(PUERTA_FILE_IPC,COLA_LOCKER_RESPUESTA),IPC_CREAT|PERMISOS)) == -1){
        Logger::loggError("Error al crear la cola de respuesta para el locker");
        exit(1);   
    }
}

void crearMemoria(int nroPuerta){
        int semid;
        if ((semid = creasem(PUERTA_FILE_IPC,SEM_PUERTA_ESPERANDO+(nroPuerta-1)*DESP,PERMISOS)) == -1){
          Logger::loggError("Error al crear el semaforo de personas esperando para una puerta");
          exit(1);   
        }
        
        Logger::logg("Inicializando el semaforo de personas esperando para una puerta");
        if (inisem(semid,0)==-1){
            Logger::loggError("Error al inicializar el semaforo de personas esperando para una puerta");
            exit(1);   
        }
        
        if ((semid = creasem(PUERTA_FILE_IPC,MUTEX_COLAS+(nroPuerta-1)*DESP,PERMISOS)) == -1){
          Logger::loggError("Error al crear el semaforo de exclusion mutua del contador de personas esperando en una puerta");
          exit(1);   
        }
        
        Logger::logg("Inicializando el semaforo de exclusion mutua del contador de personas esperando en una puerta");
        if (inisem(semid,1)==-1){
            Logger::loggError("Error al inicializar el semaforo de exclusion mutua del contador de personas esperando en una puerta");
            exit(1);   
        }
        
        
        Logger::logg("Creando la memoria compartida para la puerta");
        int shmid;
        if( (shmid = shmget(ftok(PUERTA_FILE_IPC,CONTADOR_COLAS+(nroPuerta-1)*DESP), sizeof(ColasPuertas),IPC_CREAT|IPC_EXCL|PERMISOS)) == -1 ){
            Logger::loggError("Error al crear la memoria compartida");
            exit(1);   
        }

        Logger::logg("Uniendose a la memoria compartida");
        ColasPuertas* contador;
        if ( (contador = (ColasPuertas*) shmat(shmid,NULL,0)) == (ColasPuertas*) -1 ){
            Logger::loggError("Error al atachearse a la memoria compartida");
            exit(1);   
        }

        contador->investigadores =  0;
        contador->personasNormales =  0;

        Logger::logg("Desuniendose de la memoria compartida");
        if(shmdt(contador)==-1){
            Logger::loggError("Error al desatachearse de la memoria compartida");
            exit(1);   
        }  
}

int main(int argc, char** argv) {

    if(argc!=2){
        printf("parametros incorrectos 1: numero de puerta");
        return -1;
    }
    
    Logger::startLog(PUERTA_LOGGER_DEFAULT_PATH,ID);
    
    int numeroPuerta = atoi(argv[1]);
    
    crearCarpetas();
    crearColas();
    crearMemoria(numeroPuerta);
    
    int childpid;
    if ((childpid=fork())<0){
        Logger::loggError("Error al crear la puerta ");
        exit(1);   
    }else if (childpid == 0){
        execlp(PATH_PUERTA_EXEC,NAME_PUERTA_EXEC,argv[1],(char*)NULL);
        Logger::loggError("Error al cargar la imagen de la puerta");
        exit(1);
    }

    if ((childpid=fork())<0){
        Logger::loggError("Error al crear la puerta de salida");
        exit(1);   
    }else if (childpid == 0){
        execlp(PATH_PUERTA_SALIDA_EXEC,NAME_PUERTA_SALIDA_EXEC,argv[1],(char*)NULL);
        Logger::loggError("Error al cargar la imagen de la puerta de salida");
        exit(1);
    }
    
    if ((childpid=fork())<0){
        Logger::loggError("Error al crear el locker");
        exit(1);   
    }else if (childpid == 0){
        execlp(PATH_LOCKER_EXEC,NAME_LOCKER_EXEC,argv[1],(char*)NULL);
        Logger::loggError("Error al cargar la imagen del locker");
        exit(1);
    }
    
    Logger::closeLogger();
    
    return 0;
}

