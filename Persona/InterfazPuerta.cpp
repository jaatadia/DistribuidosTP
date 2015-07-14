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
#include <fstream>

#include "../Common/Logger.h"
#include "../Common/Conectador.h"
#include "../Common/Parser.h"
#include "../Broker/idServerRPC/idServer_client.h"
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
        
        myID=idServer_client::getInst()->getNuevoIdPersona();
        if(myID==-1){
            Logger::loggError("No pude conseguir un id");
            exit(1);
        }
        
        char line[255];
        sprintf(line,"Tengo id: %ld",myID);
        Logger::logg(line);
        
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
            result = conectTo(broker,myID,ftok(PERSONA_FILE_IPC,COLA_RESPUESTA),ftok(PERSONA_FILE_IPC,COLA),portCE,portCS);
        }
        file.close();    
        if(result!=0){
            Logger::loggError("Error al conectarse con el broker");
            exit(1);   
        }
 }

    
   void InterfazPuerta::entrar(int numeroPuerta,int tipo,int tarjeta, MensajeAPuerta& mensaje){

        long puertaId=idServer_client::getInst()->getIdPuertaEnt(numeroPuerta);
        if(puertaId==-1){
            Logger::loggError("No se encontro la puerta");
            mensaje.mensaje=MENSAJE_NO_PASAR;
            return;
        }
        
        stringstream ss;
        ss<<numeroPuerta;

        MensajeAPuerta msg;
        msg.myType=myID;
        msg.origen=myID;
        msg.destino=puertaId;
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

        long puertaId=idServer_client::getInst()->getIdPuertaSal(numeroPuerta);
        if(puertaId==-1){
            Logger::loggError("No se encontro la puerta");
            msg.mensaje=MENSAJE_NO_PASAR;
            return;
        }
        
        msg.myType=myID;
        msg.origen=myID;
        msg.destino=puertaId;
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
        msg.origen=myID;
        msg.destino=myID;
        msg.mensaje=MENSAJE_END_COMMUNICATION;
        Logger::logg("Enviando mensaje para cerrar comunicaciones");
        if(msgsnd(cola,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
            Logger::loggError("Error al escribir el mensaje ");
            exit(1);
        }
        
        char line[255];
        sprintf(line,"Devolviendo id: %ld",myID);
        Logger::logg(line);
        idServer_client::getInst()->devolverId(myID);
    }