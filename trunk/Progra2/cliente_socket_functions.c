#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/timeb.h>
#include <time.h>


/**
  *  Estadísticos
  */
long int tiempoEsperaEnSerAtendido = 0;
long int cantidadDeVecesQueEsIgnorado = 0;
long int tiempoEnRecibirArchivo = 0;

/**
  *  Semáforos
  */
pthread_mutex_t esperaRecibir;
pthread_mutex_t esperaConexion;

typedef struct paramCli
{
    int idsocket;
    int nProcesos;
    int puertoArchivo;
    char* archivo;
} paramCliente;

long int getTimeMil ()
{
    struct timeb tp;
    long int instante = tp.time*1000;  //tomo los segundos y los multiplico por mil
    instante+=tp.millitm;              //le sumo los milisegundos a los ya calculados
    return instante;                   //retorno el tiempo del sistema en milisegundos
}

//void recibirArchivo(int idsocket, int puerto, char* archivo);

void recibirArchivo (int idsocket, int puertoArchivo, char* archivo)
{

	//struct sockaddr_in clientArchivo;/* para la información de la dirección del cliente */
	/*int sin_sizeArchivo;
	int fdVideo2; // los ficheros descriptores */
	//void *bufferfinal=(void*)malloc(MAX_BUFFER);
	/*int numbytes;
	sin_sizeVideo=sizeof(struct sockaddr_in);
	int as=fork();
	if(as==0){
		char* comand=(char*)malloc(sizeof(char)*200);
		sprintf(comand,"xine -pqg --no-logo --no-splash tcp://localhost:%d", params.puertoArchivo);
		system(comand);
		free(comand);
		exit(0);
	}
	if ((fdVideo2 = accept(fdVid,(struct sockaddr *)&clientVideo,(socklen_t *)&sin_sizeVideo))==-1){
		printf("VIDEO: Error en accept()\n");
		return NULL;
	}
	else{
		memset(bufferfinal, 0, MAX_BUFFER);
		while(1){
			numbytes=RecibirM(idsocket,bufferfinal,MAX_BUFFER);
			if(memcmp(bufferfinal,"fin",3)!=0){
				printf("a\n");
				send(fdVideo2,bufferfinal,numbytes,0);
				printf("b\n");
				memset(bufferfinal, 0, MAX_BUFFER);
				SendM(idsocket,"ok",3);
			}
			else{
				SendM(idsocket,"ok",3);
				printf("fin pelicula\n");
				close(fdVideo2);
				break;
			}
		}
	}
	*/
    return;
}


void *llamarN (void* params)
{
    int iter = 0;
    paramCliente *PC = (paramCliente *) params;
    while (iter<PC->nProcesos)
    {
        long int inicioEspera = getTimeMil();
        recibirArchivo (PC->idsocket, PC->puertoArchivo, PC->archivo);
        long int finEspera = getTimeMil();
        pthread_mutex_lock (&esperaConexion);
        tiempoEsperaEnSerAtendido+=finEspera-inicioEspera;
        pthread_mutex_unlock (&esperaConexion);
        iter++;
    }
    return NULL;
}

