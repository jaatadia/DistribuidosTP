/* 
 * File:   BrokerInintializer.cpp
 * Author: knoppix
 *
 * Created on June 29, 2015, 9:36 AM
 */

#include <cstdlib>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>

#include "../Common/Logger.h"
#include "../Common/Parser.h"
#include "Constantes.h"

#define BROKER_INITIALIZER_ID "BrokerInitializer"
using namespace std;

void crearCarpeta(){
    string commandoCrear=string("sudo mkdir --mode=0777 -p ")+BROKER_DIRECTORIO_IPC;
    string commandoFile=string("touch ")+BROKER_FILE_IPC;
    Logger::logg(string("Creando carpeta de IPCs: ")+BROKER_DIRECTORIO_IPC);
    system(commandoCrear.c_str());
    system(commandoFile.c_str());
}


void crearColaBroker(key_t key){
    Logger::logg("Creando la cola del broker");
    if( (msgget(key ,IPC_CREAT|IPC_EXCL|PERMISOS)) == -1){
        Logger::loggError("Error al crear la cola del broker");
        exit(1);   
    }
}

void crearBroker(key_t key,string idPuertoCE,string idPuertoCS){
    
    Logger::logg("Creando el proceso broker");
    
    Parser::setPath("../broker.conf");
    static char strkey[12];
    static char puertoCE[12];
    static char puertoCS[12];
    sprintf(strkey,"%d",key);
    int portCE = Parser::getIntParam(idPuertoCE);
    int portCS = Parser::getIntParam(idPuertoCS);
    
    if(portCE<0){
        Logger::loggError("Error al leer los puertos del broker");
        exit(1);   
    }
    
    if(portCS<0){
        Logger::loggError("Error al leer los puertos del broker");
        exit(1);   
    }
    
    sprintf(puertoCE,"%d",portCE);
    sprintf(puertoCS,"%d",portCS);
    
    int childpid;
    if ((childpid=fork())<0){
        Logger::loggError("Error al crear el servidor de ce ");
        exit(1);   
    }else if (childpid == 0){
        execlp(PATH_SERVER_CE_EXEC,NAME_SERVER_CE_EXEC,strkey,puertoCE,(char*)NULL);
        Logger::loggError("Error al cargar la imagen de ejecutable del broker");
        exit(1);
    }

    if ((childpid=fork())<0){
        Logger::loggError("Error al crear el servidor de cs ");
        exit(1);   
    }else if (childpid == 0){
        execlp(PATH_SERVER_CS_EXEC,NAME_SERVER_CS_EXEC,strkey,puertoCS,(char*)NULL);
        Logger::loggError("Error al cargar la imagen de ejecutable del broker");
        exit(1);
    }
    

}

void crearBrokerSHM(key_t key){
    
    Parser::setPath("../museo.cfg");
    int resultOpen, resultCantGente, resultMaxPersonas;
    if((resultOpen = Parser::getBoolParam("MUSEO_OPEN")) < 0 ){
        Logger::loggError("Error al leer la configuracion del museo si esta abierto");
        exit(1);   
    }
    
    if((resultCantGente = Parser::getIntParam("MUSEO_CANT")) < 0 ){
        Logger::loggError("Error al leer la configuracion del museo cuantas personas hay");
        exit(1);   
    }
    
    if((resultMaxPersonas = Parser::getIntParam("MUSEO_MAX_PERSONAS")) < 0 ){
        Logger::loggError("Error al leer la configuracion del museo cual es el maximo de personas");
        exit(1);   
    }
    
    static char strkey[12];
    static char mOpen[12];
    static char mPer[12];
    static char mMax[12];
    sprintf(strkey,"%d",key);
    sprintf(mOpen,"%d",resultOpen);
    sprintf(mPer,"%d",resultCantGente);
    sprintf(mMax,"%d",resultMaxPersonas);
    int childpid;
    if ((childpid=fork())<0){
        Logger::loggError("Error al crear el manejador de shm");
        exit(1);   
    }else if (childpid == 0){
        execlp(PATH_BROKER_SHM_EXEC,NAME_BROKER_SHM_EXEC,strkey,mOpen,mPer,mMax,(char*)NULL);
        Logger::loggError("Error al cargar la imagen de manejador de shm");
        exit(1);
    }
    
}

int main(int argc, char** argv) {

    Logger::startLog(BROKER_LOGGER_DEFAULT_PATH,BROKER_INITIALIZER_ID);
    
    crearCarpeta();
    crearColaBroker(ftok(BROKER_FILE_IPC,COLA_BROKER));
    crearBroker(ftok(BROKER_FILE_IPC,COLA_BROKER),"PUERTO_1","PUERTO_2");
    
    crearColaBroker(ftok(BROKER_FILE_IPC,COLA_BROKER_SHM));
    crearBroker(ftok(BROKER_FILE_IPC,COLA_BROKER),"PUERTO_3","PUERTO_4");
    crearBrokerSHM(ftok(BROKER_FILE_IPC,COLA_BROKER));
    
    Logger::closeLogger();
    
    return 0;
}

