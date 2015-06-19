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
#include "InterfazLocker.h"

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
    InterfazLocker locker(numero);
    Persona persona;
    
    while(true){
        Logger::logg(APP_LEVEL,"Tomo persona");
        entrada.tomarPersona(persona);
        Logger::logg(APP_LEVEL,"Me fijo si puede entrar al museo");
        bool pudoEntrar = museo.entrar();
        Logger::logg(APP_LEVEL,"obtuve si la persona pudo entrar");
        if (persona.tipoPersona == PERSONA) {
            entrada.responder(persona.idPersona,pudoEntrar);
            Logger::logg(APP_LEVEL,"Respondido a la persona");
        } else {
            int tarjeta=-1;
            if(pudoEntrar){
                tarjeta = locker.guardarPertenencia(1);//TODO Cambiar pertenencia
            }
            entrada.responderInvestigador(persona.idPersona,tarjeta);
            Logger::logg(APP_LEVEL,"Respondido al investigador");
        }       
    }

}
