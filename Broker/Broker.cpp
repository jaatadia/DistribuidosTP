
#include "../Common/tcpoppas.cpp"
#include "../Common/Logger.h"
#include "../Common/Logger.h"
#include "Constantes.h"

#define ID "BrokerConectionListener"

int main (int argc, char** argv){
    
    if(argc<2){
        printf("Mal uso 1: key Cola");
        return -1;
    }
    
    Logger::startLog("./log.txt",ID); //TODO cambiar de aca
    
    
    int sockfdCE,sockfdCS;
    int portCE = PORT_FIJO;//TODO cambiar a un port leido desde algun lado
    int portCS = PORT_FIJO +1;
    
    //abro el socket para recibir pedidos de union
    if ( (sockfdCE = tcpoppas(portCE)) < 0){
        Logger::loggError("server: no se puede abrir el stream socket");
        exit(1);
    }
    Logger::logg("server: se hizo el open pasivo");

    //abro el socket para recibir pedidos de union
    if ( (sockfdCS = tcpoppas(portCS)) < 0){
        Logger::loggError("server: no se puede abrir el stream socket");
        exit(1);
    }
    Logger::logg("server: se hizo el open pasivo");
    
    while(true){
        
        int status;
        while(waitpid(1,&status,WNOHANG)>0){};
        
        //leo un nuevo pedido de union
        struct sockaddr_in cli_addr;
        unsigned clilen = sizeof(cli_addr);
        int newsockfdCE = accept(sockfdCE, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfdCE < 0) {
            Logger::loggError("server: error en el accept");
            exit(1); 
        }
      
        //leo un nuevo pedido de union
        int newsockfdCS = accept(sockfdCS, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfdCS < 0) {
            Logger::loggError("server: error en el accept");
            exit(1); 
        }
                        
        static char CE[12];
        static char CS[12];
        static char tokill[12];
        sprintf(CE,"%d",newsockfdCE);
        sprintf(CS,"%d",newsockfdCS);
        sprintf(tokill,"%d",0);
        
        //TODO cambiar al orden correcto
        
        //bifurco el proceso de comunicaciones salientes
            //parametro 1 el int par ftok cola
            //parametro 2 el newsockfdCS
            //parametro 3 el pid de entrantes para hacerle kill
            //hacer close del newsockfdCE sockfdCE y sockfdCS
        int childpid;
        if ((childpid=fork())<0){
            Logger::loggError("Error al crear CS ");
            exit(1);   
        }else if (childpid == 0){
            close(newsockfdCE);
            close(sockfdCE);
            close(sockfdCS);
            execlp(PATH_BROKER_CS,NAME_BROKER_CS,argv[1],CS,tokill,(char*)NULL);
            Logger::loggError("Error al cargar la imagen de ejecutable del CSbroker");
            exit(1);
        }
        
        sprintf(tokill,"%d",childpid);
        
        //bifurco el proceso de comunicaciones entrantes
            //parametro 1 el int para ftok cola
            //parametro 2 el newsockfdCE
            //parametro 3 el pid de salientes para hacerle kill
            //hacer close del newsockfdCS sockfdCE y sockfdCS
        if ((childpid=fork())<0){
            Logger::loggError("Error al crear CE ");
            exit(1);   
        }else if (childpid == 0){
            close(newsockfdCE);
            close(sockfdCE);
            close(sockfdCS);
            execlp(PATH_BROKER_CE,NAME_BROKER_CE,argv[1],CS,tokill,(char*)NULL);
            Logger::loggError("Error al cargar la imagen de ejecutable del CEbroker");
            exit(1);
        }
    }
    
    Logger::closeLogger();
    
}
