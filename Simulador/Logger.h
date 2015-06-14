/* 
 * File:   Logger.h
 * Author: knoppix
 *
 * Created on March 24, 2015, 8:39 PM
 */

#ifndef LOGGER_H
#define	LOGGER_H



#include <iostream>
#include <string>

class Logger {
public:
    Logger( const Logger& other );
    Logger(const char* path,const char* identifier);
    void loggMessage(const std::string message);
    virtual ~Logger();
    
    static void startLog(const char* path,const char* identifier);
    static void logg(const std::string message);
    static void loggError(const std::string message);
    static void closeLogger();
    
private:
    void reset();
    std::string path;
    int fd;
    const char* identifier;
    static Logger* logger_instance;
};

#endif	/* LOGGER_H */

