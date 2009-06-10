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
#include "http/ProtocoloHttp.h"



typedef struct paramCli
{
    int idsocket;
    int nProcesos;
    int puertoArchivo;
    char* archivo;
    char* host;
} paramCliente;

long int getTimeMil ()
{
    struct timeb tp;
    long int instante = tp.time*1000;  //tomo los segundos y los multiplico por mil
    instante+=tp.millitm;              //le sumo los milisegundos a los ya calculados
    return instante;                   //retorno el tiempo del sistema en milisegundos
}

//void recibirArchivo(int idsocket, int puerto, char* archivo);

void recibirArchivo (int idsocket, int puertoArchivo, char* archivo, char* host)
{

	struct sockaddr_in clientArchivo;/* para la información de la dirección del cliente */
	clientArchivo.sin_family = AF_INET;
	//clienteArchivo.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;
	inet_pton(AF_INET, host, &clientArchivo.sin_addr.s_addr);
	clientArchivo.sin_port = puertoArchivo;
	

	int sin_sizeArchivo;
	int fdArc; // los ficheros descriptores */
	int fd;

	char buffer [1024];
	
	SolicitudHttp * shttp = malloc (sizeof (SolicitudHttp));

	shttp->versionMenor = 1;
	shttp->versionMayor = 1;
	shttp->metodo = GET;
	shttp->url = malloc (strlen (archivo));
	strcpy (shttp->url, archivo);

	char* encabezado = solicitudHttpABytes (shttp, 0);

	//void *bufferfinal=(void*)malloc(MAX_BUFFER);
	int numbytes = strlen(encabezado);
	//sin_sizeVideo=sizeof(struct sockaddr_in);
	//int as=fork();
	/*if(as==0){
		char* comand=(char*)malloc(sizeof(char)*200);
		sprintf(comand,"xine -pqg --no-logo --no-splash tcp://localhost:%d", params.puertoArchivo);
		system(comand);
		free(comand);
		exit(0);
	}*/
	if ((fdArc = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("socket error\n");
		return;
	}

	if ((fd = connect(fdArc,(struct sockaddr *)&clientArchivo,sizeof(clientArchivo)))==-1){
		printf("error en connect\n");
		return;
	}
	
	if ((send (fdArc, encabezado, numbytes, 0)) == -1){
		printf ("error en el send\n");
		return;
	}

	if (recv(fdArc, buffer, 1024, 0) == -1) {
               printf ("error en el recv\n");
                exit(1);
        }
	else
	{
	  printf ("%s\n", buffer);
	}
/*
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




