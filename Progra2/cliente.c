#include "cliente_socket_functions.c"


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

	if (argc != 5)
	{
        printf ("Se espera que reciba como parámetros <maquina> <puerto> <archivo> <N-Threads> <Nciclos>");
        return 0;
	}

    else
    {
        hostName = argv[0];
        puertoVideo = atoi(argv[1]);
        archi = argv[2];
        hilos = atoi(argv[3]);
        procesos = atoi(argv[4]);

        paramCliente pc;
        pc.nProcesos = procesos;
        pc.archivo = archi;

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


