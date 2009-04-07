#include <stdio.h>
#include "hilos.h"
#include "Taylor.h"
#include "Variables.h"

int main()
{
	//printf ("Valor obtenido %.30lle\n", sumarN(10000));	
    printf("Proyecto 1\n");
	inicializarVariables ();
	thread_init();
    ejecutar_scheduler();
	thread_destroy();
	finalizarVariables ();
    return 0;
}
