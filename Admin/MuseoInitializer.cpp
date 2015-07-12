/* 
 * File:   MuseoInitializer.cpp
 * Author: knoppix
 *
 * Created on June 29, 2015, 6:27 PM
 */

#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

using namespace std;

#include "../Common/Logger.h"
#include "../Common/semaforo.h"
#include "../Common/Parser.h"
#include "../Common/Museo.h"
#include "Constantes.h"

#define ID "MuseoInitializer"

void crearCarpetas(){
    string commandoCrear=string("sudo mkdir --mode=0777 -p ")+MUSEO_DIRECTORIO_IPC;
    string commandoArchivo=string("sudo touch ")+MUSEO_FILE_IPC;
    Logger::logg(string("Creando carpeta de IPCs: ")+MUSEO_DIRECTORIO_IPC);
    Logger::logg(string("Creando archivo de IPCs: ")+MUSEO_FILE_IPC);
    system(commandoCrear.c_str());
    system(commandoArchivo.c_str());
}

void crearMuseo(){
    
    Logger::logg("Creando la memoria compartida");
    int shmid;
    if( (shmid = shmget(ftok(MUSEO_FILE_IPC,MUSEO), sizeof(Museo),IPC_CREAT|IPC_EXCL|PERMISOS)) == -1 ){
        Logger::loggError("Error al crear la memoria compartida");
        exit(1);   
    }
    
    Logger::logg("Uniendose a la memoria compartida");
    Museo* myMuseum;
    if ( (myMuseum = (Museo*) shmat(shmid,NULL,0)) == (Museo*) -1 ){
        Logger::loggError("Error al atachearse a la memoria compartida");
        exit(1);   
    }
    
    
    Logger::logg("Inicializando la memoria compartida");
    
    int result;
    
    if((result = Parser::getBoolParam(MUSEO_OPEN)) < 0 ){
        Logger::loggError("Error al leer la configuracion del museo si esta abierto");
        exit(1);   
    }
    myMuseum->estaAbierto= (result == BOOL_TRUE);
    
    if((result = Parser::getIntParam(MUSEO_CANT_GENTE)) < 0 ){
        Logger::loggError("Error al leer la configuracion del museo cuantas personas hay");
        exit(1);   
    }
    myMuseum->personasAdentro=result;
    
    /*if((result = Parser::getIntParam(MUSEO_PUERTAS)) < 0 ){
        Logger::loggError("Error al leer la configuracion del museo cuantas puertas");
        exit(1);   
    }
    myMuseum->cantPuertas=result;*/
    
    if((result = Parser::getIntParam(MUSEO_MAX_PERSONAS)) < 0 ){
        Logger::loggError("Error al leer la configuracion del museo cual es el maximo de personas");
        exit(1);   
    }
    myMuseum->museoMax=result;
    
    
    
    Logger::logg("Desuniendose de la memoria compartida");
    if(shmdt(myMuseum)==-1){
        Logger::loggError("Error al desatachearse de la memoria compartida");
        exit(1);   
    }
    
    Logger::logg("Creando semaforo de exclusion mutua del estado");
    int semid;
    if ((semid = creasem(MUSEO_FILE_IPC,MUTEX_MUSEO,PERMISOS))== -1){
        Logger::loggError("Error al crear el semaforo de mutex del estado");
        exit(1);   
    };
    
    Logger::logg("Inicializando el semaforo de exclusion mutua del estado");
    if (inisem(semid,1)==-1){
        Logger::loggError("Error al inicializar semaforo de mutex del estado");
        exit(1);   
    }
    
    Logger::logg("Creando semaforo que indica si hay lugar");
    if ((semid = creasem(MUSEO_FILE_IPC,SEM_LUGAR,PERMISOS))== -1){
        Logger::loggError("Error al crear el semaforo de lugar libre");
        exit(1);   
    };
    
    Logger::logg("Inicializando el semaforo que indica si hay lugar");
    if (inisem(semid,1)==-1){
        Logger::loggError("Error al inicializar semaforo que indica si hay lugar");
        exit(1);   
    }
    
}

int main(int argc, char** argv) {
    Parser::setPath(MUSEO_CONF);
    Logger::startLog(MUSEO_LOGGER_DEFAULT_PATH,ID);
    crearCarpetas();
    crearMuseo();
    Logger::closeLogger();
    return 0;
}

