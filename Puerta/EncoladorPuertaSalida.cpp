/* 
 * File:   EncoladorPuertaSalida.cpp
 * Author: knoppix
 *
 * Created on June 28, 2015, 1:15 PM
 */

#include <cstdlib>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/types.h>


#include "../Common/Logger.h"
#include "../Common/semaforo.h"
#include "../Common/MensajeAPuerta.h"

#include "Constantes.h"

#define ENCOLADOR_PUERTA_ID "EncoladorPuerta"

using namespace std;

/*
 * 
 */
class EncoladorPuertaSalida{
    private:
        int colaSalida,colaSalidaNormal,colaSalidaPrioritaria;
        int SemaforoPuertaEsperando, mutexColas;
        int nroPuerta;
        ColasPuertas* contador;
        
    public:
        EncoladorPuertaSalida(int nroPuerta){
            this->nroPuerta=nroPuerta;
            Logger::logg("Buscando la cola de personas por salir");
            if( (colaSalida = msgget(ftok(PUERTA_FILE_IPC,COLA_PUERTA_SALIDA),PERMISOS)) == -1){
                Logger::loggError("Error al encontrar la cola de entrada");
                exit(1);   
            }
    
            Logger::logg("Buscando la cola de personas normales");
            if( (colaSalidaNormal = msgget(ftok(PUERTA_FILE_IPC,COLA_PUERTA_SALIDA_NORMAL),PERMISOS)) == -1){
                Logger::loggError("Error al encontrar la cola de personasNormales");
                exit(1);   
            }    

            Logger::logg("Buscando la cola de personas prioritarias");
            if( (colaSalidaPrioritaria = msgget(ftok(PUERTA_FILE_IPC,COLA_PUERTA_SALIDA_PRIORITARIA),PERMISOS)) == -1){
                Logger::loggError("Error al encontrar la cola de personasNormales");
                exit(1);   
            }
            
            SemaforoPuertaEsperando = getsem(PUERTA_FILE_IPC,SEM_PUERTA_ESPERANDO+(nroPuerta-1)*DESP,PERMISOS);
            mutexColas = getsem(PUERTA_FILE_IPC,MUTEX_COLAS+(nroPuerta-1)*DESP,PERMISOS);
            
            Logger::logg("buscando la memoria compartida para la puerta");
            int shmid;
            if( (shmid = shmget(ftok(PUERTA_FILE_IPC,CONTADOR_COLAS+(nroPuerta-1)*DESP), sizeof(ColasPuertas),PERMISOS)) == -1 ){
                Logger::loggError("Error al obtener la memoria compartida");
                exit(1);   
            }

            Logger::logg("Uniendose a la memoria compartida");
            if ( (contador = (ColasPuertas*) shmat(shmid,NULL,0)) == (ColasPuertas*) -1 ){
                Logger::loggError("Error al atachearse a la memoria compartida");
                exit(1);   
            }
        }
    
    void tratar(){
        MensajeAPuerta msg;
        Logger::logg("EsperandoMensaje");
        if(msgrcv(colaSalida,&msg,sizeof(MensajeAPuerta)-sizeof(long),nroPuerta,0)==-1){
            Logger::loggError("Error al leer el mensaje ");
            exit(1);
        }
        
        if(msg.tipo==PERSONA){
            aumentarEsperando(true);
            Logger::logg("reenviando mensaje por cola normal");
            if(msgsnd(colaSalidaNormal,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
                Logger::loggError("Error al escribir el mensaje ");
                exit(1);
            }
        }else{
            aumentarEsperando(false);
            Logger::logg("reenviando mensaje por cola prioritaria");
            if(msgsnd(colaSalidaPrioritaria,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
                Logger::loggError("Error al escribir el mensaje ");
                exit(1);
            }
        }
    }
    void aumentarEsperando(bool normal){

        //obtiene mutex
        Logger::logg("Espero mutex para contador de personas en la cola");
        if(p(mutexColas)==-1){
            Logger::loggError("Error al obtener el mutex de el contador de las colas");
            exit(1);   
        }

        if((contador->personasNormales==0)&&(contador->investigadores==0)){
            if(v(SemaforoPuertaEsperando)==-1){
                Logger::loggError("Error al liberar la puerta de la espera");
                exit(1);   
            }
        }

        if(normal){
            contador->personasNormales=contador->personasNormales+1;
        }else{
            contador->investigadores=contador->investigadores+1;
        }
        Logger::logg("Liberado mutex de que hay personas y aumentada la cantidad en la cola");

        if(v(mutexColas)==-1){
            Logger::loggError("Error al liberar la puerta de la espera");
            exit(1);   
        }
        Logger::logg("Liberado mutex de colas");
    }    
        
    ~EncoladorPuertaSalida(){    
        Logger::logg("Desuniendose de la memoria compartida");
        if(shmdt(contador)==-1){
            Logger::loggError("Error al desatachearse de la memoria compartida");
            exit(1);   
        }       
    }
};

int main(int argc, char** argv) {

    if (argc != 2 ){
        Logger::startLog(LOGGER_DEFAULT_PATH,ENCOLADOR_PUERTA_ID);
        Logger::loggError("No se pasaron los parametros correctos 1: id de puerta");
        exit(1);
    }
    
    int numero = atoi(argv[1]);
    string puertaid=string(ENCOLADOR_PUERTA_ID)+argv[1];
    Logger::startLog(LOGGER_DEFAULT_PATH,puertaid.c_str());
    EncoladorPuertaSalida encolador(numero);
    
    while(true){
        encolador.tratar();
    }
    
    
    return 0;
}