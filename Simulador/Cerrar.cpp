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

#include "Logger.h"
#include "Simulador.h"
#include "semaforo.h"
#include "InterfazMuseoAdministrador.h"

using namespace std;

#define CERRAR_ID "Cerrar"

int main(int argc, char** argv) {
    Logger::startLog(LOGGER_DEFAULT_PATH,CERRAR_ID);
  
    InterfazMuseoAdministrador museo;
    museo.cerrar();
    
    Logger::closeLogger();
    
    return 0;
}

