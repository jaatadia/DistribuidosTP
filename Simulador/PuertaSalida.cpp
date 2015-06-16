/* 
 * File:   Puerta.cpp
 * Author: knoppix
 *
 * Created on March 26, 2015, 2:00 PM
 */

#include <stdlib.h>
#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sstream>

#include "Logger.h"
#include "Simulador.h"
#include "InterfazPersonaSalida.h"
#include "InterfazMuseoSalida.h"
#include "semaforo.h"

using namespace std;


#define PUERTA_SALIDA_ID "PuertaSalida"

//en argv[1] el numero de puerta
int main(int argc, char** argv) {
    
    if (argc != 2 ){
        Logger::startLog(LOGGER_DEFAULT_PATH,PUERTA_SALIDA_ID);
        Logger::loggError("No se pasaron los parametros correctos 1: numero de puerta");
        exit(1);
    }
    
    int numero = atoi(argv[1]);
    string puertaid=string(PUERTA_SALIDA_ID)+argv[1];
    Logger::startLog(LOGGER_DEFAULT_PATH,puertaid.c_str());
    
   /*
    //busco las colas
    Logger::logg("Buscando la cola de entrada");
    int colaEntrada,colaRespuesta;
    if( (colaEntrada = msgget(ftok(DIRECTORIO_IPC,PUERTA_SALIDA_FILA),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de entrada");
        exit(1);   
    }

    Logger::logg("Buscando la cola de respuesta");
    if( (colaRespuesta = msgget(ftok(DIRECTORIO_IPC,PUERTA_SALIDA_RESP),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de respuesta");
        exit(1);   
    }
    * */
  
    InterfazMuseoSalida museo;
    while(true){
        /*
        Logger::logg("Esperando persona");
        MensajeAPuerta msg;
        if( (msgrcv(colaEntrada,&msg,sizeof(MensajeAPuerta)-sizeof(long),numero,0)) == -1){
            Logger::loggError("Error al leer mensaje de entrada");
            exit(1);   
        }
        */
        bool resultado = museo.salir();
        /*
        msg.destinatario  = msg.mensaje;
        msg.mensaje = MENSAJE_PASAR;
        if(msgsnd(colaRespuesta,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
            Logger::loggError("Error responder a la persona");
            exit(1);   
        }
        */
        
    }

}
