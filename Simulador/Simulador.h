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

#define PATH_ENCOLADOR_EXEC "./Encolador"
#define NAME_ENCOLADOR_EXEC "Encolador"


#define PATH_PERSONA_EXEC "./Persona"
#define NAME_PERSONA_EXEC "Persona"

#define PATH_LOCKER_EXEC "./Locker"
#define NAME_LOCKER_EXEC "Locker"

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



#define PUERTA_FILA 106 
#define PUERTA_RESP 107 
#define PUERTA_SALIDA_FILA_ENCOLADOR 108 
#define PUERTA_SALIDA_FILA_NORMAL 109
#define PUERTA_SALIDA_FILA_PRIORITARIA 110
#define PUERTA_SALIDA_RESP 111

#define COLA_LOCKER 112
#define COLA_LOCKER_RESPUESTA_DEPOSITO 113
#define COLA_LOCKER_RESPUESTA_EXTRACCION 114

#define DESP 3
#define MUTEX_PUERTA_ESPERANDO 115 //para la puerta i es MUTEX_PUERTA_ESPERANDO + (i-1)*DESP
#define MUTEX_CONTADOR_COLAS_PUERTAS 116
#define CONTADOR_COLAS_PUERTAS 117


#define TIPO_DEPOSITO 0
#define TIPO_RETIRO 1
typedef struct{
    int tipo;
    int puerta;
    int pertenenciaOTarjeta;
} Pedido;

typedef struct {
    int personasNormales;
    int investigadores;
} ColasPuertas;


typedef struct {
    bool estaAbierto;
    int personasAdentro;
    int museoMax;
    int cantPuertas;
} Museo;


#endif	/* SIMULADOR_H */

