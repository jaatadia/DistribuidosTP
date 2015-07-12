/*
 * Funcion: Open activo que hace un cliente
 *	utilizando protocolo TCP
 * Descripcion:
 *	Inicializar un socket, con los datos del servidor
 *	Alocar el socket y establecer la comunicacion (conectarse)
 *
 * Sintaxis: sockfd = tcpopact [servidor, [port]]
 *	servidor: 	nombre oficial del host server
 *	port:		nro de port
 *	devuelve: >0: el socket, si se conecto al servidor
 *                -2: si no existe el nombre del servidor
 *		  -1: si hubo error en la conexion y se debe consultar errno.
 */

#ifndef TCPOPACT
#define TCPOPACT

#include	"inet.h"

int tcpopact(const char *server, int puerto); 

int tcpopact(in_addr server, int puerto);

#endif