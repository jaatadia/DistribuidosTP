/* 
 * File:   InterfazMuseo.h
 * Author: knoppix
 *
 * Created on June 14, 2015, 5:05 PM
 */

#ifndef INTERFAZMUSEO_H
#define	INTERFAZMUSEO_H

#include "../Common/Museo.h"

class InterfazMuseoSalida {
public:
    InterfazMuseoSalida();
    bool salir();//devuelve si pudo pasar o no
    virtual ~InterfazMuseoSalida();

private:
    int mutexMuseo,semLugar;
    Museo* myMuseum;

};

#endif	/* INTERFAZMUSEO_H */

