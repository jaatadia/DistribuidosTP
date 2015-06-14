/* 
 * File:   Puerta.cpp
 * Author: knoppix
 *
 * Created on March 26, 2015, 2:00 PM
 */

#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sstream>

#include "Logger.h"
#include "Simulador.h"
#include "InterfazMuseoEntrada.h"
#include "InterfazPersonaEntrada.h"

using namespace std;


#define PUERTA_ID "Puerta"

//en argv[1] el numero de puerta
int main(int argc, char** argv) {
    
    if (argc != 2 ){
        Logger::startLog(LOGGER_DEFAULT_PATH,PUERTA_ID);
        Logger::loggError("No se pasaron los parametros correctos 1: numero de puerta");
        exit(1);
    }
    
    int numero = atoi(argv[1]);
    string puertaid=string(PUERTA_ID)+argv[1];
    Logger::startLog(LOGGER_DEFAULT_PATH,puertaid.c_str());
    
    InterfazMuseoEntrada museo;
    InterfazPersonaEntrada entrada(numero);
  
    while(true){
        Persona persona;
        
        entrada.tomarPersona(persona);
        bool pudoEntrar = museo.entrar();
        if (persona.tipoPersona == PERSONA) {
            entrada.responder(persona.idPersona,pudoEntrar);
        } else {
            entrada.responderInvestigador(persona.idPersona,0); //TODO numero de tarjeta
        }       
    }

}
