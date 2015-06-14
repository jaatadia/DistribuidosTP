/* 
 * File:   Logger.cpp
 * Author: knoppix
 * 
 * Created on March 24, 2015, 8:39 PM
 */

#include "Logger.h"

#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <unistd.h>
#include <iomanip>
#include <string.h>




Logger* Logger::logger_instance = NULL;

Logger::Logger(const char* path,const char* identifier) {
    this->path = path;
    this->fd = open(path,O_WRONLY|O_APPEND|O_CREAT,00666);
    if(this->fd ==-1){
        perror("Error al abrir el archivo de log");
        exit(1);
    };
    this->identifier = identifier;
}

Logger::Logger( const Logger& other ){
    this->path = other.path;
    this->fd = open(this->path.c_str(),O_WRONLY|O_APPEND|O_CREAT,00666);
    if(this->fd ==-1){
        perror("Error al abrir el archivo de log");
        exit(1);
    };
    this->identifier = other.identifier;
}

void Logger::loggMessage(const std::string message){
    
    if(flock(this->fd,LOCK_EX)==-1){
        perror("Error al obtener el lock sobre el archivo de log");
        exit(1);
    }
    write(STDOUT_FILENO,message.c_str(),sizeof(char)*message.length());
    if (write(this->fd,message.c_str(),sizeof(char)*message.length())==-1){
       flock(this->fd,LOCK_UN);
       perror("Error al escribir sobre el archivo de log");
       exit(1);
    };
    
    if(flock(this->fd,LOCK_UN)==-1){
        perror("Error al devolver el lock sobre el archivo de log");
        exit(1);
    }
}

void Logger::reset(){
    close(this->fd);
    this->fd = open(path.c_str(),O_WRONLY|O_APPEND);
    if(this->fd ==-1){
        perror("Error al re-abrir el archivo de log");
        exit(1);
    };
}

Logger::~Logger() {
    close(this->fd);
}

// dd/mm/aaa hh:mm:ss
std::string getTimeStamp(){
    	time_t now = time(0);
        struct timeval tv;
        gettimeofday(&tv,NULL);
	tm* ltm = localtime(&now);
	std::stringstream ss;
        ss << std::setfill('0') << std::setw(2) << ltm->tm_mday;
        ss << "/";
        ss << std::setfill('0') << std::setw(2) << ltm->tm_mon;
        ss << "/";
        ss << std::setfill('0') << std::setw(4) << (ltm->tm_year + 1900 );
        ss << " ";
        ss << std::setfill('0') << std::setw(2) << ltm->tm_hour;
        ss << ":";
        ss << std::setfill('0') << std::setw(2) << ltm->tm_min;
        ss << ":";
        ss << std::setfill('0') << std::setw(2) << ltm->tm_sec;
        ss << ":";
        ss << std::setfill('0') << std::setw(3) << int((tv.tv_usec)/1000) ;
        ss << ":";
        ss << std::setfill('0') << std::setw(3) << tv.tv_usec - int((tv.tv_usec)/1000)*1000 ;

	return ss.str();
}

std::string formatMessage(std::string identifier,std::string message){
    std::stringstream ss;
    ss << "Process: " << identifier << " PID: " << getpid() << " Time: " << getTimeStamp() << " Message: " << message << std::endl;
    return ss.str();
}

std::string formatError(std::string identifier,std::string message){
    std::stringstream ss;
    ss << "Process: " << identifier << " PID: " << getpid() << " Time: " << getTimeStamp() << " Error: " << message <<": "<< strerror(errno) << std::endl;
    return ss.str();
}

void Logger::startLog(const char* path,const char* identifier){
    if (Logger::logger_instance == NULL){
        Logger::logger_instance = new Logger(path,identifier);
    }
}

void Logger::logg(const std::string message){
    Logger::logger_instance->loggMessage(formatMessage(logger_instance->identifier,message));
}

void Logger::loggError(const std::string message){
    Logger::logger_instance->loggMessage(formatError(logger_instance->identifier,message));
    Logger::logger_instance->reset();
}

void Logger::closeLogger(){
    delete Logger::logger_instance;
    Logger::logger_instance=NULL;
}
