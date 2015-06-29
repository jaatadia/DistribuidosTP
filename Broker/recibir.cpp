
/*
  Funcion TcpRecibir()
  Descripcion:
 *      Lee  la cantidad de bytes sobre un descriptor que TCP tiene:
 *		si no se leyeron n bytes
 *			repetir hasta que se hayan leido los n bytes
 *	(Se debe usar esta funcion cuando el descriptor es un stream socket 
 * Sintaxis: recibir(int sockfd, void *datos, size_t nbytes)
 *	sockfd:	descriptor del socket
 *	datos:	puntero al buffer en el cual se leen los bytes 
 *	nbytes:     cantidad de bytes que deben recibirse
 */
#include "inet.h"

int recibir(int sockfd, void *datos, size_t nbytes)
{
	/* Variables. */
	//char mostrar[80];
	int ult_lec;	/* Cantidad de bytes leidos en la ultima lectura. */
	int leidos;		/* Cantidad de bytes leidos. */
    /* Lee  */
	leidos = 0;
	/*
	sprintf(mostrar," recibir, bytes a recibir %d\n", nbytes);
	write(fileno(stdout), mostrar, strlen(mostrar));
	 */
	while(nbytes)
	{
		ult_lec = read(sockfd, ((unsigned char *) datos) + leidos,
				nbytes);
		if (ult_lec < 0) return leidos;    /* error */
		leidos += ult_lec;
		nbytes -= ult_lec;
	}
	/*
	sprintf(mostrar," recibir, bytes recibidos %d\n", leidos);
	write(fileno(stdout), mostrar, strlen(mostrar));
	 */
	/* Devuelve la cantidad de bytes recibidor. */
	return leidos;
}
