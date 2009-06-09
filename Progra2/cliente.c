#include "cliente_socket_functions.c"
//#include "http/ProtocoloHttp.c"


int QUE_HACER = 0;

char* hostName;
char* archi;
int fd=0; /*Id del socket para la conexion*/
int puertoVideo=8059; //desde el 8050
int hilos=0;
int procesos=0;

void mostrarEstadisticas();

int main (int argc, char *argv[])
{
   /* int i;
    for (i = 1; i<6;i++)
    {
	printf ("%s\n", argv[i]);
    }*/
      printf ("Prueba\n");
	//if (argc != 6)
	{
        printf ("Se espera que reciba como parámetros <maquina> <puerto> <archivo> <N-Threads> <Nciclos>\n");
        return 0;
	}

    //else
    {
      printf ("Se van a asignar valores");

        hostName = argv[1];
        puertoVideo = atoi(argv[2]);
        archi = argv[3];
        hilos = atoi(argv[4]);
        procesos = atoi(argv[5]);

	printf ("Se asignan valores");

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

        mostrarEstadisticas();
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

void mostrarEstadisticas()
{
}


