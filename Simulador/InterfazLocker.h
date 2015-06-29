/* 
 * File:   InterfazLocker.h
 * Author: knoppix
 *
 * Created on June 16, 2015, 1:05 PM
 */

#ifndef INTERFAZLOCKER_H
#define	INTERFAZLOCKER_H

#include "MensajeAPuerta.h"

class InterfazLocker {
public:
    InterfazLocker(int nroPuerta);
    int guardarPertenencia(int pertenencia);
    int tomarPertenencia(int tarjeta);
    virtual ~InterfazLocker();
private:
    int nroPuerta;
    int colaEntrada;
    int colaRespuestaDeposito;
    int colaRespuestaExtraccion;
};

#endif	/* INTERFAZLOCKER_H */

