#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "http/ServidorHttp.h"

#define PUERTO_THREADED 8051
#define MAX_CONEXIONES_ESPERA 0
#define MAX_THREADS 256

ServidorHttp * crearThreaded(int puerto, int maxSolicitudes)
{
    ServidorHttp * servidor;
    servidor = crearServidorHttp(puerto, maxSolicitudes);
    return servidor;
}

int main()
{
    ServidorHttp * servidor;

    servidor = crearThreaded(PUERTO_THREADED, 0);

    if (servidor == NULL) {
        printf("No se pudo crear el servidor Threaded\n");
        exit(EXIT_FAILURE);
    }

    printf("Servidor Threaded iniciado, escuchando en el puerto %d\n", PUERTO_THREADED);
    printf("Conexiones en espera permitidas: %d\n", MAX_CONEXIONES_ESPERA);

    while (1) {
	int idSocket = aceptarSolicitud(servidor);
	printf ("Nos llega un socket id = %d\n", idSocket);
        if (idSocket == -1)
            printf("No se pudo aceptar la solicitud\n");
        else
	{
		pthread_t idHilo=(pthread_t)malloc(sizeof(pthread_t));
		printf ("Levantamos un hilo para atender un cliente\n");
		int error = pthread_create(&idHilo, NULL, &enviarHTTP, (void *)&idSocket);
		if (error != 0)
		  printf ("No puedo crear el hilo\n");
	}

    }

    finalizarServidorHttp(servidor);
}
