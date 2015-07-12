
#include <sys/msg.h>

#include "../Common/tcpoppas.cpp"
#include "../Common/Logger.h"
#include "../Common/Museo.h"
#include "Constantes.h"

#define ID "BrokerSHM"

int main (int argc, char** argv){
    
    if(argc!=5){
        printf("Mal uso 1: key Cola 2: abierto 3: genteAdentro 4: maximoPersonas");
        return -1;
    }
    
    int cola = atoi(argv[1]);
    
    Logger::startLog(BROKER_LOGGER_DEFAULT_PATH,ID);
    
    if( (cola = msgget(cola,0660)) == -1){
        Logger::loggError("Error al encontrar la cola del brokerSHM");
        exit(1);   
    }
    
    Museo shm;
    shm.estaAbierto=(atoi(argv[2])==1);
    shm.personasAdentro=atoi(argv[3]);
    shm.museoMax=atoi(argv[4]);
    
    while(true){
        Museo museo;
        
        Logger::logg("Esperando pedido de shm");
        if(msgrcv(cola,&museo,sizeof(Museo)-sizeof(long),PETICION,0)==-1){
            Logger::loggError("Error al escribir el mensaje ");
            exit(1);
        }

        char origen[14];
        sprintf(origen,"%ld",museo.origen);
                
        museo.myType=museo.origen;
        museo.destino=museo.origen;
        museo.origen=0;
        museo.estaAbierto=shm.estaAbierto;
        museo.personasAdentro=shm.personasAdentro;
        museo.museoMax=shm.museoMax;
        
        Logger::logg(std::string("Enviando la shm a: ")+origen);
        
        if(msgsnd(cola,&museo,sizeof(Museo)-sizeof(long),0)==-1){
            Logger::loggError("Error al escribir el mensaje ");
            exit(1);
        }
        
        Logger::logg("Esperando que la shm vuelva");
        if(msgrcv(cola,&museo,sizeof(Museo)-sizeof(long),DEVOLUCION,0)==-1){
            Logger::loggError("Error al escribir el mensaje ");
            exit(1);
        }
        
        Logger::logg("Actualizando la shm");
        shm.estaAbierto=museo.estaAbierto;
        shm.personasAdentro=museo.personasAdentro;

        
    }
    
    Logger::closeLogger();
    
}
