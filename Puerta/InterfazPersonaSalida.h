/* 
 * File:   InterfazPersonaSalida.h
 * Author: knoppix
 *
 * Created on June 14, 2015, 11:56 PM
 */

#ifndef INTERFAZPERSONASALIDA_H
#define	INTERFAZPERSONASALIDA_H

#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/shm.h>

#include "../Common/Logger.h"
#include "Constantes.h"

class InterfazPersonaSalida {
public:
    InterfazPersonaSalida(int numeroPuerta);
    virtual ~InterfazPersonaSalida();
    
    //toma una persona, devuelve un struct con el id de la persona y el tipo
    void tomarPersona(Persona& persona);
    
    //responde a la persona con el id "id", si puede pasar o no
    void responder(long idPersona,bool puedePasar);
    
    //responde al investigador. el id de pertenencias es null si no pudo salir
    void responderInvestigador(long idInvestigador,int pertenencias);
    
private:
    int colaSalida;
    int colaSalidaPrioritaria;
    int colaSalidaRespuesta;
    int semPuertaEsperando;
    int mutexColas;
    ColasPuertas* contador;
    int numero; //numero de la puerta de salida
    long myID;
};

#endif	/* INTERFAZPERSONASALIDA_H */

