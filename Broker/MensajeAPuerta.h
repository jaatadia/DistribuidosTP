/* 
 * File:   MensajeAPuerta.h
 * Author: knoppix
 *
 * Created on June 29, 2015, 11:31 AM
 */

#ifndef MENSAJEAPUERTA_H
#define	MENSAJEAPUERTA_H

typedef struct {
    long int destinatario;
    int mensaje; //viene a quien responderle - va el mensaje
    int tipo;
    int pertenenciasOTarjeta;
} MensajeAPuerta;

#endif	/* MENSAJEAPUERTA_H */

