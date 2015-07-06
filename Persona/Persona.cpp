/* 
 * File:   persona.cpp
 * Author: knoppix
 *
 * Created on March 31, 2015, 4:50 PM
 */

#include <cstdlib>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sstream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include "../Common/Logger.h"
#include "../Common/MensajeAPuerta.h"
#include "InterfazPuerta.h"


using namespace std;

/*
 * 
 */
#define PERSONA_ID "Persona"

//argv[1] que puerta debe usar para entrar,argv[2] tiempo a dormir, argv[3] puerta para slir, argv[4] si es PERSONA o INVESTIGADOR
int main(int argc, char** argv) {
    Logger::startLog(LOGGER_DEFAULT_PATH,PERSONA_ID);
    
    if (argc != 6 ){
        Logger::loggError("No se pasaron los parametros correctos 1: numero de puerta, 2: tiempo a dormir, 3: puerta para salir, 4: tipo de persona (1:P o 2:I), 5: cant de puertas totales");
        exit(1);
    }
    srand(time(NULL));
    
    int entrada = atoi(argv[1]);
    int dormir = atoi(argv[2]);
    int salida = atoi(argv[3]);
    int tipoPersona = atoi(argv[4]);
    int cantPuertas = atoi(argv[5]);
    int pertenenciasIn = getpid(); //TODO pertenencias?? -F
    int tarjeta;

    InterfazPuerta puerta;
    
    if (tipoPersona == PERSONA) {
        Logger::logg(APP_LEVEL,"Persona solicitando entrada");
        if (!puerta.entrar(entrada)) {
            Logger::logg(APP_LEVEL,"No pude entrar. Me voy");
            return 0;
        }
    } else if (tipoPersona == INVESTIGADOR) {
        Logger::logg(APP_LEVEL,"Investigador solicitando entrada");
        tarjeta = puerta.entrarInvestigador(entrada,pertenenciasIn);
        if ( tarjeta == -1) {
            Logger::logg(APP_LEVEL,"Investigador no pude entrar. Me voy");
            return 0;
        }
    } else {
        Logger::loggError("Tipo de persona invalido");
        exit(1);
    }
    
    Logger::logg(APP_LEVEL,"Yendo a pasear");
    puerta.pasear(dormir);
    Logger::logg(APP_LEVEL,"Me desperte");
    
    if (tipoPersona == PERSONA) {
        Logger::logg(APP_LEVEL,"Persona solicitando salida");
        if (!puerta.salir(salida)) {
            Logger::loggError("Persona no puede salir");
        };
    } else if (tipoPersona == INVESTIGADOR) {
        Logger::logg(APP_LEVEL,"Investigador solicitando salida");
        int pertenencias = puerta.salirInvestigador(salida,tarjeta);
        while (pertenencias == -1) {
            Logger::logg(APP_LEVEL,"Investigador no pudo salir");
            Logger::logg(APP_LEVEL,"Investigador re-solicitando salida");
            pertenencias = puerta.salirInvestigador(rand()%cantPuertas+1,tarjeta);
        }
        if(pertenencias!=pertenenciasIn){
            Logger::logg(APP_LEVEL,"Me dieron otras pertenencias");
        }
    } else {
        Logger::loggError("Tipo de persona invalido");
        exit(1);
    }
    Logger::logg(APP_LEVEL,"Ya salí!");
    return 0;
}

