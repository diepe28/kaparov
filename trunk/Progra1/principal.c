#include <stdlib.h>
#include "hilos.h"
#include "Taylor.h"
#include "Variables.h"
#include "interfaz.h"

int main(int argc, char * argv[])
{

	inicializarVariables ((argc > 1 ? argv[1] : NULL));

	thread_init();

	inicializarDespliegue(&argc, &argv);
    desplegarThreads(ejecutar_scheduler);

	thread_destroy();

	finalizarVariables ();

    return EXIT_SUCCESS;
}


