#include <stdlib.h>
#include "hilos.h"
#include "Taylor.h"
#include "Variables.h"
#include "interfaz.h"

int main(int argc, char * argv[])
{

    // Pasar argumentos de linea de comandos a GTK
	inicializarDespliegue(&argc, &argv);

    // Leer variables de archivo de configuracion
	inicializarVariables ((argc > 1 ? argv[1] : NULL));

    // Asignar memoria para los hilos
	thread_init();

    // Iniciar el recorrido de los hilos
    desplegarThreads(ejecutar_scheduler);

    // Liberar memoria para los hilos
	thread_destroy();

    // Liberar memoria para las variables
	finalizarVariables ();

    return EXIT_SUCCESS;
}


