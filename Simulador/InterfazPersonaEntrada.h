/* 
 * File:   InterfazPersonaEntrada.h
 * Author: knoppix
 *
 * Created on June 14, 2015, 4:31 PM
 */

#ifndef INTERFAZPERSONAENTRADA_H
#define	INTERFAZPERSONAENTRADA_H

class InterfazPersonaEntrada {
public:
    InterfazPersonaEntrada();
    virtual ~InterfazPersonaEntrada();
    
    //toma una persona, devuelve un struct con el id de la persona y el tipo
    struct Persona tomarPersona();
    
    //responde a la persona con el id "id", si puede pasar o no
    void responder(int idPersona,bool puedePasar);
    
    //responde al investigador. el numero de tarjeta es 0 si no puede entrar
    void responderInvestigador(int idInvestigador,int tarjeta);
    
private:

};

#endif	/* INTERFAZPERSONAENTRADA_H */

