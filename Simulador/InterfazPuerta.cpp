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

#include <sstream>

#include <unistd.h>
#include <signal.h>
#include "Logger.h"
#include "Simulador.h"
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

        Logger::logg("Buscando la cola de respuesta");
        if( (colaSalidaRespuesta = msgget(ftok(DIRECTORIO_IPC,PUERTA_SALIDA_RESP),PERMISOS)) == -1){
            Logger::loggError("Error al encontrar la cola de respuesta");
            exit(1);   
        }
 }

    
    MensajeAPuerta InterfazPuerta::entrar(int numeroPuerta,int tipo,int tarjeta){

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
        if(msgrcv(colaEntradaRespuesta,&msg,sizeof(MensajeAPuerta)-sizeof(long),getpid(),0)==-1){
            Logger::loggError("Error al leer el mensaje ");
            exit(1);
        }

        if(msg.mensaje==MENSAJE_NO_PASAR){
            Logger::logg("El museo esta cerrado me voy");
        }


        return (msg.mensaje==MENSAJE_PASAR);
        
    }
    
    // devuelve si pudo entrar o no 
    bool InterfazPuerta::entrar(int numeroPuerta){
        MensajeAPuerta msg = entrar(numeroPuerta,TIPO_NORMAL,0);
        return (msg.mensaje==MENSAJE_PASAR);
    }
    
    //devuelve -1 si no pudo entrar, o la tarjeta en caso contrario
    int InterfazPuerta::entrarInvestigador(int numeroPuerta,int pertenencias){
        MensajeAPuerta msg = entrar(numeroPuerta,TIPO_INVESTIGADOR,pertenencias);
        return msg.pertenenciasOTarjeta;
    }
    
    //pasea por tanto tiempo o hasta que le digan que salga
    void InterfazPuerta::pasear(int milisegundos){
        //TODO
        usleep(milisegundos);
    }
    
    
    MensajeAPuerta InterfazPuerta::salir(int numeroPuerta,int tipo,int pertenencias){
        MensajeAPuerta msg;
        msg.destinatario=numeroPuerta;
        msg.mensaje=getpid();
        msg.tipo=tipo;
        msg.pertenenciasOTarjeta=pertenencias;

        stringstream ss;
        ss<<numeroPuerta;

        msg.mensaje=getpid();
        Logger::logg("Enviando mensaje para salir");
        if(msgsnd(colaSalida,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
            Logger::loggError("Error al escribir el mensaje "+ss.str());
            exit(1);
        }

        Logger::logg("Esperando respuesta");
        if(msgrcv(colaSalidaRespuesta,&msg,sizeof(MensajeAPuerta)-sizeof(long),getpid(),0)==-1){
            Logger::loggError("Error al leer el mensaje ");
            exit(1);
        }

        Logger::logg("Sali del museo");

        return false;
    }
    
    
    //true pudo salir, false no
    bool InterfazPuerta::salir(int numeroPuerta){
        MensajeAPuerta msg = salir(numeroPuerta,TIPO_NORMAL,0);
        return (msg.mensaje==MENSAJE_PASAR);
    }
    
    //devuelve -1 en caso de que no sea la puerta correcta o las pertenencias
    int InterfazPuerta::salirInvestigador(int numeroPuerta,int tarjeta){
        MensajeAPuerta msg = entrar(numeroPuerta,TIPO_NORMAL,0);
        return msg.pertenenciasOTarjeta;
    }
    
    InterfazPuerta::~InterfazPuerta(){
        
    }

