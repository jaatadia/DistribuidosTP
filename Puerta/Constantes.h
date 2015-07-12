/* 
 * File:   Constantes.h
 * Author: knoppix
 *
 * Created on June 29, 2015, 3:06 PM
 */

#ifndef CONSTANTES_H
#define	CONSTANTES_H

typedef struct {
    long idPersona;
    int tipoPersona;
    int pertenencias;
} Persona;

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
    long origen;
} Pedido;



#define PUERTA_LOGGER_DEFAULT_PATH "./log.txt"
#define PUERTA_DIRECTORIO_IPC  "/home/knoppix/Atadia_Etcheverry/TPCuatrimestre"
#define PUERTA_FILE_IPC "/home/knoppix/Atadia_Etcheverry/TPCuatrimestre/puerta.txt"

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



#define COLA_PUERTA_ENTRADA 4
#define COLA_PUERTA_ENTRADA_RESPUESTA 5
#define COLA_PUERTA_SALIDA 6
#define COLA_PUERTA_SALIDA_NORMAL 7
#define COLA_PUERTA_SALIDA_PRIORITARIA 8
#define COLA_PUERTA_SALIDA_RESPUESTA 9
#define COLA_LOCKER 10
#define COLA_LOCKER_RESPUESTA 11
#define COLA_SHM 12
#define COLA_SHM_RESPUESTA 13

#define DESP 3
#define SEM_PUERTA_ESPERANDO 14
#define MUTEX_COLAS 15
#define CONTADOR_COLAS 16

#endif	/* CONSTANTES_H */

