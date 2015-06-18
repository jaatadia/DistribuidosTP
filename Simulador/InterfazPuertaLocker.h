/* 
 * File:   InterfazPuertaLocker.h
 * Author: knoppix
 *
 * Created on June 16, 2015, 12:30 PM
 */

#ifndef INTERFAZPUERTALOCKER_H
#define	INTERFAZPUERTALOCKER_H

#include "Simulador.h"

class InterfazPuertaLocker {
public:
    InterfazPuertaLocker(int nroPuerta);
    void recivirPedido(Pedido& pedido);
    void responderDeposito(int tarjeta);
    void responderExtraccion(int pertenencia);
    
    virtual ~InterfazPuertaLocker();
private:
    int nroPuerta;
    int colaEntrada;
    int colaRespuestaDeposito;
    int colaRespuestaExtraccion;
};

#endif	/* INTERFAZPUERTALOCKER_H */

