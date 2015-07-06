/* 
 * File:   PersonaInitializer.cpp
 * Author: knoppix
 *
 * Created on June 30, 2015, 2:12 PM
 */

#include <cstdlib>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sstream>
#include <sys/types.h>
#include <sys/wait.h>


#include "Constantes.h"
#include "../Common/Logger.h"
#include "../Common/Parser.h"

using namespace std;

void crearCarpeta(){
    string commandoCrear=string("sudo mkdir --mode=0777 -p ")+PERSONA_DIRECTORIO_IPC;
    string commandoFile=string("touch ")+PERSONA_FILE_IPC;
    Logger::logg(string("Creando carpeta de IPCs: ")+PERSONA_DIRECTORIO_IPC);
    system(commandoCrear.c_str());
    system(commandoFile.c_str());
}

void crearClientes(){
    Parser::setPath(MUSEO_CONF);
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
        
        while(waitpid(-1,NULL,WNOHANG)>0);
        usleep(rand()%100000);
    }
    
}

#define ID "PersonaInitializer"
int main(int argc, char** argv) {
    
    Logger::startLog(LOGGER_DEFAULT_PATH,ID);
    
    crearCarpeta();
    
    Logger::logg("creando la cola para las personas");
    int cola;
    if( (cola = msgget(ftok(PERSONA_FILE_IPC,COLA),IPC_CREAT|PERMISOS)) == -1){
            Logger::loggError("Error al encontrar la cola de entrada");
            exit(1);   
    }
    
    Logger::logg("creando la cola para las respuestas de las personas");
    if( (cola = msgget(ftok(PERSONA_FILE_IPC,COLA_RESPUESTA),IPC_CREAT|PERMISOS)) == -1){
            Logger::loggError("Error al encontrar la cola de entrada");
            exit(1);   
    }
    
    if(argc==1){crearClientes();}
    
    Logger::closeLogger();
    return 0;
}

