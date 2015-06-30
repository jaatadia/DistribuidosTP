
#include "tcpoppas.cpp"
#include "tcpopact.cpp"
#include "Logger.h"
#include "inet.h"

#define ID "ConectionConecter"


#define PATH_CLIENTE_CS "../Common/CSCliente"
#define NAME_CLIENTE_CS "CSCliente"

#define PATH_CLIENTE_CE "../Common/CECliente"
#define NAME_CLIENTE_CE "CECliente"

const char* name="broker"; //TODO cambiar a leer de un archivo

int main (int argc, char** argv){
    
    if(argc<4){
        printf("Mal uso 1:nombreBroker 2:id 3:ftok cola entrada 4: ftok cola salida\n");
        return -1;
    }
    
    Logger::startLog(LOGGER_DEFAULT_PATH,ID);//TODO CAMBIAR
    
    int portCS = PORT_FIJO;//TODO cambiar a un port leido desde algun lado
    int portCE = PORT_FIJO+1;
    
    //pido unirme al broker
    int newsockfdCS = tcpopact(argv[1],portCS);
    if (newsockfdCS < 0) {
        Logger::loggError("cliente: error al generar primera conexion");
        exit(1); 
    }
    
    int newsockfdCE = tcpopact(name,portCE);
    if (newsockfdCE < 0) {
        Logger::loggError("cliente: error al generar segunda conexion");
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
       //parametro 1 el fd de la cola
       //parametro 2 el newsockfdCS
       //hacer close del newsockfdCE
    int childpid;
    if ((childpid=fork())<0){
        Logger::loggError("Error al crear CS ");
        exit(1);   
    }else if (childpid == 0){
        close(newsockfdCE);
        execlp(PATH_CLIENTE_CS,NAME_CLIENTE_CS,argv[4],CS,tokill,(char*)NULL);
        Logger::loggError("Error al cargar la imagen de ejecutable del CSCliente");
        exit(1);
    }
    
    sprintf(tokill,"%d",childpid);
    
    //bifurco el proceso de comunicaciones entrantes
        //parametro 1 id
        //parametro 2 el fd de la cola
        //parametro 3 el newsockfdCE
        //parametro 4 el pid de salientes para hacerle kill
        //hacer close del newsockfdCS sockfdCE y sockfdCS
    if ((childpid=fork())<0){
        Logger::loggError("Error al crear CE ");
        exit(1);   
    }else if (childpid == 0){
        close(newsockfdCS);
        execlp(PATH_CLIENTE_CE,NAME_CLIENTE_CE,argv[2],argv[3],CE,tokill,(char*)NULL);
        Logger::loggError("Error al cargar la imagen de ejecutable del CSCliente");
        exit(1);
    }
}
