/* 
 * File:   Museo.h
 * Author: knoppix
 *
 * Created on July 4, 2015, 12:13 PM
 */

#ifndef MUSEO_H
#define	MUSEO_H


#define MUSEO_DIRECTORIO_IPC  "/home/knoppix/Atadia_Etcheverry/TPCuatrimestre"
#define MUSEO_FILE_IPC "/home/knoppix/Atadia_Etcheverry/TPCuatrimestre/museo.txt"

#define MUSEO 1
#define MUTEX_MUSEO 1
#define SEM_LUGAR 2

#define PETICION 1
#define DEVOLUCION 2

typedef struct {
    long myType;
    long origen;
    long destino;
    bool estaAbierto;
    int personasAdentro;
    int museoMax;
    //int cantPuertas;
} Museo;


#endif	/* MUSEO_H */

