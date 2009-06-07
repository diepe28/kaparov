#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libxml/xmlwriter.h>
#include <libxml/xmlreader.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
//grep -R -B3 -A4 "libxml" ./* 

void* recibirVideo (int idsocket,int fdVid,int portVideo)
{
	struct sockaddr_in clientVideo;/* para la información de la dirección del cliente */
	int sin_sizeVideo;
	int fdVideo2; /* los ficheros descriptores */
	void *bufferfinal=(void*)malloc(MAX_BUFFER);
	int numbytes;
	sin_sizeVideo=sizeof(struct sockaddr_in);
	int as=fork();
	if(as==0){
		char* comand=(char*)malloc(sizeof(char)*200);
		sprintf(comand,"xine -pqg --no-logo --no-splash tcp://localhost:%d",portVideo);
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
	return NULL;
}
