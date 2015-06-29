/* 
 * File:   Broker.cpp
 * Author: knoppix
 *
 * Created on June 21, 2015, 12:38 PM
 */

#include <cstdlib>

using namespace std;

/*
 * 
 */
#include "Logger.h"
#include "Simulador.h"

int main(int argc, char** argv) {
    Logger::startLog(LOGGER_DEFAULT_PATH);
    
    
    
    
    Logger::closeLogger();
    return 0;
}

