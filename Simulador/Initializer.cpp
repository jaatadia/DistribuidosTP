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
#include "MensajeAPuerta.h"

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
    //creo las colas para la puerta
    Logger::logg("Creando la cola de entrada");
    if( (cola = msgget(ftok(DIRECTORIO_IPC,PUERTA_FILA),IPC_CREAT|IPC_EXCL|PERMISOS)) == -1){
        Logger::loggError("Error al crear la cola de entrada");
        exit(1);   
    }

    Logger::logg("Creando la cola de respuesta de entrada");
    if( (cola = msgget(ftok(DIRECTORIO_IPC,PUERTA_RESP),IPC_CREAT|IPC_EXCL|PERMISOS)) == -1){
        Logger::loggError("Error al crear la cola de respuesta de entrada");
        exit(1);   
    }    

    //creo las colas para la puerta de salida
    
    Logger::logg("Creando la cola para el recibidor de personas de salida");
    if( (cola = msgget(ftok(DIRECTORIO_IPC,PUERTA_SALIDA_FILA_ENCOLADOR),IPC_CREAT|IPC_EXCL|PERMISOS)) == -1){
        Logger::loggError("Error al crear la cola de salida");
        exit(1);   
    }
    
    
    Logger::logg("Creando la cola de salida");
    if( (cola = msgget(ftok(DIRECTORIO_IPC,PUERTA_SALIDA_FILA_NORMAL),IPC_CREAT|IPC_EXCL|PERMISOS)) == -1){
        Logger::loggError("Error al crear la cola de salida");
        exit(1);   
    }

    //creo las colas para la puerta de salida prioritarias
    Logger::logg("Creando la cola de salida prioritaria");
    if( (cola = msgget(ftok(DIRECTORIO_IPC,PUERTA_SALIDA_FILA_PRIORITARIA),IPC_CREAT|IPC_EXCL|PERMISOS)) == -1){
        Logger::loggError("Error al crear la cola de salida");
        exit(1);   
    }
    
    Logger::logg("Creando la cola de respuesta de salida");
    if( (cola = msgget(ftok(DIRECTORIO_IPC,PUERTA_SALIDA_RESP),IPC_CREAT|IPC_EXCL|PERMISOS)) == -1){
        Logger::loggError("Error al crear la cola de respuesta de salida");
        exit(1);   
    }
    
    Logger::logg(string("Creando cola para recepcion de pedidos del locker"));
    if( (cola = msgget(ftok(DIRECTORIO_IPC,COLA_LOCKER),IPC_CREAT|IPC_EXCL|PERMISOS)) == -1){
        Logger::loggError("Error al crear la cola para el locker");
        exit(1);   
    }
    
    Logger::logg(string("Creando cola para respuesta de pedidos de extracciones del locker"));
    if( (cola = msgget(ftok(DIRECTORIO_IPC,COLA_LOCKER_RESPUESTA_EXTRACCION),IPC_CREAT|IPC_EXCL|PERMISOS)) == -1){
        Logger::loggError("Error al crear la cola de respuesta para el locker");
        exit(1);   
    }
    
    Logger::logg(string("Creando cola para respuesta de pedidos de depositos del locker"));
    if( (cola = msgget(ftok(DIRECTORIO_IPC,COLA_LOCKER_RESPUESTA_DEPOSITO),IPC_CREAT|IPC_EXCL|PERMISOS)) == -1){
        Logger::loggError("Error al crear la cola de respuesta para el locker");
        exit(1);   
    }
    
    for (int i=1;i<=result;i++){
    
        int semid;
        std::stringstream ss;
        ss<<i;
        Logger::logg(string("Creando la puerta nro ")+ss.str());
           
        
        if ((semid = creasem(MUTEX_PUERTA_ESPERANDO+(i-1)*DESP)) == -1){
          Logger::loggError("Error al crear el semaforo de personas esperando para una puerta");
          exit(1);   
        }
        
        Logger::logg("Inicializando el semaforo de personas esperando para una puerta");
        if (inisem(semid,0)==-1){
            Logger::loggError("Error al inicializar el semaforo de personas esperando para una puerta");
            exit(1);   
        }
        
        if ((semid = creasem(MUTEX_CONTADOR_COLAS_PUERTAS+(i-1)*DESP)) == -1){
          Logger::loggError("Error al crear el semaforo de exclusion mutua del contador de personas esperando en una puerta");
          exit(1);   
        }
        
        Logger::logg("Inicializando el semaforo de exclusion mutua del contador de personas esperando en una puerta");
        if (inisem(semid,1)==-1){
            Logger::loggError("Error al inicializar el semaforo de exclusion mutua del contador de personas esperando en una puerta");
            exit(1);   
        }
        
        
        Logger::logg("Creando la memoria compartida para la puerta");
        int shmid;
        if( (shmid = shmget(ftok(DIRECTORIO_IPC,CONTADOR_COLAS_PUERTAS+(i-1)*DESP), sizeof(ColasPuertas),IPC_CREAT|IPC_EXCL|PERMISOS)) == -1 ){
            Logger::loggError("Error al crear la memoria compartida");
            exit(1);   
        }

        Logger::logg("Uniendose a la memoria compartida");
        ColasPuertas* contador;
        if ( (contador = (ColasPuertas*) shmat(shmid,NULL,0)) == (ColasPuertas*) -1 ){
            Logger::loggError("Error al atachearse a la memoria compartida");
            exit(1);   
        }

        contador->investigadores =  0;
        contador->personasNormales =  0;

        Logger::logg("Desuniendose de la memoria compartida");
        if(shmdt(contador)==-1){
            Logger::loggError("Error al desatachearse de la memoria compartida");
            exit(1);   
        }    
        
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
        
        Logger::logg("Creando el proceso Encolador");
        //preparo los parametros para la puerta
        if ((childpid=fork())<0){
            Logger::loggError(string("Error al crear el encolador nro ") + ss.str());
            exit(1);   
        }else if (childpid == 0){
            execlp(PATH_ENCOLADOR_EXEC,NAME_ENCOLADOR_EXEC,ss.str().c_str(),(char*)NULL);
            Logger::loggError(string("Error al cargar la imagen de ejecutable en la puerta de salida nro ") + ss.str());
            exit(1);
        }
        
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
        
        Logger::logg("Creando el proceso locker");
        if ((childpid=fork())<0){
            Logger::loggError(string("Error al crear el locker"));
            exit(1);   
        }else if (childpid == 0){
            execlp(PATH_LOCKER_EXEC,NAME_LOCKER_EXEC,ss.str().c_str(),(char*)NULL);
            Logger::loggError(string("Error al cargar la imagen de ejecutable en la locker "));
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
    
    Logger::logg("Creando "+per.str()+" personas");
    
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
        stringstream tipoPersona;
        
        puertaEntrada<<(rand()%puertas)+1;
        espera<<rand()%10000000;
        puertaSalida<<(rand()%puertas)+1;
        if ( ((rand()%2) +1) == INVESTIGADOR ) {
            tipoPersona<<INVESTIGADOR;
        } else { tipoPersona<<PERSONA; }
        std::stringstream cantPuertas;
        cantPuertas<<puertas;
        
        
        Logger::logg("Creando una persona Entra: "+puertaEntrada.str()+" Duerme: "+espera.str()+" Sale: "+puertaSalida.str()+" Tipo: "+tipoPersona.str());
        
        int childpid;
        if( ( childpid = fork() ) < 0 ){
            Logger::loggError("Error al forkearse");
            exit(1);   
        }else if(childpid==0){
            execlp(PATH_PERSONA_EXEC,NAME_PERSONA_EXEC,puertaEntrada.str().c_str(),espera.str().c_str(),puertaSalida.str().c_str(),tipoPersona.str().c_str(),cantPuertas.str().c_str(),(char*)NULL);
            Logger::loggError("Error al generar la persona");
            exit(1);
        }
        
        while(waitpid(-1,NULL,WNOHANG)!=0);
        usleep(rand()%100000);
    }
    
}

void crearLocker(int puertas){
    
    
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

