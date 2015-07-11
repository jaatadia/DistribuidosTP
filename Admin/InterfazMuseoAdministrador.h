/* 
 * File:   InterfazMuseoAdministrador.h
 * Author: knoppix
 *
 * Created on June 16, 2015, 11:38 AM
 */

#ifndef INTERFAZMUSEOADMINISTRADOR_H
#define	INTERFAZMUSEOADMINISTRADOR_H

#include "../Common/Museo.h"

class InterfazMuseoAdministrador {
public:
    InterfazMuseoAdministrador();
    void abrir();
    void cerrar();
    virtual ~InterfazMuseoAdministrador();
private:
    int mutexEstado;
    int mutexPersonas;
    Museo* myMuseum;
};

#endif	/* INTERFAZMUSEOADMINISTRADOR_H */

