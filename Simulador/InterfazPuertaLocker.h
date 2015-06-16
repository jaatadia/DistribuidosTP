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
    InterfazPuertaLocker();
    void recivirPedido(Pedido& pedido);
    void responderDeposito(int puerta, int tarjeta);
    void responderExtraccion(int puerta, int pertenencia);
    
    virtual ~InterfazPuertaLocker();
private:
    int colaEntrada;
    int colaRespuestaDeposito;
    int colaRespuestaExtraccion;
};

#endif	/* INTERFAZPUERTALOCKER_H */

