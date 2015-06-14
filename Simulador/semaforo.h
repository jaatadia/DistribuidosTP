/* 
 * File:   semaforo.h
 * Author: knoppix
 *
 * Created on March 26, 2015, 2:20 PM
 */

#ifndef SEMAFORO_H
#define	SEMAFORO_H
  
/*
* ADT para semaforos: semaforos.h
* definiciones de datos y funciones de semáforos
*
* Created by Maria Feldgen on 3/10/12.
* Copyright 2012 Facultad de Ingenieria U.B.A. All rights reserved.
*/

int inisem(int, int);
int getsem(int); //devuleve semid
int creasem(int); //develve semid
int p(int); //wait
int v(int);//signal
int elisem(int);

#endif	/* SEMAFORO_H */

