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

#include	"inet.h"

int tcpopact(const char *server, int puerto){
 int    sockfd;			     /* socket de la conexion */
 struct sockaddr_in	serv_addr;
 struct hostent		*ptr_server; /*puntero a dir del server(gethostbyname)*/
 /*			
  *			ALOCAR EL SOCKET Y COMUNICARSE CON EL SERVER
  *
  * 	Completar la estructura del socket "serv_addr" con la direccion del
  * 	server y port al que nos queremos connectar.
  * 
  * 	Paso 1: Borrar la estructura (poner en cero)
  */ 
	bzero((char *) &serv_addr, sizeof(serv_addr));
	
 /*	Paso 2:  Inicializar familia de direcciones (protocolo IP) 
  */
	serv_addr.sin_family      = AF_INET;

 /*
  *	Paso 3: Cargar port en el socket:
  *	        Convertir Host-TO-Network-Short integer 
  */
	serv_addr.sin_port        = htons(puerto); /* valido */
  /*
  *	Paso 4: Cargar direccion del server en el socket
  *	        Convertir nombre del host en su direccion 
  */
  ptr_server = gethostbyname(server); 
  if ( ((char *)ptr_server) == NULL )
	{		/* no existe: aviso y terminar */
	return (-2);
	}
 else
	memcpy(&serv_addr.sin_addr, ptr_server->h_addr, ptr_server->h_length);
/*
 * 	Abrir como un socket de TCP (Internet stream socket).
 */
 if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
	return (-1); /* error en la creacion del socket */
	}
/*
 * 	Connectarse al server.
 */
 if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
	return (-1); /* error en el connect */
	}
 /*
  *		se conecto exitosamente
  */
  return(sockfd);
} 

int tcpopact(in_addr server, int puerto){
 int    sockfd;			     /* socket de la conexion */
 struct sockaddr_in	serv_addr;
 /*			
  *			ALOCAR EL SOCKET Y COMUNICARSE CON EL SERVER
  *
  * 	Completar la estructura del socket "serv_addr" con la direccion del
  * 	server y port al que nos queremos connectar.
  * 
  * 	Paso 1: Borrar la estructura (poner en cero)
  */ 
	bzero((char *) &serv_addr, sizeof(serv_addr));
	
 /*	Paso 2:  Inicializar familia de direcciones (protocolo IP) 
  */
	serv_addr.sin_family      = AF_INET;

 /*
  *	Paso 3: Cargar port en el socket:
  *	        Convertir Host-TO-Network-Short integer 
  */
	serv_addr.sin_port        = htons(puerto); /* valido */
  /*
  *	Paso 4: Cargar direccion del server en el socket
  *	        Convertir nombre del host en su direccion 
  */
        serv_addr.sin_addr=server;
/*
 * 	Abrir como un socket de TCP (Internet stream socket).
 */
 if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
	return (-1); /* error en la creacion del socket */
	}
/*
 * 	Connectarse al server.
 */
 if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
	return (-1); /* error en el connect */
	}
 /*
  *		se conecto exitosamente
  */
  return(sockfd);
} 