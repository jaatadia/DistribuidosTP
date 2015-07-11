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
#include <fstream>

#include "../Common/MensajeAPuerta.h"
#include "../Common/Logger.h"
#include "../Common/Parser.h"
#include "../Common/Conectador.cpp"

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

    Parser::setPath("../broker.conf");
    int portCS = Parser::getIntParam("PUERTO_1");
    int portCE = Parser::getIntParam("PUERTO_2");
    if(portCS<0 || portCE<0){
        Logger::loggError("Error al leer los puertos del broker");
        exit(1);   
    }
 
    char broker[255];
    int result=-1;
    std::ifstream file;
    file.open("../brokers.conf");
    Logger::logg("Buscando broker");
    while((result==-1) && (!file.eof())){
        file.getline(broker,255);
        Logger::logg(std::string("Tratando de conectar con broker: ")+broker);
        result = conectTo(broker,myID,ftok(PUERTA_FILE_IPC,COLA_PUERTA_ENTRADA),ftok(PUERTA_FILE_IPC,COLA_PUERTA_ENTRADA_RESPUESTA),portCE,portCS);
    }
    file.close();    
    if(result!=0){
        Logger::loggError("Error al conectarse con el broker");
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
    
