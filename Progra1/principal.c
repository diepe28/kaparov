#include <stdio.h>
#include <stdlib.h>
#include "hilos.h"
#include "Taylor.h"
#include "Variables.h"

int main(int argc, char * argv[])
{

    printf("Proyecto 1\n");

	inicializarVariables ((argc > 1 ? argv[1] : NULL));

    /*CANT_TRABAJO[0] = 75000000L;
    CANT_TRABAJO[2] = 75000000L;

    CANT_TIQUETES[0] = 20;
    CANT_TIQUETES[2] = 20;

	CANT_TIQUETES[1] = CANT_TIQUETES[0];
	CANT_TRABAJO[1] = CANT_TRABAJO[0];
	*/

    printf("Num hilos: %d, Modo: %s, Quantum o factor: %d \n",
        NUM_HILOS, (modoActual == Expropiativo ? "Expropiativo" : "NoExpropiativo"), QUANTUM );

	thread_init();
    ejecutar_scheduler();
	thread_destroy();
	finalizarVariables ();
    return 0;
}


