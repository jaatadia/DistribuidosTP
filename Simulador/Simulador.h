/* 
 * File:   Simulador.h
 * Author: knoppix
 *
 * Created on March 26, 2015, 1:53 PM
 */

#ifndef SIMULADOR_H
#define	SIMULADOR_H

#define MUSEO_CONF "./museo.cfg"
#define MUSEO_OPEN "MUSEO_OPEN"
#define MUSEO_CANT_GENTE "MUSEO_CANT"
#define MUSEO_PUERTAS "MUSEO_PUERTAS"
#define MUSEO_MAX_PERSONAS "MUSEO_MAX_PERSONAS"
#define CANT_PERSONAS "CANT_PERSONAS"

#define DIRECTORIO_IPC "/home/knoppix/Atadia/Simulador"

#define LOGGER_DEFAULT_PATH  "./log.txt"

#define PATH_PUERTA_EXEC "./Puerta"
#define NAME_PUERTA_EXEC "Puerta"

#define PATH_PUERTA_SALIDA_EXEC "./PuertaSalida"
#define NAME_PUERTA_SALIDA_EXEC "PuertaSalida"

#define PATH_PERSONA_EXEC "./Persona"
#define NAME_PERSONA_EXEC "Persona"

#define PATH_WAKER_EXEC "./Waker"
#define NAME_WAKER_EXEC "Waker"

#define PATH_DEST_EXEC "./Destroyer"
#define NAME_DEST_EXEC "Destroyer"

#define PERMISOS 0660


#define MUSEO 100
#define MUTEX_ESTADO 101
#define MUTEX_LUGAR 102
#define MUTEX_CONTADOR 103
#define COLA_MATAR_PUERTAS 104
#define COLA_MATAR_PUERTAS_STR "104"
#define COLA_MATAR_PERSONAS 105
#define COLA_MATAR_PERSONAS_STR "105"

#define DESP 4

#define PUERTA_FILA 106 //las sucesivas puertas van a tener PUERTA_FILA + DESP * nro_puerta
#define PUERTA_RESP 107 //las sucesivas puertas van a tener PUERTA_RESP + DESP * nro_puerta
#define PUERTA_SALIDA_FILA 108 //las sucesivas puertas van a tener PUERTA_SALIDA_FILA + DESP * nro_puerta
#define PUERTA_SALIDA_RESP 109 //las sucesivas puertas van a tener PUERTA_SALIDA_RESP + DESP * nro_puerta

typedef struct {
    bool estaAbierto;
    int personasAdentro;
    int museoMax;
    int cantPuertas;
} Museo;


#define MENSAJE_NO_PASAR 0
#define MENSAJE_PASAR 1
typedef struct {
    long int destinatario;
    int mensaje;
} Mensaje;


#endif	/* SIMULADOR_H */

