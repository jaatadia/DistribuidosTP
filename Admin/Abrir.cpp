/* 
 * File:   Abrir.cpp
 * Author: knoppix
 *
 * Created on March 28, 2015, 8:50 PM
 */

#include <stdlib.h>
#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#include "../Common/Logger.h"
#include "InterfazMuseoAdministrador.h"

using namespace std;

#define ABRIR_ID "Abrir"

int main(int argc, char** argv) {
    Logger::startLog(LOGGER_DEFAULT_PATH,ABRIR_ID);
    
    InterfazMuseoAdministrador museo;
    Logger::logg(APP_LEVEL,"Iniciando Apertura de museo");
    museo.abrir();
    Logger::logg(APP_LEVEL,"Finalizado Apertura de museo");
    
    
    Logger::closeLogger();
    
    return 0;
}

