/* 
 * File:   Constantes.h
 * Author: knoppix
 *
 * Created on June 29, 2015, 3:06 PM
 */

#ifndef CONSTANTES_H
#define	CONSTANTES_H

typedef struct {
    int idPersona;
    int tipoPersona;
    int pertenencias;
} Persona;

typedef struct {
    bool estaAbierto;
    int personasAdentro;
    int museoMax;
    int cantPuertas;
} Museo;

typedef struct {
    int personasNormales;
    int investigadores;
} ColasPuertas;

#define TIPO_DEPOSITO 0
#define TIPO_RETIRO 1
typedef struct{
    int tipo;
    int puerta;
    int pertenenciaOTarjeta;
} Pedido;

#define MUSEO_CONF "./museo.cfg"
#define MUSEO_OPEN "MUSEO_OPEN"
#define MUSEO_CANT_GENTE "MUSEO_CANT"
#define MUSEO_PUERTAS "MUSEO_PUERTAS"
#define MUSEO_MAX_PERSONAS "MUSEO_MAX_PERSONAS"
#define CANT_PERSONAS "CANT_PERSONAS"

#define PUERTA_LOGGER_DEFAULT_PATH "./log.txt"
#define PUERTA_DIRECTORIO_IPC  "/home/knoppix/Atadia_Etcheverry/Simulador"
#define PUERTA_FILE_IPC "/home/knoppix/Atadia_Etcheverry/Simulador/puerta.txt"

#define PATH_CONECTADOR_EXEC "../Common/Conectador"
#define NAME_CONECTADOR_EXEC "Conectador"

#define PATH_ENCOLADOR_EXEC "./Encolador"
#define NAME_ENCOLADOR_EXEC "Encolador"

#define PATH_PUERTA_EXEC "./Puerta"
#define NAME_PUERTA_EXEC "Puerta"

#define PATH_PUERTA_SALIDA_EXEC "./PuertaSalida"
#define NAME_PUERTA_SALIDA_EXEC "PuertaSalida"

#define PATH_LOCKER_EXEC "./Locker"
#define NAME_LOCKER_EXEC "Locker"

#define PERMISOS 0660

#define MUSEO 1
#define MUTEX_MUSEO 2
#define SEM_LUGAR 3

#define COLA_PUERTA_ENTRADA 4
#define COLA_PUERTA_ENTRADA_RESPUESTA 5
#define COLA_PUERTA_SALIDA 6
#define COLA_PUERTA_SALIDA_NORMAL 7
#define COLA_PUERTA_SALIDA_PRIORITARIA 8
#define COLA_PUERTA_SALIDA_RESPUESTA 9
#define COLA_LOCKER 10
#define COLA_LOCKER_RESPUESTA 11 //TODO MATAR

#define DESP 3
#define SEM_PUERTA_ESPERANDO 12
#define MUTEX_COLAS 13
#define CONTADOR_COLAS 14

#endif	/* CONSTANTES_H */

