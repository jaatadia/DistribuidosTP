
#include "../Common/tcpoppas.cpp"
#include "../Common/Logger.h"
#include "../Common/Logger.h"
#include "Constantes.h"

#define ID "BrokerConectionListener"

int main (int argc, char** argv){
    
    if(argc!=5){
        printf("Mal uso 1: key 2:portCS 3:image 4:imageName");
        return -1;
    }
    
    Logger::startLog(BROKER_LOGGER_DEFAULT_PATH,ID);
    
    
    int sockfdCS;
    int portCS = atoi(argv[2]);
    

    //abro el socket para recibir pedidos de union
    if ( (sockfdCS = tcpoppas(portCS)) < 0){
        Logger::loggError("server: no se puede abrir el stream socket");
        exit(1);
    }
    Logger::logg("server: se hizo el open pasivo de CS");
    
    while(true){
        
        int status;
        while(waitpid(-1,&status,WNOHANG)>0){
            Logger::logg("Mate un zombie");
        };
        
        Logger::logg("Esperando nueva conexion");
        //leo un nuevo pedido de union
        struct sockaddr_in cli_addr;
        unsigned clilen = sizeof(cli_addr);
      
        //leo un nuevo pedido de union
        int newsockfdCS = accept(sockfdCS, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfdCS < 0) {
            Logger::loggError("server: error en el accept");
            exit(1); 
        }
        Logger::logg("conexiones detectadas, creando proceso CS");
        
        static char CS[12];
        sprintf(CS,"%d",newsockfdCS);
        
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
            close(sockfdCS);
            execlp(argv[3],argv[4],argv[1],CS,(char*)NULL);
            Logger::loggError("Error al cargar la imagen de ejecutable del CSbroker");
            exit(1);
        }
        
        close(newsockfdCS);
        
    }
    
    Logger::closeLogger();
    
}
