#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "http/ServidorHttp.h"

#define PUERTO_PREFORKED 8054
#define MAX_CONEXIONES_ESPERA 15
#define MAX_THREADS 256

int numProcesos;

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

void finalizarProcesos(int sig)
{
     printf ("Se van a termina el proceso \n");
     kill(0, SIGKILL);
}

int main(int argc, char * argv[])
{

    if (argc != 2)
    {
        printf ("Se espera que reciba como parámetros <N-Processes> \n");
        return 0;
    }

    numProcesos = atoi(argv[1]);

    signal(SIGTERM, finalizarProcesos);
    signal(SIGQUIT, finalizarProcesos);
    signal(SIGINT, finalizarProcesos);

    // Info de los hilos
    //pthread_t * idHilos = malloc(sizeof(pthread_t)*numThreads);


    ServidorHttp * servidor;
    servidor = crearServidorHttp(PUERTO_PREFORKED, MAX_CONEXIONES_ESPERA);


    if (servidor == NULL) {
        printf("No se pudo crear el servidor PreForked\n");
        exit(EXIT_FAILURE);
    }

    printf("Servidor PreForked iniciado, escuchando en el puerto %d\n", PUERTO_PREFORKED);
    printf("Conexiones en espera permitidas: %d\n", MAX_CONEXIONES_ESPERA);

    int i;
    for (i = 0; i < numProcesos; i++) {
	
	int pid = fork();
	if (pid == 0) {
	    printf ("Levantamos un proceso para atender un cliente\n");
		  atenderSolicitudes(servidor);
	  printf ("El proceso ya atendio clientea\n");
	  exit(0);
	}

	else if (pid == -1)
	  printf ("No puedo crear el proceso\n");


/*int error = pthread_create(&idHilos[i], NULL, &atenderSolicitudes, (void *)servidor);
	if (error != 0)
	  printf ("No puedo crear el hilo\n");*/
       
    }



      //espero que los hijos terminen
      while (1) {}

      //espero que los hilos terminen
      /*for(i=0; i<numThreads; i++){
	 pthread_join(idHilos[i], NULL);
      }*/

    finalizarServidorHttp(servidor);
}
