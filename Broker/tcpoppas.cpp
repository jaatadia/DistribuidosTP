/*
 * Funcion: Open pasivo usando protocolo TCP.
 * Descripcion:	
 *	Inicializar y crear el socket.
 *	Binding con el SO.
 *	Definir longitud de cola de espera de clientes.
 * Sintaxis: sockfd = tcpoppas [port]
 *	port: 	port sobre el cual atiende este servidor
 *   Devuelve: > 0: el socket, si la operacion fue exitosa
 *             < 0: si hubo un error (ver errno)
 *   
 */

#include "inet.h"

int tcpoppas(int puerto)
{
// char mostrar[120];      /* mensajes en la pantalla */ 
 int	sockfd;		/* socket que sirve como template */ 
 struct sockaddr_in	serv_addr;
 /*			INICIALIZACION DEL SERVER

  * 		Inicializar la estructura del socket:
  * bzero(): borrar la estructura
  * htonl(): Host-TO-Network Long integer (Byte order)
  * htons(): Host-TO-Network Short integer (Byte order)     
  */

 bzero((char *) &serv_addr, sizeof(serv_addr));
 serv_addr.sin_family      = AF_INET; /* familia protocolos TCP/IP */
 serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);    /* cualquier cliente */ 
 serv_addr.sin_port = htons((u_short)puerto); /* port en formato red */
 /*
  * 		Crear un socket para TCP (un Internet stream socket).
  */
 if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
	 return (-1); /* error no se puede abrir el stream socket */
	 }
	 //sprintf (mostrar,"server: socket creado %d\n", sockfd);
         //write(fileno(stdout), mostrar, strlen(mostrar));
 /*
  * 		Vincular el socket con la direccion local 
  */
 if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
 	 return (-1); /* error: no se puede hacer el bind */
 	 }
	 //sprintf (mostrar, "server: se hizo el bind\n");
          //write(fileno(stdout), mostrar, strlen(mostrar));
  /* 
   * 		Definir la cola de espera = hasta 5 clientes 
   */
 listen(sockfd, 5);
 //sprintf (mostrar, "server: se hizo el listen con el socket %d\n", sockfd);
 //write(fileno(stdout), mostrar, strlen(mostrar));

 return (sockfd);
}
