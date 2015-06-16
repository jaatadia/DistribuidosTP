/* 
 * File:   InterfazPersonaSalida.h
 * Author: knoppix
 *
 * Created on June 14, 2015, 11:56 PM
 */

#ifndef INTERFAZPERSONASALIDA_H
#define	INTERFAZPERSONASALIDA_H

#define PERSONA 1
#define INVESTIGADOR 2
typedef struct {
    int idPersona;
    int tipoPersona;
} Persona; //TODO esta repetido en entrada

#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include "Simulador.h"
#include "Logger.h"
#include "semaforo.h"

class InterfazPersonaSalida {
public:
    InterfazPersonaSalida();
    virtual ~InterfazPersonaSalida();
    
    //toma una persona, devuelve un struct con el id de la persona y el tipo
    void tomarPersona(Persona& persona);
    
    //responde a la persona con el id "id", si puede pasar o no
    void responder(int idPersona,bool puedePasar);
    
    //responde al investigador. el id de pertenencias es null si no pudo salir
    void responderInvestigador(int idInvestigador,int pertenencias);
    
private:
    int colaEntrada;
    int colaRespuesta;
    int numero; //numero de la puerta de salida
    //MensajeAPuerta request;
    void disminuirEsperando(bool normal,int nroPuerta);
};

#endif	/* INTERFAZPERSONASALIDA_H */

