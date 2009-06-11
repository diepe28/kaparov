#include "cliente_socket_functions.c"
#include "Estadisticos.c"


int QUE_HACER = 0;

char* hostName;
char* archi;
int fd=0; /*Id del socket para la conexion*/
short puertoVideo=8059; //desde el 8050
int hilos=0;
int procesos=0;

void mostrarEstadisticas();

void *llamarN (void* params);

int main (int argc, char *argv[])
{
   /* int i;
    for (i = 1; i<6;i++)
    {
	printf ("%s\n", argv[i]);
    }*/
      printf ("Prueba\n");
	if (argc != 6)
	{
        printf ("Se espera que reciba como parámetros <maquina> <puerto> <archivo> <N-Threads> <Nciclos>\n");
        return 0;
	}

   else
    {
      printf ("Se van a asignar valores\n");

        hostName = argv[1];
        puertoVideo = atoi(argv[2]);
        archi = argv[3];
        hilos = atoi(argv[4]);
        procesos = atoi(argv[5]);

	printf ("Se asignan valores, puerto vale: %d\n", puertoVideo);

        paramCliente pc;
        pc.nProcesos = procesos;
        pc.archivo = archi;
	pc.host = hostName;
	pc.puertoArchivo = puertoVideo;

        pthread_t *hilosCliente = malloc(sizeof(pthread_t)*hilos);

          int i;
          for(i=0;i<hilos;i++)
          {
	      printf ("levantamos un hilo \n");
              pthread_create(&hilosCliente[i], NULL, llamarN, &pc);
          }
          for (i = 0; i<hilos;i++)
              pthread_join(hilosCliente[i], NULL);

	//sleep (10);
	pthread_mutex_lock (&esperaRecibir);
        mostrarEstadisticas();
	pthread_mutex_unlock (&esperaRecibir);
    }

/*
#ifdef ENABLE_NLS
  	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
  	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  	textdomain (GETTEXT_PACKAGE);
#endif
*/

/*
  	gtk_set_locale ();
  	gtk_init (&argc, &argv);

  	window1 = create_window1 ();
  	gtk_widget_show (window1);

  	gtk_main ();
*/

 	return 0;
}

void *llamarN (void* params)
{
    int iter = 0;
    paramCliente *PC = (paramCliente *) params;
    while (iter < PC->nProcesos)
    {
	int numBytes = 0;
	while (numBytes == 0)
	{
	  long int inicioEspera = getTimeMil();
	  numBytes = solicitarDocumentoPorHttp (PC->archivo, PC->puertoArchivo,  PC->host);
	  printf ("En el cliente, numbytes vale : %d\n", numBytes);
	  long int finEspera = getTimeMil();
	  pthread_mutex_lock (&esperaRecibir);
	  cantidadDeBytesRecibidos += numBytes;
	  tiempoEsperaEnSerAtendido+=finEspera-inicioEspera;
	  pthread_mutex_unlock (&esperaRecibir);
	}
        iter++;
	pthread_mutex_lock (&esperaRecibir);
	cantidadDeVecesAtendido++;
	pthread_mutex_unlock (&esperaRecibir);
    }
    //return NULL;
}

void mostrarEstadisticas()
{

     long int megas = (cantidadDeBytesRecibidos/1024)/1024;
     long int kilobytes = cantidadDeBytesRecibidos/1024;
    long int segundos = tiempoEsperaEnSerAtendido/1000;
  
    printf ("/********************************************\n");
    printf ("***************ESTADISTICAS******************\n");
    printf ("/********************************************\n");
    printf ("** Tiempo total de espera en ser atendido: %ld segundos\n", tiempoEsperaEnSerAtendido/1000);
    printf ("** Cantidad de veces que las peticiones estuvieron sin respuesta inmediata: %ld\n", cantidadDeVecesQueEsIgnorado);
    printf ("** Promedio del tiempo que se tarda en atender las solicitudes: %ld\n", (tiempoEsperaEnSerAtendido/(hilos*procesos)));
    printf ("** Total de bytes recibidos: %ld\n", cantidadDeBytesRecibidos);
    printf ("** Total de Kb recibidos: %ld\n", kilobytes);
    printf ("** Total de Mb recibidos: %ld\n", megas);
    printf ("** Relacion bytes/milisegundos: %ld\n", (cantidadDeBytesRecibidos/tiempoEsperaEnSerAtendido));
    printf ("** Relacion Kbytes/segundos: %ld\n", (kilobytes/segundos));
    printf ("** Relacion Mb/segundos: %ld\n", (megas/segundos));
    printf ("*********************************************\n");

  
}


