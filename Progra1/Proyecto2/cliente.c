#include "bibliotecas/referencias.c"
#include "bibliotecas/listaPeliculas.c"
#include "bibliotecas/encriptacion.c"
#include "muxine.c"
int QUE_HACER = 0;
listaPeliculas* Peliculas;
char hostName[100];
int fd=0; /*Id del socket para la conexion*/
int puertoVideo=8075;
int fdVideo;
int fdVideo2; /* los ficheros descriptores */

#include "../socket_functions.c"
#include "client_socket_functions.c"
#include "bibliotecas/XMLCliente.c"
#include "interfaz/interface.c"


int main (int argc, char *argv[])
{
	
	Peliculas =(listaPeliculas *)malloc (sizeof (listaPeliculas));
	Peliculas->primero = NULL;	/* Al comenzar NO hay nodos */
	Peliculas->final = NULL;	/* Al comenzar NO hay nodos */
	/* para la información de la dirección del servidor */
	
	
	if(gethostname(hostName,sizeof(char)*100)){
		exit(0);
	}
	
#ifdef ENABLE_NLS
  	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
  	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  	textdomain (GETTEXT_PACKAGE);
#endif

  	gtk_set_locale ();
  	gtk_init (&argc, &argv);

  	window1 = create_window1 ();
  	gtk_widget_show (window1);

  	gtk_main ();
 	return 0;
}
