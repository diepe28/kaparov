#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "http/ServidorHttp.h"

#define PUERTO_PRETHREADED 8052
#define MAX_CONEXIONES_ESPERA 5
#define MAX_THREADS 256

int numThreads;

void * atenderSolicitudes(void * param)
{

    ServidorHttp * servidor = (ServidorHttp*) param;
    while (1) {
	int idSocket = aceptarSolicitud(servidor);
	printf ("Nos llega un socket id = %d\n", idSocket);
        if (idSocket == -1)
            printf("No se pudo aceptar la solicitud\n");
        else
	{
		//pthread_t idHilo=(pthread_t)malloc(sizeof(pthread_t));
		//printf ("Levantamos un hilo para atender un cliente\n");
		//int error = pthread_create(&idHilo, NULL, &enviarHTTP, (void *)&idSocket);
		//if (error != 0)
		//  printf ("No puedo crear el hilo\n");
	      enviarHTTP(&idSocket);
	}

    }

}

int main(int argc, char * argv[])
{

    if (argc != 2)
    {
        printf ("Se espera que reciba como parámetros <N-Threads> \n");
        return 0;
    }

    numThreads = atoi(argv[1]);

    // Info de los hilos
    pthread_t * idHilos = malloc(sizeof(pthread_t)*numThreads);


    ServidorHttp * servidor;
    servidor = crearServidorHttp(PUERTO_PRETHREADED, MAX_CONEXIONES_ESPERA);


    if (servidor == NULL) {
        printf("No se pudo crear el servidor PreThreaded\n");
        exit(EXIT_FAILURE);
    }

    printf("Servidor Prethreaded iniciado, escuchando en el puerto %d\n", PUERTO_PRETHREADED);
    printf("Conexiones en espera permitidas: %d\n", MAX_CONEXIONES_ESPERA);

    int i;
    for (i = 0; i < numThreads; i++) {
	printf ("Levantamos un hilo para atender un cliente\n");
	int error = pthread_create(&idHilos[i], NULL, &atenderSolicitudes, (void *)servidor);
	if (error != 0)
	  printf ("No puedo crear el hilo\n");
       
    }

      //espero que los hilos terminen
      for(i=0; i<numThreads; i++){
	 pthread_join(idHilos[i], NULL);
      }

    finalizarServidorHttp(servidor);
}
