/* 
 * File:   InterfazPuertaLocker.h
 * Author: knoppix
 *
 * Created on June 16, 2015, 12:30 PM
 */

#ifndef INTERFAZPUERTALOCKER_H
#define	INTERFAZPUERTALOCKER_H

#include "Constantes.h"

class InterfazPuertaLocker {
public:
    InterfazPuertaLocker(int nroPuerta);
    void recivirPedido(Pedido& pedido);
    void responder(long idDestino, int tipoOperacion,int tarjeta);
    
    virtual ~InterfazPuertaLocker();
private:
    int nroPuerta;
    int colaEntrada;
    int colaRespuesta;
};

#endif	/* INTERFAZPUERTALOCKER_H */

