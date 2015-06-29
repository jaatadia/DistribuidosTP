/* 
 * File:   Parser.h
 * Author: knoppix
 *
 * Created on March 28, 2015, 1:39 PM
 */

#ifndef PARSER_H
#define	PARSER_H

#include <string>

#define FILE_NOT_FOUND -1
#define PARAM_NOT_FOUND -2

#define BOOL_TRUE 1
#define BOOL_FALSE 0

class Parser {
public:
    Parser();
    static void setPath(const char* path);
    static int getBoolParam(std::string key);
    static int getIntParam(std::string key);
    virtual ~Parser();
    
private:
    static std::string path;
};

#endif	/* PARSER_H */

