
#include "../Common/tcpoppas.cpp"
#include "../Common/tcpopact.cpp"
#include "../Common/Logger.h"
#include "../Common/inet.h"

#include "Constantes.h"

#define ID "NameServerBroker"

int main (int argc, char** argv){
    
    Logger::startLog("./log.txt",ID);
    
    int sockfd;
    int port = PORT_FIJO +2;
    
    //abro el socket para recibir pedidos de union
    if ( (sockfd = tcpoppas(port)) < 0){
        Logger::loggError("server: no se puede abrir el stream socket");
        exit(1);
    }
    Logger::logg("server: se hizo el open pasivo");
    
    while(true){
        
        //leo un nuevo pedido de union
        struct sockaddr_in cli_addr;
        unsigned clilen = sizeof(cli_addr);
        int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            Logger::loggError("server: error en el accept");
            exit(1); 
        }
                        
        static char soc[12];
        sprintf(soc,"%d",newsockfd);
        
        //bifurco el proceso que se encargara de responder pedidos de ids etc
        int childpid;
        if ((childpid=fork())<0){
            Logger::loggError("Error al crear NameServer");
            exit(1);   
        }else if (childpid == 0){
            close(sockfd);
            execlp(PATH_NAME_SERVER_EXEC,NAME_NAME_SERVER_EXEC,soc,(char*)NULL);
            Logger::loggError("Error al cargar la imagen de ejecutable del CSbroker");
            exit(1);
        }
    }
    
    Logger::closeLogger();
    
}
