/* 
 * File:   InterfazMuseoAdministrador.cpp
 * Author: knoppix
 * 
 * Created on June 16, 2015, 11:38 AM
 */

#include "InterfazMuseoAdministrador.h"

#include "../Common/Logger.h"
#include "../Common/Parser.h"
#include "../Common/Conectador.h"
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>

#include "Constantes.h"

InterfazMuseoAdministrador::InterfazMuseoAdministrador() {
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

void InterfazMuseoAdministrador::abrir(){
    Museo museo;
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

    museo.estaAbierto=true;
    Logger::logg("Abierto el museo");
    
    Logger::logg("Devolviendo la shm");
    museo.myType=myId;
    museo.origen=myId;
    museo.destino=DEVOLUCION;
    if( (msgsnd(colaPeticion,&museo,sizeof(Museo)-sizeof(long),0)) == -1){
        Logger::loggError("Error leer mensaje de peticion");
        exit(1);   
    }
    
}

void InterfazMuseoAdministrador::cerrar(){
    Museo museo;
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

    museo.estaAbierto=false;
    Logger::logg("Cerrado el museo");
    
    Logger::logg("Devolviendo la shm");
    museo.myType=myId;
    museo.origen=myId;
    museo.destino=DEVOLUCION;
    if( (msgsnd(colaPeticion,&museo,sizeof(Museo)-sizeof(long),0)) == -1){
        Logger::loggError("Error leer mensaje de peticion");
        exit(1);   
    }
}

InterfazMuseoAdministrador::~InterfazMuseoAdministrador() {
    //TODO liberar id
    //Logger::logg("Cerrando conexiones");//esto se llama despues de cerrar el log y tira segfault
    Museo museo;
    museo.myType=myId;
    museo.origen=myId;
    museo.destino=myId;
    if( (msgsnd(colaPeticion,&museo,sizeof(Museo)-sizeof(long),0)) == -1){
        //Logger::loggError("Error leer mensaje de peticion");//esto se llama despues de cerrar el log y tira segfault
        printf("Error al terminar conexion");
        exit(1);   
    }
}

