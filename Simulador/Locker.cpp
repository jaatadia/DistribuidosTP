/* 
 * File:   Locker.cpp
 * Author: knoppix
 *
 * Created on June 16, 2015, 12:28 PM
 */

#include <cstdlib>

#include "InterfazPuertaLocker.h"
#include "Simulador.h"
#include "Logger.h"

using namespace std;

#define LOCKER_ID "Locker"
/*
 * 
 */
int main(int argc, char** argv) {
    
    int nroPuerta = atoi(argv[1]);
    string puertaid=string(LOCKER_ID)+argv[1];
    Logger::startLog(LOGGER_DEFAULT_PATH,puertaid.c_str());
    
    InterfazPuertaLocker puerta(nroPuerta);
    Pedido pedido;
    while(true){
        Logger::logg(APP_LEVEL,"Esperando un Pedido");
        puerta.recivirPedido(pedido);
        if(pedido.tipo==TIPO_DEPOSITO){
            Logger::logg(APP_LEVEL,"Respondiendo un deposito");
            puerta.responderDeposito(nroPuerta); //TODO cambiar respuesta de tarjeta
        }else{
            Logger::logg(APP_LEVEL,"Respondiendo una extraccion");
            puerta.responderExtraccion( (nroPuerta ==pedido.pertenenciaOTarjeta)? 1:-1);//TODO cambiar respuesta de pertenencias
        }
    }
    
    Logger::closeLogger();
    return 0;
}

