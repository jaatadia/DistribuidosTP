/* 
 * File:   AdminInitializer.cpp
 * Author: knoppix
 *
 * Created on July 12, 2015, 11:34 PM
 */


#include <cstdlib>
#include <string>
#include <sys/types.h>
#include <sys/msg.h>
#include  "Constantes.h"
#include "../Common/Logger.h"

using namespace std;


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
}
int main(int argc, char** argv) {
    crearCarpetas();
    crearColas();
    return 0;
}

