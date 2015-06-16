/* 
 * File:   Locker.cpp
 * Author: knoppix
 *
 * Created on June 16, 2015, 12:28 PM
 */

#include <cstdlib>

#include "InterfazPuertaLocker.h"
#include "Simulador.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    InterfazPuertaLocker puerta;
    Pedido pedido;
    while(true){
        puerta.recivirPedido(pedido);
        if(pedido.tipo==TIPO_DEPOSITO){
            puerta.responderDeposito(pedido.puerta,pedido.puerta); //TODO cambiar respuesta de tarjeta
        }else{
            puerta.responderExtraccion(pedido.puerta,(pedido.puerta==pedido.pertenenciaOTarjeta)? -1:1);//TODO cambiar respuesta de pertenencias
        }
    }
    
    return 0;
}

