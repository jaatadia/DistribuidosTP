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
        
        if( (cola = msgget(ftok(PERSONA_DIRECTORIO_IPC,COLA),PERMISOS)) == -1){
            Logger::loggError("Error al encontrar la cola de entrada");
            exit(1);   
        }

        //TODO PEDIR id
        
        static char colaEntrada[12];
        static char id[12];
        sprintf(colaEntrada,"%d",ftok(PERSONA_DIRECTORIO_IPC,COLA));
        sprintf(id,"%d",getpid());//reemplazar por el id
        
        int childpid;
        if ((childpid=fork())<0){
            Logger::loggError("Error al crear conectador");
            exit(1);   
        }else if (childpid == 0){
            execlp(PATH_CONECTADOR_EXEC,NAME_CONECTADOR_EXEC,id,colaEntrada,colaEntrada,(char*)NULL);
            Logger::loggError("Error al cargar la imagen de ejecutable del Conectador");
            exit(1);
        }
        
 }

    
   void InterfazPuerta::entrar(int numeroPuerta,int tipo,int tarjeta, MensajeAPuerta& mensaje){
       //TODO pedir id puerta
        stringstream ss;
        ss<<numeroPuerta;

        MensajeAPuerta msg;
        msg.destinatario=2*numeroPuerta;//TODO remplazar por el id de la puerta
        msg.mensaje=getpid();//TODO poner esto en otro campo
        msg.tipo=tipo;
        msg.pertenenciasOTarjeta=tarjeta;
        
        Logger::logg("Enviando mensaje");
        if(msgsnd(cola,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
            Logger::loggError("Error al escribir el mensaje "+ss.str());
            exit(1);
        }

        Logger::logg("Esperando respuesta");
        if(msgrcv(cola,&mensaje,sizeof(MensajeAPuerta)-sizeof(long),getpid(),0)==-1){//TODO poner aca el id
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
       //TODO pedir id puerta 
        msg.destinatario=(2*numeroPuerta)-1;//TODO reemplazar por numero de puerta
        msg.mensaje=getpid();//Poner esto en otro campo
        msg.tipo= tipo;
        msg.pertenenciasOTarjeta=tarjeta;

        stringstream ss;
        ss<<numeroPuerta;

        msg.mensaje=getpid();
        Logger::logg("Enviando mensaje para salir");
        if(msgsnd(cola,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
            Logger::loggError("Error al escribir el mensaje "+ss.str());
            exit(1);
        }
        
        Logger::logg("Esperando respuesta");
        if(msgrcv(cola,&msg,sizeof(MensajeAPuerta)-sizeof(long),getpid(),0)==-1){//TODO poner id aca
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
        
        //TODO enviar mensaje de cierre de comunicaciones
        //TODO  liberar id
    }