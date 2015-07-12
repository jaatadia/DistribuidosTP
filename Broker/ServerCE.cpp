
#include "../Common/tcpoppas.cpp"
#include "../Common/Logger.h"
#include "../Common/Logger.h"
#include "Constantes.h"

#define ID "BrokerConectionListener"

int main (int argc, char** argv){
    
    if(argc!=5){
        printf("Mal uso 1: key Cola 2: portCE 3:image 4: imageName");
        return -1;
    }
    
    Logger::startLog(BROKER_LOGGER_DEFAULT_PATH,ID);
    
    
    int sockfdCE;
    int portCE = atoi(argv[2]);
    
    //abro el socket para recibir pedidos de union
    if ( (sockfdCE = tcpoppas(portCE)) < 0){
        Logger::loggError("server: no se puede abrir el stream socket");
        exit(1);
    }
    Logger::logg("server: se hizo el open pasivo de CE");

    
    while(true){
        
        int status;
        while(waitpid(-1,&status,WNOHANG)>0){
            Logger::logg("Mate un zombie");
        };
        
        Logger::logg("Esperando nueva conexion");
        //leo un nuevo pedido de union
        struct sockaddr_in cli_addr;
        unsigned clilen = sizeof(cli_addr);
        int newsockfdCE = accept(sockfdCE, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfdCE < 0) {
            Logger::loggError("server: error en el accept");
            exit(1); 
        }
      
        static char CE[12];
        sprintf(CE,"%d",newsockfdCE);
        
        //bifurco el proceso de comunicaciones entrantes
            //parametro 1 el int para ftok cola
            //parametro 2 el newsockfdCE
            //parametro 3 el pid de salientes para hacerle kill
            //hacer close del newsockfdCS sockfdCE y sockfdCS
        int childpid;
        if ((childpid=fork())<0){
            Logger::loggError("Error al crear CE ");
            exit(1);   
        }else if (childpid == 0){
            close(sockfdCE);
            execlp(argv[3],argv[4],argv[1],CE,(char*)NULL);
            Logger::loggError("Error al cargar la imagen de ejecutable del CEbroker");
            exit(1);
        }
        
        close(newsockfdCE);
    }
    
    Logger::closeLogger();
    
}
