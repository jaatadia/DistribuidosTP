/* 
 * File:   Locker.cpp
 * Author: knoppix
 *
 * Created on June 16, 2015, 12:28 PM
 */

#include <cstdlib>
#include <stdio.h>

#include "InterfazPuertaLocker.h"
#include "../Common/Logger.h"
#include "Constantes.h"

#include <map>

using namespace std;

#define LOCKER_ID "Locker"

int guardarPertenencias(std::map<int,int>* myMap, int nroPuerta, int pertenencias){
    int i=nroPuerta*1000;
    while(myMap->find(i)!=myMap->end()){
        i--;
        if(i==(nroPuerta-1)*1000){
            Logger::logg(APP_LEVEL,"No tengo mas tarjetas");
            return -1;
        }
    }
    (*myMap)[i]=pertenencias;
    char nro[20];
    sprintf(nro,"%d",i);
    Logger::logg(APP_LEVEL,string("Emitiendo tarjeta: ")+nro);
    return i;
}

int sacarPertenencias(std::map<int,int>* myMap, int tarjeta){
    char nro[20];
    sprintf(nro,"%d",tarjeta);
    Logger::logg(APP_LEVEL,string("Recibi tarjeta: ")+nro);
    
    if(myMap->find(tarjeta)!=myMap->end()){
        int result = (*myMap)[tarjeta];
        myMap->erase(tarjeta);
        return result;
    }else{
        Logger::logg(APP_LEVEL,"No pertenece esta tarjeta");
        return -1;
    }
}

int main(int argc, char** argv) {
    
    int nroPuerta = atoi(argv[1]);
    string puertaid=string(LOCKER_ID)+argv[1];
    Logger::startLog(LOGGER_DEFAULT_PATH,puertaid.c_str());
    
    std::map<int,int> myMap;
    
    InterfazPuertaLocker puerta(nroPuerta);
    Pedido pedido;
    while(true){
        Logger::logg(APP_LEVEL,"Esperando un Pedido");
        puerta.recivirPedido(pedido);
        if(pedido.tipo==TIPO_DEPOSITO){
            Logger::logg(APP_LEVEL,"Respondiendo un deposito");
            puerta.responderDeposito(guardarPertenencias(&myMap,nroPuerta,pedido.pertenenciaOTarjeta)); //TODO cambiar respuesta de tarjeta
        }else{
            Logger::logg(APP_LEVEL,"Respondiendo una extraccion");
            puerta.responderExtraccion(sacarPertenencias(&myMap,pedido.pertenenciaOTarjeta));//TODO cambiar respuesta de pertenencias
        }
    }
    
    Logger::closeLogger();
    return 0;
}

