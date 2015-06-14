
#include "semaforo.h"
#include "Simulador.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

/* Funciones de semaforos
* crear el set de semaforos (si no existe)
*/

int creasem(int identif) {
    key_t clave;
    clave = ftok(DIRECTORIO_IPC, identif);
    return( semget(clave, 1, IPC_CREAT | IPC_EXCL | PERMISOS));
    /* da error si ya existe */
}

/* adquirir derecho de acceso al set de semaforos existentes
*/

int getsem(int identif){
    key_t clave;
    clave = ftok(DIRECTORIO_IPC, identif);
    return( semget(clave, 1, PERMISOS));
}
/* inicializar al semáforo del set de semaforos
*/

int inisem(int semid, int val){
    union semun {
        int val; /* Value for SETVAL */
        struct semid_ds *buf; /* Buffer for IPC_STAT, IPC_SET */
        unsigned short *array; /* Array for GETALL, SETALL */
        struct seminfo *__buf; /* Buffer for IPC_INFO(Linux specific)*/
    } arg;
    arg.val = val;
    return( semctl(semid, 0, SETVAL, arg));
}
 
/* ocupar al semáforo (p) WAIT
*/
int p(int semid){
    struct sembuf oper;
    oper.sem_num = 0;/* nro. de semáforo del set */
    oper.sem_op = -1;/* p(sem) */
    oper.sem_flg = 0;
    return (semop (semid, &oper, 1));
}

/* liberar al semáforo (v) SIGNAL
*/
int v(int semid){
    struct sembuf oper;
    oper.sem_num = 0;/* nro. de semáforo */
    oper.sem_op = 1;/* v(sem) */
    oper.sem_flg = 0;
    return (semop (semid, &oper, 1));
}

/* eliminar el set de semaforos
*/
int elisem(int semid){
    return (semctl (semid, 0, IPC_RMID, (struct semid_ds *) 0));
}
