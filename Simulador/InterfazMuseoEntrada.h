/* 
 * File:   InterfazMuseo.h
 * Author: knoppix
 *
 * Created on June 14, 2015, 5:05 PM
 */

#ifndef INTERFAZMUSEO_H
#define	INTERFAZMUSEO_H

#include "Simulador.h"

class InterfazMuseoEntrada {
public:
    InterfazMuseoEntrada();
    bool entrar();//devuelve si pudo pasar o no
    virtual ~InterfazMuseoEntrada();

private:
    int mutexEstado,mutexLugar,mutexPersonas;
    Museo* myMuseum;

};

#endif	/* INTERFAZMUSEO_H */

