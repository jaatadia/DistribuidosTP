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
#include "InterfazLocker.h"

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
    
    InterfazPersonaSalida persona(numero);
    InterfazMuseoSalida museo;
    InterfazLocker locker(numero);
    Persona datos;
    while(true){
        persona.tomarPersona(datos);
        if(datos.tipoPersona==INVESTIGADOR){
            int resultado = locker.guardarPertenencia(datos.pertenencias);
            if(resultado!=-1) museo.salir();
            persona.responderInvestigador(datos.idPersona,resultado);
        }else{
            persona.responder(datos.idPersona,museo.salir());
        }
        
    }

}
