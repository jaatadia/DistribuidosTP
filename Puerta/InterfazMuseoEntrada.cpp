/* 
 * File:   InterfazMuseo.cpp
 * Author: knoppix
 * 
 * Created on June 14, 2015, 5:05 PM
 */

#include "InterfazMuseoEntrada.h"
#include "../Common/semaforo.h"
#include "../Common/Logger.h"
#include "../Common/Conectador.h"
#include "../Common/Parser.h"

#include <sys/ipc.h>
#include <sys/msg.h>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include "Constantes.h"

InterfazMuseoEntrada::InterfazMuseoEntrada() {
    
    myId = getpid();//TODO pedir id
    
       //busco las colas
    Logger::logg("Buscando la cola de peticion");
    if( (colaPeticion = msgget(ftok(PUERTA_FILE_IPC,COLA_SHM),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de entrada");
        exit(1);   
    }

    Logger::logg("Buscando la cola de respuesta");
    if( (colaRespuesta = msgget(ftok(PUERTA_FILE_IPC,COLA_SHM_RESPUESTA),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de respuesta");
        exit(1);   
    }
    
    Parser::setPath("../broker.conf");
    int portCS = Parser::getIntParam("PUERTO_3");
    int portCE = Parser::getIntParam("PUERTO_4");
    if(portCS<0 || portCE<0){
        Logger::loggError("Error al leer los puertos del broker");
        exit(1);   
    }
 
    char broker[255];
    int result=-1;
    std::ifstream file;
    file.open("../brokers.conf");
    Logger::logg("Buscando broker");
    while((result==-1) && (!file.eof())){
        file.getline(broker,255);
        Logger::logg(std::string("Tratando de conectar con broker: ")+broker);
        result = conectToSHM(broker,myId,ftok(PUERTA_FILE_IPC,COLA_SHM_RESPUESTA),ftok(PUERTA_FILE_IPC,COLA_SHM),portCE,portCS);
    }
    file.close();    
    if(result!=0){
        Logger::loggError("Error al conectarse con el broker");
        exit(1);   
    }
    
}

bool InterfazMuseoEntrada::entrar(){

    bool result=false;
    bool found=false;
    Museo museo;
    while(!found){
        museo.myType=myId;
        museo.origen=myId;
        museo.destino=PETICION;
        if( (msgsnd(colaPeticion,&museo,sizeof(Museo)-sizeof(long),0)) == -1){
            Logger::loggError("Error leer mensaje de peticion");
            exit(1);   
        }
        
        if( (msgrcv(colaRespuesta,&museo,sizeof(Museo)-sizeof(long),myId,0)) == -1){
            Logger::loggError("Error leer mensaje de entrada");
            exit(1);   
        }
        
        if(!museo.estaAbierto){
            found=true;
            result=false;
            Logger::logg("El museo esta cerrado");
        }else if(museo.personasAdentro<museo.museoMax){
            museo.personasAdentro=museo.personasAdentro+1;
            char personas[15];
            sprintf(personas,"%d",museo.personasAdentro);
            Logger::logg(std::string("Entro una persona, ahora hay: ")+personas);
            found=true;
            result=true;
        }
        
        Logger::logg("Devolviendo la shm");
        museo.myType=myId;
        museo.origen=myId;
        museo.destino=DEVOLUCION;
        if( (msgsnd(colaPeticion,&museo,sizeof(Museo)-sizeof(long),0)) == -1){
            Logger::loggError("Error leer mensaje de peticion");
            exit(1);   
        }
    }
    return result;
}

InterfazMuseoEntrada::~InterfazMuseoEntrada() {
    //TODO cerrar comunicacion
    //TODO devolver id
}

