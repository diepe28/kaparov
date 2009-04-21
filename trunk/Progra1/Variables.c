#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Variables.h"

#define CANTIDAD_VARIABLES_ARCHIVO 53

int NUM_HILOS;
long * CANT_TRABAJO;
long * CANT_TIQUETES;
long * CANT_TIQUETES_ACUM;
long * ITERACION_ACTUAL;
long double * RESPUESTAS;
int QUANTUM;
ModoTrabajo modoActual;

void cargarArchivo (const char* file);
int isDigit (char c);

void inicializarVariables (const char * file)
{
	int i ;

	cargarArchivo ((file == NULL ? "input" : file));

	for (i = 0; i<NUM_HILOS; i++)
	{
		RESPUESTAS [i] = 0.00;
		CANT_TIQUETES_ACUM [i]  =1;
		ITERACION_ACTUAL [i] = 0;
	}
}

void finalizarVariables ()
{
	free (CANT_TIQUETES);
	free (CANT_TRABAJO);
	free (ITERACION_ACTUAL);

//	free (RESPUESTAS);
}

void cargarHilosModo (long *valores)
{
	int i = 0;
	int j = 0;
	NUM_HILOS = valores[i++];
	modoActual = valores[i++];
	QUANTUM = valores[i++];

	CANT_TIQUETES = malloc (sizeof(long)*NUM_HILOS);
	CANT_TRABAJO = malloc (sizeof(long)*NUM_HILOS);
	CANT_TIQUETES_ACUM = malloc (sizeof(long)*NUM_HILOS);
	RESPUESTAS = malloc (sizeof(long double)*NUM_HILOS);
	ITERACION_ACTUAL = malloc (sizeof(long)*NUM_HILOS);

	for(j= 0; j<NUM_HILOS;j++) {
		CANT_TIQUETES[j] = valores[i++];
	}
	for (j=0; j<NUM_HILOS;j++) {
		CANT_TRABAJO[j] = 50*valores[i++];
	}

}

void cargarArchivo (const char* file)
{
  FILE *fp;
  char temp;

  long variables [CANTIDAD_VARIABLES_ARCHIVO];
  int numVariables = 0;
  int i;

  fp = fopen(file, "r");

  temp = getc (fp);
  while (temp != EOF)
  {
	int pos;
    char s[11];
    for (pos = 0; isDigit(temp) && pos <10 ;pos++)
	  {
	    s[pos] = temp;
        temp = getc (fp);
	  }

	  if (pos != 0)
	  {
		  char num[pos];
		  for (i=0;i<pos;i++)
			  num[i] = s[i];
		  variables[numVariables] = atoi(num);
		  numVariables++;
	  }

	  if (numVariables == CANTIDAD_VARIABLES_ARCHIVO)
		  break;
	  temp = getc(fp);
  }
  fclose (fp);

  cargarHilosModo (variables);
  return;
}

int isDigit (char c)
{
  return c >= '0' && c<='9';
}


