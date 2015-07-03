/* 
 * File:   InterfazPersonaEntrada.cpp
 * Author: knoppix
 * 
 * Created on June 14, 2015, 4:31 PM
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#include "../Common/MensajeAPuerta.h"
#include "../Common/Logger.h"

#include "InterfazPersonaEntrada.h"
#include "Constantes.h"

InterfazPersonaEntrada::InterfazPersonaEntrada(int numeroPuerta) {
    
    this->numero=numeroPuerta;
    
    //busco las colas
    Logger::logg("Buscando la cola de entrada");
    if( (colaEntrada = msgget(ftok(PUERTA_FILE_IPC,COLA_PUERTA_ENTRADA),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de entrada");
        exit(1);   
    }

    Logger::logg("Buscando la cola de respuesta");
    if( (colaRespuesta = msgget(ftok(PUERTA_FILE_IPC,COLA_PUERTA_ENTRADA_RESPUESTA),PERMISOS)) == -1){
        Logger::loggError("Error al encontrar la cola de respuesta");
        exit(1);   
    }
    
    //TODO pedir id
    myID=numeroPuerta*2;
    
    static char broker[18];
    static char id[18];
    static char colaEntrada[18];
    static char colaSalida[18];
    sprintf(broker,"broker");//TODO leer de un archivo
    sprintf(id,"%ld",myID);
    sprintf(colaEntrada,"%d",ftok(PUERTA_FILE_IPC,COLA_PUERTA_ENTRADA));
    sprintf(colaSalida,"%d",ftok(PUERTA_FILE_IPC,COLA_PUERTA_ENTRADA_RESPUESTA));
    
    int childpid;
    if ((childpid=fork())<0){
        Logger::loggError("Error al crear el conectador ");
        exit(1);   
    }else if (childpid == 0){
        execlp(PATH_CONECTADOR_EXEC,NAME_CONECTADOR_EXEC,broker,id,colaEntrada,colaSalida,(char*)NULL);
        Logger::loggError("Error al cargar la imagen de ejecutable del CSCliente");
        exit(1);
    }
    
    Logger::logg("Esperando que las conexiones se realizen");
    int status;
    wait(&status);
    if(WEXITSTATUS(status)!=0){
        Logger::loggError("Error al crear las conexiones");
        exit(1);   
    }
}

InterfazPersonaEntrada::~InterfazPersonaEntrada() {    
}

void InterfazPersonaEntrada::tomarPersona(Persona& persona){
    
    Logger::logg("Esperando persona");
    MensajeAPuerta msg;
    if( (msgrcv(colaEntrada,&msg,sizeof(MensajeAPuerta)-sizeof(long),myID,0)) == -1){
        Logger::loggError("Error leer mensaje de entrada");
        exit(1);   
    }
    //request = msg; 
    
    persona.idPersona = msg.origen;
    persona.tipoPersona = msg.tipo;
    persona.pertenencias = msg.pertenenciasOTarjeta;
    
    Logger::logg("Recibi persona");
}
    
void InterfazPersonaEntrada::responder(long idPersona,bool puedePasar){
    
    MensajeAPuerta msg;
    msg.myType = myID;
    msg.origen = myID;
    msg.destino = idPersona;
    msg.mensaje = (puedePasar) ? MENSAJE_PASAR : MENSAJE_NO_PASAR;
    Logger::logg("Respondiendo a persona si puede pasar");
    if(msgsnd(colaRespuesta,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
        Logger::loggError("Error responder a la persona");
        exit(1);   
    }
    (puedePasar) ? Logger::logg("Respondido a persona que puede pasar") : Logger::logg("Respondido a persona que NO puede pasar");
}
    
void InterfazPersonaEntrada::responderInvestigador(long idInvestigador,int tarjeta){ 
    MensajeAPuerta msg;
    msg.myType = myID;
    msg.origen = myID;
    msg.destino  = idInvestigador;
    msg.mensaje = (tarjeta!=-1) ? MENSAJE_PASAR : MENSAJE_NO_PASAR;
    msg.pertenenciasOTarjeta = tarjeta;
    Logger::logg("Respondiendo al investigador que puede pasar");
    if(msgsnd(colaRespuesta,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
        Logger::loggError("Error responder a la persona");
        exit(1);   
    }
    (tarjeta!=-1) ? Logger::logg("Respondido a investigador que puede pasar") : Logger::logg("Respondido a investigador que NO puede pasar");
    
}
    
