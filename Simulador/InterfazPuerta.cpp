/* 
 * File:   InterfazPuerta.cpp
 * Author: knoppix
 * 
 * Created on June 14, 2015, 2:58 PM
 */

#include "InterfazPuerta.h"

#include <cstdlib>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/shm.h>

#include <sstream>

#include <unistd.h>
#include <signal.h>
#include "Logger.h"
#include "Simulador.h"
#include "semaforo.h"
#include <sys/wait.h>

using namespace std;

    InterfazPuerta::InterfazPuerta(){
        //busco las colas
        Logger::logg("Buscando la cola de entrada");
        
        if( (colaEntrada = msgget(ftok(DIRECTORIO_IPC,PUERTA_FILA),PERMISOS)) == -1){
            Logger::loggError("Error al encontrar la cola de entrada");
            exit(1);   
        }

        Logger::logg("Buscando la cola de respuesta");
        if( (colaEntradaRespuesta = msgget(ftok(DIRECTORIO_IPC,PUERTA_RESP),PERMISOS)) == -1){
            Logger::loggError("Error al encontrar la cola de respuesta");
            exit(1);   
        }
        
        Logger::logg("Buscando la cola de salida");
        if( (colaSalida = msgget(ftok(DIRECTORIO_IPC,PUERTA_SALIDA_FILA),PERMISOS)) == -1){
            Logger::loggError("Error al encontrar la cola de entrada");
            exit(1);   
        }
        
        Logger::logg("Buscando la cola de salida prioritaria");
        if( (colaSalidaPrioritaria = msgget(ftok(DIRECTORIO_IPC,PUERTA_SALIDA_FILA_PRIORITARIA),PERMISOS)) == -1){
            Logger::loggError("Error al encontrar la cola de salida prioritaria");
            exit(1);   
        }

        Logger::logg("Buscando la cola de respuesta");
        if( (colaSalidaRespuesta = msgget(ftok(DIRECTORIO_IPC,PUERTA_SALIDA_RESP),PERMISOS)) == -1){
            Logger::loggError("Error al encontrar la cola de respuesta");
            exit(1);   
        }
        
 }

    
   void InterfazPuerta::entrar(int numeroPuerta,int tipo,int tarjeta, MensajeAPuerta& mensaje){ //TODO no usa el tipo ni la tarjeta

        stringstream ss;
        ss<<numeroPuerta;

        MensajeAPuerta msg;
        msg.destinatario=1;
        msg.mensaje=getpid();

        Logger::logg("Enviando mensaje");
        if(msgsnd(colaEntrada,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
            Logger::loggError("Error al escribir el mensaje "+ss.str());
            exit(1);
        }

        Logger::logg("Esperando respuesta");
        if(msgrcv(colaEntradaRespuesta,&mensaje,sizeof(MensajeAPuerta)-sizeof(long),getpid(),0)==-1){
            Logger::loggError("Error al leer el mensaje ");
            exit(1);
        }

        if(mensaje.mensaje==MENSAJE_NO_PASAR){
            Logger::logg("El museo esta cerrado me voy");
        }
        
    }
    
    // devuelve si pudo entrar o no 
    bool InterfazPuerta::entrar(int numeroPuerta){
        MensajeAPuerta msg;
        entrar(numeroPuerta,TIPO_NORMAL,0,msg);
        return (msg.mensaje==MENSAJE_PASAR);
    }
    
    //devuelve -1 si no pudo entrar, o la tarjeta en caso contrario
    int InterfazPuerta::entrarInvestigador(int numeroPuerta,int pertenencias){
        MensajeAPuerta msg;
        entrar(numeroPuerta,TIPO_INVESTIGADOR,pertenencias,msg);
        return msg.pertenenciasOTarjeta;
    }
    
    //pasea por tanto tiempo o hasta que le digan que salga
    void InterfazPuerta::pasear(int milisegundos){
        //TODO
        usleep(milisegundos);
    }
    
    //true pudo salir, false no
    bool InterfazPuerta::salir(int numeroPuerta){
        MensajeAPuerta msg;
        msg.destinatario=numeroPuerta;
        msg.mensaje=getpid();
        msg.tipo=TIPO_NORMAL;

        stringstream ss;
        ss<<numeroPuerta;

        msg.mensaje=getpid();
        Logger::logg("Enviando mensaje para salir");
        if(msgsnd(colaSalida,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
            Logger::loggError("Error al escribir el mensaje "+ss.str());
            exit(1);
        }

        aumentarEsperando(true,numeroPuerta);
        
        Logger::logg("Esperando respuesta");
        if(msgrcv(colaSalidaRespuesta,&msg,sizeof(MensajeAPuerta)-sizeof(long),getpid(),0)==-1){
            Logger::loggError("Error al leer el mensaje ");
            exit(1);
        }
        
        return msg.mensaje==MENSAJE_PASAR;
    }
    
    //devuelve -1 en caso de que no sea la puerta correcta o las pertenencias
    int InterfazPuerta::salirInvestigador(int numeroPuerta,int tarjeta){
        
        MensajeAPuerta msg;
        msg.destinatario=numeroPuerta;
        msg.mensaje=getpid();
        msg.tipo=TIPO_NORMAL;
        msg.pertenenciasOTarjeta=tarjeta;

        stringstream ss;
        ss<<numeroPuerta;

        msg.mensaje=getpid();
        Logger::logg("Enviando mensaje para salir");
        if(msgsnd(colaSalidaPrioritaria,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
            Logger::loggError("Error al escribir el mensaje "+ss.str());
            exit(1);
        }
        
        aumentarEsperando(false,numeroPuerta);
        
        Logger::logg("Esperando respuesta");
        if(msgrcv(colaSalidaRespuesta,&msg,sizeof(MensajeAPuerta)-sizeof(long),getpid(),0)==-1){
            Logger::loggError("Error al leer el mensaje ");
            exit(1);
        }
        
        return (msg.mensaje==MENSAJE_PASAR) ? msg.pertenenciasOTarjeta:-1;
    }
    
    InterfazPuerta::~InterfazPuerta(){
        
    }

    void InterfazPuerta::aumentarEsperando(bool normal,int nroPuerta){
        int puertaEsperando = getsem(MUTEX_PUERTA_ESPERANDO+(nroPuerta-1)*DESP);
        int colaNormal = getsem(MUTEX_CONTADOR_FILA_NORMAL+(nroPuerta-1)*DESP);
        int colaPrioritaria = getsem(MUTEX_CONTADOR_FILA_PRIORITARIA+(nroPuerta-1)*DESP);
        
        Logger::logg("buscando la memoria compartida para la puerta");
        int shmid;
        if( (shmid = shmget(ftok(DIRECTORIO_IPC,CONTADOR_FILA_NORMAL+(nroPuerta-1)*DESP), sizeof(int),PERMISOS)) == -1 ){
            Logger::loggError("Error al obtener la memoria compartida");
            exit(1);   
        }

        Logger::logg("Uniendose a la memoria compartida");
        int* contadorNormal;
        if ( (contadorNormal = (int*) shmat(shmid,NULL,0)) == (int*) -1 ){
            Logger::loggError("Error al atachearse a la memoria compartida");
            exit(1);   
        }

        Logger::logg("buscando la memoria compartida para la puerta");
        if( (shmid = shmget(ftok(DIRECTORIO_IPC,CONTADOR_FILA_PRIORITARIA+(nroPuerta-1)*DESP), sizeof(int),PERMISOS)) == -1 ){
            Logger::loggError("Error al obtener la memoria compartida");
            exit(1);   
        }

        Logger::logg("Uniendose a la memoria compartida");
        int* contadorPrioritario;
        if ( (contadorPrioritario = (int*) shmat(shmid,NULL,0)) == (int*) -1 ){
            Logger::loggError("Error al atachearse a la memoria compartida");
            exit(1);   
        }

        if(p(colaNormal)==-1){
            Logger::loggError("Error al obtener el mutex de la cola normal");
            exit(1);   
        }
        
        if(p(colaPrioritaria)==-1){
            Logger::loggError("Error al obtener el mutex de la cola prioritaria");
            exit(1);   
        }
        if((*contadorNormal==0)&&(*contadorPrioritario==0)){
            if(v(puertaEsperando)==-1){
                Logger::loggError("Error al liberar la puerta de la espera");
                exit(1);   
            }
        }
        
        if(normal){
            *contadorNormal=*contadorNormal+1;
        }else{
            *contadorPrioritario=*contadorPrioritario+1;
        }
        
        Logger::logg("Desuniendose de la memoria compartida");
        if(shmdt(contadorNormal)==-1){
            Logger::loggError("Error al desatachearse de la memoria compartida");
            exit(1);   
        }    
        
        Logger::logg("Desuniendose de la memoria compartida");
        if(shmdt(contadorPrioritario)==-1){
            Logger::loggError("Error al desatachearse de la memoria compartida");
            exit(1);   
        }
        
    }
