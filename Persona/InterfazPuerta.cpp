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
#include <sys/wait.h>
#include <stdio.h>

#include "../Common/Logger.h"
#include "Constantes.h"


using namespace std;

    InterfazPuerta::InterfazPuerta(){
        //busco la cola
        Logger::logg("Buscando la cola de entrada");
        
        if( (cola = msgget(ftok(PERSONA_FILE_IPC,COLA),PERMISOS)) == -1){
            Logger::loggError("Error al encontrar la cola de entrada");
            exit(1);   
        }

        if( (colaRespuesta = msgget(ftok(PERSONA_FILE_IPC,COLA_RESPUESTA),PERMISOS)) == -1){
            Logger::loggError("Error al encontrar la cola de respuesta");
            exit(1);   
        }
        
        
        myID=getpid();//TODO PEDIR id
        
        static char broker[255];
        static char colaEntrada[12];
        static char colaSalida[12];
        static char id[12];
        sprintf(broker,"broker");//TODO leer de un archivo
        sprintf(colaSalida,"%d",ftok(PERSONA_FILE_IPC,COLA));
        sprintf(colaEntrada,"%d",ftok(PERSONA_FILE_IPC,COLA_RESPUESTA));
        sprintf(id,"%ld",myID);
        
        int childpid;
        if ((childpid=fork())<0){
            Logger::loggError("Error al crear conectador");
            exit(1);   
        }else if (childpid == 0){
            execlp(PATH_CONECTADOR_EXEC,NAME_CONECTADOR_EXEC,broker,id,colaEntrada,colaSalida,(char*)NULL);
            Logger::loggError("Error al cargar la imagen de ejecutable del Conectador");
            exit(1);
        }
        int status;
        wait(&status);
        if(WEXITSTATUS(status)!=0){
            Logger::loggError("Error al crear las conexiones");
            exit(1);   
        }
 }

    
   void InterfazPuerta::entrar(int numeroPuerta,int tipo,int tarjeta, MensajeAPuerta& mensaje){

        stringstream ss;
        ss<<numeroPuerta;

        MensajeAPuerta msg;
        msg.myType=myID;
        msg.origen=myID;
        msg.destino=2*numeroPuerta;//TODO pedir y remplazar por el id de la puerta
        msg.mensaje=0;
        msg.tipo=tipo;
        msg.pertenenciasOTarjeta=tarjeta;
        
        Logger::logg("Enviando mensaje");
        if(msgsnd(cola,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
            Logger::loggError("Error al escribir el mensaje "+ss.str());
            exit(1);
        }

        Logger::logg("Esperando respuesta");
        if(msgrcv(colaRespuesta,&mensaje,sizeof(MensajeAPuerta)-sizeof(long),myID,0)==-1){
            Logger::loggError("Error al leer el mensaje ");
            exit(1);
        }

        if(mensaje.mensaje==MENSAJE_NO_PASAR){
            Logger::logg("El museo esta cerrado me voy");
        }
        Logger::logg("Recibi el mensaje que puedo pasar");
        
    }
    
    // devuelve si pudo entrar o no 
    bool InterfazPuerta::entrar(int numeroPuerta){
        MensajeAPuerta msg;
        entrar(numeroPuerta,PERSONA,0,msg);
        return (msg.mensaje==MENSAJE_PASAR);
    }
    
    //devuelve -1 si no pudo entrar, o la tarjeta en caso contrario
    int InterfazPuerta::entrarInvestigador(int numeroPuerta,int pertenencias){
        MensajeAPuerta msg;
        entrar(numeroPuerta,INVESTIGADOR,pertenencias,msg);
        if(msg.mensaje==MENSAJE_NO_PASAR) return -1;
        return msg.pertenenciasOTarjeta;
    }
    
    //pasea por tanto tiempo o hasta que le digan que salga
    void InterfazPuerta::pasear(int milisegundos){
        usleep(milisegundos);
    }
    

    
    void InterfazPuerta::salir(int numeroPuerta, int tipo, int tarjeta,MensajeAPuerta& msg){

        msg.myType=myID;
        msg.origen=myID;
        msg.destino=(2*numeroPuerta)-1;//TODO pedir y reemplazar por id de puerta
        msg.mensaje=0;
        msg.tipo= tipo;
        msg.pertenenciasOTarjeta=tarjeta;

        stringstream ss;
        ss<<numeroPuerta;

        Logger::logg("Enviando mensaje para salir");
        if(msgsnd(cola,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
            Logger::loggError("Error al escribir el mensaje "+ss.str());
            exit(1);
        }
        
        Logger::logg("Esperando respuesta");
        if(msgrcv(colaRespuesta,&msg,sizeof(MensajeAPuerta)-sizeof(long),myID,0)==-1){
            Logger::loggError("Error al leer el mensaje ");
            exit(1);
        }
    }


    //true pudo salir, false no
    bool InterfazPuerta::salir(int numeroPuerta){
        MensajeAPuerta msg;
        salir(numeroPuerta,PERSONA,0,msg);
        return msg.mensaje==MENSAJE_PASAR;
    }
    
    //devuelve -1 en caso de que no sea la puerta correcta o las pertenencias
    int InterfazPuerta::salirInvestigador(int numeroPuerta,int tarjeta){
        MensajeAPuerta msg;
        salir(numeroPuerta,INVESTIGADOR,tarjeta,msg);
        return (msg.mensaje==MENSAJE_PASAR) ? msg.pertenenciasOTarjeta:-1;
    }
    
    InterfazPuerta::~InterfazPuerta(){
        MensajeAPuerta msg;
        msg.myType=myID;
        msg.mensaje=MENSAJE_END_COMMUNICATION;
        Logger::logg("Enviando mensaje para cerrar comunicaciones");
        if(msgsnd(cola,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
            Logger::loggError("Error al escribir el mensaje ");
            exit(1);
        }
        
        //TODO  liberar id
    }