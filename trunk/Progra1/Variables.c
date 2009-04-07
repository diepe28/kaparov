#include <stdlib.h>
#include "Variables.h"
#include <stdio.h>
#include <string.h>

#define CANTIDAD_VARIABLES_ARCHIVO 53

int NUM_HILOS;
int * CANT_TRABAJO;
int * CANT_TIQUETES;
int * CANT_TIQUETES_ACUM;
int * ITERACION_ACTUAL;
long double * RESPUESTAS;
int QUANTUM;
ModoTrabajo modoActual;

void cargarArchivo (char* file);
int isDigit (char c);


void inicializarVariables ()
{
	int i ;
//	NUM_HILOS = 5;
	cargarArchivo ("input");
			
	for (i = 0; i<NUM_HILOS; i++)
	{
		RESPUESTAS [i] = 0.00;
		CANT_TIQUETES_ACUM [i]  =1;
		ITERACION_ACTUAL [i] = 0;
	}
	//QUANTUM = 1;
	/*
	for (i = 0; i<NUM_HILOS;i++)
	{
		CANT_TIQUETES [i] = (i+1)*10;
		CANT_TRABAJO [i] = 75000000;
	}	
	
	//modoActual = NoExpropiativo;
	//modoActual = Expropiativo;*/
}

void finalizarVariables ()
{
	free (CANT_TIQUETES);
	free (CANT_TRABAJO);
	free (ITERACION_ACTUAL);
	
//	free (RESPUESTAS);
}

void cargarHilosModo (int *valores)
{
	int i = 0;
	int j = 0;
	NUM_HILOS = valores[i++];
	modoActual = valores[i++];
	QUANTUM = valores[i++];
	
	CANT_TIQUETES = malloc (sizeof(int)*NUM_HILOS);
	CANT_TRABAJO = malloc (sizeof(int)*NUM_HILOS);
	CANT_TIQUETES_ACUM = malloc (sizeof(int)*NUM_HILOS);
	RESPUESTAS = malloc (sizeof(long double)*NUM_HILOS);
	ITERACION_ACTUAL = malloc (sizeof(int)*NUM_HILOS);
	
	for(j= 0; j<NUM_HILOS;j++)
		CANT_TIQUETES[j] = valores[i++];
	for (j=0; j<NUM_HILOS;j++)
		CANT_TRABAJO[j] = 50*valores[i++];
	
	
}

void cargarArchivo (char* file)
{
  FILE *fp;
  fp = fopen(file, "r");

  int  variables [CANTIDAD_VARIABLES_ARCHIVO];
  int numVariables = 0;
	int i;

  char temp = getc (fp);
  while (temp != EOF)
  {
	  int pos;
    char s[11];
    for (pos = 0; isDigit(temp) && pos <10 ;pos++)
	  {
		//  printf ("%c", temp);
	    s[pos] = temp;
		  temp = getc (fp);
	  }
	  	//printf(" VALOR DE POS: %d\n", pos);
	  if (pos != 0)
	  {
		  char num[pos];
		  for (i=0;i<pos;i++)
			  num[i] = s[i];
		  variables[numVariables] = atoi(num);
		 // printf("%d\n", variables[numVariables]);
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


