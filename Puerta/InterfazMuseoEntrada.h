/* 
 * File:   InterfazMuseo.h
 * Author: knoppix
 *
 * Created on June 14, 2015, 5:05 PM
 */

#ifndef INTERFAZMUSEO_H
#define	INTERFAZMUSEO_H

#include "../Common/Museo.h"

class InterfazMuseoEntrada {
public:
    InterfazMuseoEntrada();
    bool entrar();//devuelve si pudo pasar o no
    virtual ~InterfazMuseoEntrada();

private:
    long myId;
    int colaPeticion;
    int colaRespuesta;

};

#endif	/* INTERFAZMUSEO_H */

