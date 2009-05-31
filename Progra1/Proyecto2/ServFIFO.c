#include "bibliotecas/referencias.h"
#include "bibliotecas/listaClientes.h"

#define BACKLOG 1 /* El número de conexiones permitidas */
#define MAX_BUFFER 5000

listaClientes *Clientes;
char *directorioArchivos;
char HostName[100];
int fin_contar=0;
int puertos [5] = {8070,8071,8072,8073,8074};

pthread_mutex_t mutexModificacionClientes;

/*Para los servers*/
int N_workers_P_threaded;
int fdThreaded;
int fdPreThreaded;
int fdFifo;
int fdForked;
int fdPreForked;
pthread_t idHiloTiempo1;
pthread_t idHiloTiempo2;
pthread_t idHiloSecuenciaEscape;
pthread_t idHiloServer1;
pthread_t idHiloServer2;
pthread_t idHiloServer3;
pthread_t idHiloServer4;
pthread_t idHiloServer5;


void* cargarFIFO();
char* recibir (int idsocket);

int main()
{
    int error;

	Clientes = (listaClientes *)malloc (sizeof (listaClientes));
	Clientes->primero = NULL;	/* Al comenzar NO hay nodos */
	Clientes->final = NULL;	/* Al comenzar NO hay nodos */
	Cliente_cargarArchivo(Clientes);

    error = pthread_create(&idHiloServer1, NULL, &cargarFIFO, NULL);
		if (error != 0){
			printf ("No puedo crear el hilo\n");
		}

    	return 0;
}

//Rutina que recibe de un socket todo un mensaje completo
char* recibir (int idsocket)
{
	char buffer[MAX_BUFFER+1];
	char *bufferfinal,*bufferTemp="";
	int numbytes,sihaystring=0,largo,largobuffer;
	bufferfinal=(char*)malloc(MAX_BUFFER+1);
	memset(bufferfinal, 0, MAX_BUFFER+1);
	if ((numbytes=recv(idsocket,bufferfinal,MAX_BUFFER,0)) == -1){
		printf("Error en recv() \n");
		return "";
	}
	else{
		sihaystring=1;
		bufferfinal[numbytes]='\0';
		while((numbytes=recv(idsocket,buffer,MAX_BUFFER, MSG_DONTWAIT))>0){
			largobuffer=numbytes;
			buffer[numbytes]='\0';
			largo=strlen(bufferfinal)+1;
			bufferTemp=(char*)malloc(largo);
			strcpy(bufferTemp,bufferfinal);
			if(largo!=1)
				free(bufferfinal);
			bufferfinal=(char*)malloc(largo+largobuffer+1);
			strcpy(bufferfinal,bufferTemp);
			strncat(bufferfinal,buffer,largobuffer);
			memset(buffer, 0, MAX_BUFFER+1);
			free(bufferTemp);
		}
		bufferfinal[strlen(bufferfinal)]='\0';
		return bufferfinal;
	}
}


//Rutina de carga del servidor en Modo Fifo
void* cargarFIFO(){
	int puerto=puertos[0];
   	int fd2; /* los ficheros descriptores */
	struct sockaddr_in server;/* para la información de la dirección del servidor */
	struct sockaddr_in client;/* para la información de la dirección del cliente */
	int sin_size;

	if(gethostname(HostName,sizeof(char)*100)){
		return NULL;
	}

	/* A continuación la llamada a socket() */
	if ((fdFifo=socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
		printf("FIFO: Error en socket()\n");
		return NULL;
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(puerto);
	server.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server.sin_zero),8);

	if(bind(fdFifo,(struct sockaddr*)&server,sizeof(struct sockaddr))==-1){
		printf("FIFO: Error en bind() \n");
		return NULL;
	}

	printf("Servidor FIFO: Esperando clientes en el puerto: %d ...\n",puerto);
	if(listen(fdFifo,BACKLOG) == -1) {
		printf("FIFO: Error en listen()\n");
		return NULL;
	}
	while(1){
		sin_size=sizeof(struct sockaddr_in);
		if ((fd2 = accept(fdFifo,(struct sockaddr *)&client,(socklen_t *)&sin_size))==-1) {
			printf("FIFO: Error en accept()\n");
			break;
		}
		else{
			printf("Se obtuvo una conexión desde %s\n",inet_ntoa(client.sin_addr));
			char *msg="";
			msg=recibir(fd2);
			if(strlen(msg)>0){
				//EnviarRespuesta(fd2,msg,"FIFO",Clientes,Peliculas,Horario);
				free(msg);
			}
			close(fd2);
		}
	}
	return NULL;
}
