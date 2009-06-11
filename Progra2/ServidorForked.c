#include <stdio.h>
#include <stdlib.h>

#include "http/ServidorHttp.h"

#define PUERTO_FORKED 8053
#define MAX_CONEXIONES_ESPERA 10
#define MAX_THREADS 256

int main()
{
    ServidorHttp * servidor;

    servidor = crearServidorHttp(PUERTO_FORKED, MAX_CONEXIONES_ESPERA);

    if (servidor == NULL) {
        printf("No se pudo crear el servidor Forked\n");
        exit(EXIT_FAILURE);
    }

    printf("Servidor Forked iniciado, escuchando en el puerto %d\n", PUERTO_FORKED);
    printf("Conexiones en espera permitidas: %d\n", MAX_CONEXIONES_ESPERA);

    while (1) {
	int idSocket = aceptarSolicitud(servidor);
	printf ("Nos llega un socket id = %d\n", idSocket);
        if (idSocket == -1)
            printf("No se pudo aceptar la solicitud\n");
        else
	{
	        int pid = fork();
		if (pid == 0) {
		  printf ("Levantamos un proceso para atender un cliente\n");
		  enviarHTTP(&idSocket);
		  printf ("El proceso ya atendio un cliente\n");
		  exit(0);
		}

		else if (pid == -1)
		  printf ("No puedo crear el proceso\n");
	}

    }

    finalizarServidorHttp(servidor);
}
