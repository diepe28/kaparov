#include <stdlib.h>
#include "Variables.h"

int NUM_HILOS;
int * CANT_TRABAJO;
int * CANT_TIQUETES;
int * CANT_TIQUETES_ACUM;
int * ITERACION_ACTUAL;
long double * RESPUESTAS;
int QUANTUM;
ModoTrabajo modoActual;

void inicializarVariables ()
{
	int i ;
	NUM_HILOS = 5;
	
	CANT_TIQUETES = malloc (sizeof(int)*NUM_HILOS);
	CANT_TRABAJO = malloc (sizeof(int)*NUM_HILOS);
	CANT_TIQUETES_ACUM = malloc (sizeof(int)*NUM_HILOS);
	RESPUESTAS = malloc (sizeof(long double)*NUM_HILOS);
	ITERACION_ACTUAL = malloc (sizeof(int)*NUM_HILOS);
	for (i = 0; i<NUM_HILOS; i++)
	{
		RESPUESTAS [i] = 0.00;
		CANT_TIQUETES_ACUM [i]  =1;
		ITERACION_ACTUAL [i] = 0;
	}
	QUANTUM = 1;
	
	for (i = 0; i<NUM_HILOS;i++)
	{
		CANT_TIQUETES [i] = (i+1)*10;
		CANT_TRABAJO [i] = 750000*i;
	}	
	
	//modoActual = NoExpropiativo;
	modoActual = NoExpropiativo;
}

void finalizarVariables ()
{
	free (CANT_TIQUETES);
	free (CANT_TRABAJO);
	free (ITERACION_ACTUAL);
	
//	free (RESPUESTAS);
}



