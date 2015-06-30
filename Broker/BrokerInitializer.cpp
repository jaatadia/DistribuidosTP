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


void crearColaBroker(){
    Logger::logg("Creando la cola del broker");
    if( (msgget(ftok(BROKER_FILE_IPC,COLA_BROKER),IPC_CREAT|IPC_EXCL|PERMISOS)) == -1){
        Logger::loggError("Error al crear la cola del broker");
        exit(1);   
    }
}

void crearBroker(key_t key){
    
    Logger::logg("Creando el proceso broker");

    static char strkey[12];
    sprintf(strkey,"%d",key);
    
    int childpid;
    if ((childpid=fork())<0){
        Logger::loggError("Error al crear el broker ");
        exit(1);   
    }else if (childpid == 0){
        execlp(PATH_BROKER_EXEC,NAME_BROKER_EXEC,strkey,(char*)NULL);
        Logger::loggError("Error al cargar la imagen de ejecutable del broker");
        exit(1);
    }


}

int main(int argc, char** argv) {

    Logger::startLog(BROKER_LOGGER_DEFAULT_PATH,BROKER_INITIALIZER_ID);
    
    crearCarpeta();
    crearColaBroker();
    crearBroker(ftok(BROKER_FILE_IPC,COLA_BROKER));
    
    Logger::closeLogger();
    
    return 0;
}

