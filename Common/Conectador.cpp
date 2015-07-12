
#include "Conectador.h"
#include "tcpopact.h"
#include "Logger.h"


int conectTo(const char* normbreBroker, long id, long ftokCE, long ftokCS, int portCE, int portCS){
    
    //pido unirme al broker
    int newsockfdCS = tcpopact(normbreBroker,portCS);
    if (newsockfdCS < 0) {
        Logger::loggError("cliente: error al generar primera conexion");
        return -1;
    }
    
    int newsockfdCE = tcpopact(normbreBroker,portCE);
    if (newsockfdCE < 0) {
        Logger::loggError("cliente: error al generar segunda conexion");
        close(newsockfdCS);
        return -1; 
    }
    
    static char ID[12];
    static char colaCE[12];
    static char colaCS[12];
    static char CE[12];
    static char CS[12];
    static char tokill[12];
    sprintf(ID,"%ld",id);
    sprintf(colaCE,"%ld",ftokCE);
    sprintf(colaCS,"%ld",ftokCS);
    sprintf(CE,"%d",newsockfdCE);
    sprintf(CS,"%d",newsockfdCS);
    sprintf(tokill,"%d",0);
    
    //bifurco el proceso de comunicaciones entrantes
        //parametro 1 id
        //parametro 2 el fd de la cola
        //parametro 3 el newsockfdCE
        //parametro 4 el pid de salientes para hacerle kill
        //hacer close del newsockfdCS sockfdCE y sockfdCS
    int childpid;
    if ((childpid=fork())<0){
        Logger::loggError("Error al crear CE ");
        return -2; 
    }else if (childpid == 0){
        close(newsockfdCS);
        execlp(PATH_CLIENTE_CE,NAME_CLIENTE_CE,ID,colaCE,CE,tokill,(char*)NULL);
        Logger::loggError("Error al cargar la imagen de ejecutable del CSCliente");
        return -3;
    }
    
    //bifurco el proceso de comunicaciones salientes
       //parametro 1 el fd de la cola
       //parametro 2 el newsockfdCS
       //hacer close del newsockfdCE
    
    sprintf(tokill,"%d",childpid);
    
    if ((childpid=fork())<0){
        Logger::loggError("Error al crear CS ");
        return -2;
    }else if (childpid == 0){
        close(newsockfdCE);
        execlp(PATH_CLIENTE_CS,NAME_CLIENTE_CS,ID,colaCS,CS,tokill,(char*)NULL);
        Logger::loggError("Error al cargar la imagen de ejecutable del CSCliente");
        return -3;
    }
    
    char pidCS[12];
    sprintf(pidCS,"%d",childpid);
    Logger::logg(std::string("Creada conexion bajo procesos CE: ")+tokill+" y CS: "+pidCS);
        
    close(newsockfdCE);
    close(newsockfdCS);
    
    return 0;

}

int conectToSHM(const char* normbreBroker, long id, long ftokCE, long ftokCS, int portCE, int portCS){
    
    
    //pido unirme al broker
    int newsockfdCS = tcpopact(normbreBroker,portCS);
    if (newsockfdCS < 0) {
        Logger::loggError("cliente: error al generar primera conexion");
        return -1;
    }
    
    int newsockfdCE = tcpopact(normbreBroker,portCE);
    if (newsockfdCE < 0) {
        Logger::loggError("cliente: error al generar segunda conexion");
        close(newsockfdCS);
        return -1; 
    }
    
    static char ID[12];
    static char colaCE[12];
    static char colaCS[12];
    static char CE[12];
    static char CS[12];
    static char tokill[12];
    sprintf(ID,"%ld",id);
    sprintf(colaCE,"%ld",ftokCE);
    sprintf(colaCS,"%ld",ftokCS);
    sprintf(CE,"%d",newsockfdCE);
    sprintf(CS,"%d",newsockfdCS);
    sprintf(tokill,"%d",0);
    
    //bifurco el proceso de comunicaciones entrantes
        //parametro 1 id
        //parametro 2 el fd de la cola
        //parametro 3 el newsockfdCE
        //parametro 4 el pid de salientes para hacerle kill
        //hacer close del newsockfdCS sockfdCE y sockfdCS
    int childpid;
    if ((childpid=fork())<0){
        Logger::loggError("Error al crear CE ");
        return -2; 
    }else if (childpid == 0){
        close(newsockfdCS);
        execlp(PATH_CLIENTE_CE_MUSEO,NAME_CLIENTE_CE_MUSEO,ID,colaCE,CE,tokill,(char*)NULL);
        Logger::loggError("Error al cargar la imagen de ejecutable del CSCliente");
        return -3;
    }
    
    //bifurco el proceso de comunicaciones salientes
       //parametro 1 el fd de la cola
       //parametro 2 el newsockfdCS
       //hacer close del newsockfdCE
    
    sprintf(tokill,"%d",childpid);
    
    if ((childpid=fork())<0){
        Logger::loggError("Error al crear CS ");
        return -2;
    }else if (childpid == 0){
        close(newsockfdCE);
        execlp(PATH_CLIENTE_CS_MUSEO,NAME_CLIENTE_CS_MUSEO,ID,colaCS,CS,tokill,(char*)NULL);
        Logger::loggError("Error al cargar la imagen de ejecutable del CSCliente");
        return -3;
    }
    
    char pidCS[12];
    sprintf(pidCS,"%d",childpid);
    Logger::logg(std::string("Creada conexion bajo procesos CE: ")+tokill+" y CS: "+pidCS);
        
    close(newsockfdCE);
    close(newsockfdCS);
    
    return 0;

}