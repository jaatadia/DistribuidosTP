/* 
 * File:   InterfazPuerta.h
 * Author: knoppix
 *
 * Created on June 14, 2015, 2:58 PM
 */

#ifndef INTERFAZPUERTA_H
#define	INTERFAZPUERTA_H

#include "Simulador.h"
class InterfazPuerta {
private:
    int colaEntrada;
    int colaEntradaRespuesta;
    int colaSalida;
    int colaSalidaRespuesta;
    void entrar(int numeroPuerta,int tipo,int tarjeta,MensajeAPuerta& mensaje);
    void salir(int numeroPuerta,int tipo,int pertenencias,MensajeAPuerta& mensaje);
    
public:
    
    InterfazPuerta();

    // devuelve si pudo entrar o no 
    bool entrar(int numeroPuerta);
    
    //devuelve -1 si no pudo entrar, o el numero de tarjeta en caso contrario
    int entrarInvestigador(int numeroPuerta,int pertenencias);
    
    //pasea por tanto tiempo o hasta que le digan que salga TODO
    void pasear(int milisegundos);
    
    //true pudo salir, false no
    bool salir(int numeroPuerta);
    
    //devuelve el id de las pertenencias o -1 en caso de que no sea la puerta correcta
    int salirInvestigador(int numeroPuerta,int tarjeta);
    
    virtual ~InterfazPuerta();


};

#endif	/* INTERFAZPUERTA_H */

