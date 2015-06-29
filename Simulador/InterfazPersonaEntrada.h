/* 
 * File:   InterfazPersonaEntrada.h
 * Author: knoppix
 *
 * Created on June 14, 2015, 4:31 PM
 */

#ifndef INTERFAZPERSONAENTRADA_H
#define	INTERFAZPERSONAENTRADA_H

typedef struct {
    int idPersona;
    int tipoPersona;
    int pertenencias;
} Persona;

#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#include "Logger.h"

class InterfazPersonaEntrada {
public:
    InterfazPersonaEntrada(int numeroPuerta);
    virtual ~InterfazPersonaEntrada();
    
    //toma una persona, devuelve un struct con el id de la persona y el tipo
    void tomarPersona(Persona& persona);
    
    //responde a la persona con el id "id", si puede pasar o no
    void responder(int idPersona,bool puedePasar);
    
    //responde al investigador. el numero de tarjeta es 0 si no puede entrar
    void responderInvestigador(int idInvestigador,int tarjeta);
    
private:
        int colaEntrada;
        int colaRespuesta;
        int numero; //numero de la puerta de entrada
        //MensajeAPuerta request;

};

#endif	/* INTERFAZPERSONAENTRADA_H */

