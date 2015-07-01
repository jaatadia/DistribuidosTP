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

void crearBroker(key_t key){
    
    Logger::logg("Creando el proceso broker");
    
    Parser::setPath("../Broker.conf");
    static char strkey[12];
    static char puerto1[12];
    static char puerto2[12];
    sprintf(strkey,"%d",key);
    std::string port1 = Parser::getStringParam("PUERTO_1");
    std::string port2 = Parser::getStringParam("PUERTO_2");
    
    if(port1.empty()){
        Logger::loggError("Error al leer los puertos del broker");
        exit(1);   
    }
    
    if(port2.empty()){
        Logger::loggError("Error al leer los puertos del broker");
        exit(1);   
    }
    
    sprintf(puerto1,"%s",port1.c_str());
    sprintf(puerto2,"%s",port2.c_str());
    
    int childpid;
    if ((childpid=fork())<0){
        Logger::loggError("Error al crear el broker ");
        exit(1);   
    }else if (childpid == 0){
        execlp(PATH_BROKER_EXEC,NAME_BROKER_EXEC,strkey,puerto1,puerto2,(char*)NULL);
        Logger::loggError("Error al cargar la imagen de ejecutable del broker");
        exit(1);
    }


}

int main(int argc, char** argv) {

    Logger::startLog(BROKER_LOGGER_DEFAULT_PATH,BROKER_INITIALIZER_ID);
    
    crearCarpeta();
    crearColaBroker(ftok(BROKER_FILE_IPC,COLA_BROKER));
    crearBroker(ftok(BROKER_FILE_IPC,COLA_BROKER));
    
    Logger::closeLogger();
    
    return 0;
}

