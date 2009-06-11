#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "ClienteHttp.h"


#define TAM_BUFFER 4096



int solicitarDocumentoPorHttp(const char * urlDocumento, short puerto, const char * nomServidor)
{

    char buffer[TAM_BUFFER];

    printf ("Valores de llamada: archivo: %s, puerto: %d, host: %s\n", urlDocumento, puerto, nomServidor );
    int bytesLeidos;

    int totalBytes = 0;

    char strPuerto[5];
    struct addrinfo dirBuscada, *dirEncontradas, *dirServidor;

    int idSocket;
    int numBytes, opcionesSocket;

    // Parametros para direccion del servidor
    memset(&dirBuscada, 0, sizeof(struct addrinfo));
    dirBuscada.ai_family = AF_INET;
    dirBuscada.ai_socktype = SOCK_STREAM;

    // Buscar direccion del servidor
    sprintf(strPuerto, "%d", puerto);
    if (getaddrinfo(nomServidor, strPuerto, &dirBuscada, &dirEncontradas) != 0) return -1;

    // Establecer conexion con alguna de las direcciones encontradas
    for (dirServidor = dirEncontradas; dirServidor != NULL; dirServidor = dirServidor->ai_next) {

        // Crear el socket
        if ((idSocket = socket(dirServidor->ai_family,
                dirServidor->ai_socktype, dirServidor->ai_protocol)) < 0)
            continue;

        // Conectarse al socket
        if (connect(idSocket, dirServidor->ai_addr, dirServidor->ai_addrlen) < 0)
            continue;

        break; // Conexion establecida
    }

    if (dirServidor == NULL) {
        // No se pudo establecer conexion con el servidor : retornar error
        bytesLeidos = -1;

    } else {

        // Linea de solicitud para el servidor
        memset(buffer, 0, sizeof buffer);
        strcpy(buffer, "GET ");
        strcat(buffer, urlDocumento);
        strcat(buffer, " HTTP/1.1\x0D\x0A\x0D\x0A");

        // Enviar solicitud
        numBytes = strlen(buffer);
        if (send(idSocket, buffer, numBytes, 0) == numBytes) {

	    if (shutdown(idSocket, SHUT_WR) == -1) printf ("Error en shutdown\n");

	    fd_set read_fd_set;

	    FD_ZERO (&read_fd_set);

	    int encabezadoRecibido = 0;
	    do {
	        FD_SET (idSocket, &read_fd_set);
		select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL);
               
		if (!FD_ISSET(idSocket, &read_fd_set)) break;

		numBytes = recv(idSocket, buffer, sizeof buffer, 0);
		totalBytes +=numBytes;

		printf ("Bytes recibidos: %d\n", numBytes );

		if (numBytes == 0 || (numBytes < TAM_BUFFER && encabezadoRecibido)) break;
		encabezadoRecibido = 1;	

	    } while(FD_ISSET(idSocket, &read_fd_set));

	      printf ("Salgo del WHILE: \n");

            // Recibir respuesta
            /*opcionesSocket = 0;
            bytesLeidos = 0;
            while ((numBytes = recv(idSocket, buffer, sizeof buffer, opcionesSocket)) > 0) {
	        printf ("Bytes leidos: %d\n", numBytes );
                bytesLeidos += numBytes;
                if (numBytes < sizeof buffer) break;
                opcionesSocket = MSG_DONTWAIT;
            }*/

        }

        close(idSocket);


    }

    // Liberar direcciones encontradas
    freeaddrinfo(dirEncontradas);

    return totalBytes;
}
