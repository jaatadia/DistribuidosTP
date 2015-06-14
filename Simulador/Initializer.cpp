/* 
 * File:   main.cpp
 * Author: knoppix
 *
 * Created on March 24, 2015, 1:51 PM
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sstream>
#include <sys/wait.h>

#include "Simulador.h"
#include "Logger.h"
#include "semaforo.h"
#include "Parser.h"

using namespace std;

void crearCarpeta(){
       
    //string commandoLimpiar=string("sudo rm -fr ")+DIRECTORIO_IPC;
    //Logger::logg(string("Borrando carpeta de IPCs")+DIRECTORIO_IPC);
    //system(commandoLimpiar.c_str());    
    
    string commandoCrear=string("sudo mkdir --mode=0777 -p ")+DIRECTORIO_IPC;
    Logger::logg(string("Creando carpeta de IPCs: ")+DIRECTORIO_IPC);
    system(commandoCrear.c_str());    

}

void crearMuseo(){
    
    Logger::logg("Creando la memoria compartida");
    int shmid;
    if( (shmid = shmget(ftok(DIRECTORIO_IPC,MUSEO), sizeof(Museo),IPC_CREAT|IPC_EXCL|PERMISOS)) == -1 ){
        Logger::loggError("Error al crear la memoria compartida");
        exit(1);   
    }
    
    Logger::logg("Uniendose a la memoria compartida");
    Museo* myMuseum;
    if ( (myMuseum = (Museo*) shmat(shmid,NULL,0)) == (Museo*) -1 ){
        Logger::loggError("Error al atachearse a la memoria compartida");
        exit(1);   
    }
    
    
    //TODO cargar configuracion de memoria un archivo
    Logger::logg("Inicializando la memoria compartida");
    
    int result;
    
    if((result = Parser::getBoolParam(MUSEO_OPEN)) < 0 ){
        Logger::loggError("Error al leer la configuracion del museo si esta abierto");
        exit(1);   
    }
    myMuseum->estaAbierto= (result == BOOL_TRUE);
    
    if((result = Parser::getIntParam(MUSEO_CANT_GENTE)) < 0 ){
        Logger::loggError("Error al leer la configuracion del museo cuantas personas hay");
        exit(1);   
    }
    myMuseum->personasAdentro=result;
    
    if((result = Parser::getIntParam(MUSEO_PUERTAS)) < 0 ){
        Logger::loggError("Error al leer la configuracion del museo cuantas puertas");
        exit(1);   
    }
    myMuseum->cantPuertas=result;
    
    if((result = Parser::getIntParam(MUSEO_MAX_PERSONAS)) < 0 ){
        Logger::loggError("Error al leer la configuracion del museo cual es el maximo de personas");
        exit(1);   
    }
    myMuseum->museoMax=result;
    
    
    
    Logger::logg("Desuniendose de la memoria compartida");
    if(shmdt(myMuseum)==-1){
        Logger::loggError("Error al desatachearse de la memoria compartida");
        exit(1);   
    }
    
    Logger::logg("Creando semaforo de exclusion mutua del estado");
    int semid;
    if ((semid = creasem(MUTEX_ESTADO))== -1){
        Logger::loggError("Error al crear el semaforo de mutex del estado");
        exit(1);   
    };
    
    Logger::logg("Inicializando el semaforo de exclusion mutua del estado");
    if (inisem(semid,1)==-1){
        Logger::loggError("Error al inicializar semaforo de mutex del estado");
        exit(1);   
    }
    
    Logger::logg("Creando semaforo que indica si hay lugar");
    if ((semid = creasem(MUTEX_LUGAR))== -1){
        Logger::loggError("Error al crear el semaforo de mutex");
        exit(1);   
    };
    
    Logger::logg("Inicializando el semaforo que indica si hay lugar");
    if (inisem(semid,1)==-1){
        Logger::loggError("Error al inicializar semaforo que indica si hay lugar");
        exit(1);   
    }
    
    Logger::logg("Creando semaforo de exclusion mutua de las personas adentro");
    if ((semid = creasem(MUTEX_CONTADOR))== -1){
        Logger::loggError("Error al crear el semaforo de exclusion mutua de las personas adentro");
        exit(1);   
    };
    
    Logger::logg("Inicializando el semaforo de exclusion mutua de las personas adentro");
    if (inisem(semid,1)==-1){
        Logger::loggError("Error al inicializar semaforo de exclusion mutua de las personas adentro");
        exit(1);   
    }
    
}


void crearPuertas(){
    //todo cargar cantidad de puertas desde archivo
    int childpid;
    int result = 0;
    if((result = Parser::getIntParam(MUSEO_PUERTAS)) < 0 ){
        Logger::loggError("Error al leer la configuracion de la cantidad de puertas");
        exit(1);   
    }
    
    int cola;
    
    
    for (int i=0;i<result;i++){
    
        std::stringstream ss;
        ss<<i;
        Logger::logg(string("Creando la puerta nro ")+ss.str());
        
        //creo las colas para la puerta
        Logger::logg("Creando la cola de entrada");
        if( (cola = msgget(ftok(DIRECTORIO_IPC,PUERTA_FILA + DESP * i),IPC_CREAT|IPC_EXCL|PERMISOS)) == -1){
            Logger::loggError("Error al crear la cola de entrada");
            exit(1);   
        }
        
        Logger::logg("Creando la cola de respuesta de entrada");
        if( (cola = msgget(ftok(DIRECTORIO_IPC,PUERTA_RESP + DESP * i),IPC_CREAT|IPC_EXCL|PERMISOS)) == -1){
            Logger::loggError("Error al crear la cola de respuesta de entrada");
            exit(1);   
        }
        
        //TODO Preguntar sobre el pasaje del parametro
        Logger::logg("Creando el proceso puerta");
        //preparo los parametros para la puerta
        if ((childpid=fork())<0){
            Logger::loggError(string("Error al crear la puerta nro ") + ss.str());
            exit(1);   
        }else if (childpid == 0){
            execlp(PATH_PUERTA_EXEC,NAME_PUERTA_EXEC,ss.str().c_str(),(char*)NULL);
            Logger::loggError(string("Error al cargar la imagen de ejecutable en la puerta nro ") + ss.str());
            exit(1);
        }
        
        
        
        //creo las colas para la puerta de salida
        Logger::logg("Creando la cola de salida");
        if( (cola = msgget(ftok(DIRECTORIO_IPC,PUERTA_SALIDA_FILA + DESP *i),IPC_CREAT|IPC_EXCL|PERMISOS)) == -1){
            Logger::loggError("Error al crear la cola de salida");
            exit(1);   
        }
        
        Logger::logg("Creando la cola de respuesta de salida");
        if( (cola = msgget(ftok(DIRECTORIO_IPC,PUERTA_SALIDA_RESP + DESP *i),IPC_CREAT|IPC_EXCL|PERMISOS)) == -1){
            Logger::loggError("Error al crear la cola de respuesta de salida");
            exit(1);   
        }
        
        //TODO Preguntar sobre el pasaje del parametro
        Logger::logg("Creando el proceso puerta de salida");
        //preparo los parametros para la puerta
        if ((childpid=fork())<0){
            Logger::loggError(string("Error al crear la puerta de salida nro ") + ss.str());
            exit(1);   
        }else if (childpid == 0){
            execlp(PATH_PUERTA_SALIDA_EXEC,NAME_PUERTA_SALIDA_EXEC,ss.str().c_str(),(char*)NULL);
            Logger::loggError(string("Error al cargar la imagen de ejecutable en la puerta de salida nro ") + ss.str());
            exit(1);
        }
        
    }
    
    
}

void crearClientes(){
    int personas;
    if((personas = Parser::getIntParam(CANT_PERSONAS)) < 0 ){
        Logger::loggError("Error al leer la configuracion de la cantidad personas");
        exit(1);   
    }
    int puertas;
    if((puertas = Parser::getIntParam(MUSEO_PUERTAS)) < 0 ){
        Logger::loggError("Error al leer la configuracion de la cantidad de puertas puerta");
        exit(1);   
    }
    
    std::stringstream per;
    per << personas;
    
    Logger::logg("Creando "+per.str()+"personas");
    
    int cola;
    Logger::logg(string("Creando cola para terminacion de las personas"));
    if( (cola = msgget(ftok(DIRECTORIO_IPC,COLA_MATAR_PERSONAS),IPC_CREAT|IPC_EXCL|PERMISOS)) == -1){
        Logger::loggError("Error al crear la cola para terminacion de las puertas");
        exit(1);   
    }
    
    srand(time(NULL));
    for (int i=0;i<100;i++){rand();};
    
    for (int i=0;i<personas;i++){
        stringstream puertaEntrada;
        stringstream espera;
        stringstream puertaSalida;
        
        puertaEntrada<<rand()%puertas;
        espera<<rand()%10000000;
        puertaSalida<<rand()%puertas;
        
        Logger::logg("Creando una persona Entra: "+puertaEntrada.str()+" Duerme: "+espera.str()+" Sale:"+puertaSalida.str());
        
        int childpid;
        if( ( childpid = fork() ) < 0 ){
            Logger::loggError("Error al forkearse");
            exit(1);   
        }else if(childpid==0){
            execlp(PATH_PERSONA_EXEC,NAME_PERSONA_EXEC,puertaEntrada.str().c_str(),espera.str().c_str(),puertaSalida.str().c_str(),(char*)NULL);
            Logger::loggError("Error al generar la persona");
            exit(1);
        }
        
        while(waitpid(-1,NULL,WNOHANG)!=0);
        usleep(rand()%100000);
    }
    
}


#define INITIALIZER_ID "Initializer"
int main(int argc, char** argv) {
    Parser::setPath(MUSEO_CONF);
    Logger::startLog(LOGGER_DEFAULT_PATH,INITIALIZER_ID);
    
    crearCarpeta();
    crearMuseo();
    crearPuertas();
    crearClientes();
    
    Logger::closeLogger();
    
    return 0;
}

