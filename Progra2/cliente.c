#include "cliente_socket_functions.c"
#include "Estadísticos.c"


int QUE_HACER = 0;

char* hostName;
char* archi;
int fd=0; /*Id del socket para la conexion*/
int puertoVideo=8059; //desde el 8050
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

	printf ("Se asignan valores\n");

        paramCliente pc;
        pc.nProcesos = procesos;
        pc.archivo = archi;
	pc.host = hostName;

        pthread_t *hilosCliente = malloc(sizeof(pthread_t)*hilos);

          int i;
          for(i=0;i<hilos;i++)
          {
              pthread_create(&hilosCliente[i], NULL, llamarN, &pc);
          }
          for (i = 0; i<hilos;i++)
              pthread_join(hilosCliente[i], NULL);

	sleep (10);
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
	  long int finEspera = getTimeMil();
	  pthread_mutex_lock (&esperaConexion);
	  tiempoEsperaEnSerAtendido+=finEspera-inicioEspera;
	  pthread_mutex_unlock (&esperaConexion);
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
  
    printf ("%ld tiempo de espera, %ld cantidad de iteraciones\n", tiempoEsperaEnSerAtendido, cantidadDeVecesAtendido);
  
}


