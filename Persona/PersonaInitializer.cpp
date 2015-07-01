/* 
 * File:   PersonaInitializer.cpp
 * Author: knoppix
 *
 * Created on June 30, 2015, 2:12 PM
 */

#include <cstdlib>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "Constantes.h"
#include "../Common/Logger.h"

using namespace std;

void crearCarpeta(){
    string commandoCrear=string("sudo mkdir --mode=0777 -p ")+PERSONA_DIRECTORIO_IPC;
    string commandoFile=string("touch ")+PERSONA_FILE_IPC;
    Logger::logg(string("Creando carpeta de IPCs: ")+PERSONA_DIRECTORIO_IPC);
    system(commandoCrear.c_str());
    system(commandoFile.c_str());
}

#define ID "PersonaInitializer"
int main(int argc, char** argv) {
    
    Logger::startLog(LOGGER_DEFAULT_PATH,ID);
    
    crearCarpeta();
    
    Logger::logg("creando la cola para las personas");
    int cola;
    if( (cola = msgget(ftok(PERSONA_FILE_IPC,COLA),IPC_CREAT|IPC_EXCL|PERMISOS)) == -1){
            Logger::loggError("Error al encontrar la cola de entrada");
            exit(1);   
    }
    
    Logger::logg("creando la cola para las respuestas de las personas");
    if( (cola = msgget(ftok(PERSONA_FILE_IPC,COLA_RESPUESTA),IPC_CREAT|IPC_EXCL|PERMISOS)) == -1){
            Logger::loggError("Error al encontrar la cola de entrada");
            exit(1);   
    }
    
    Logger::closeLogger();
    return 0;
}

