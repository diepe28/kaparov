#ifndef VAR_H
#define VAR_H

typedef enum  {Expropiativo, NoExpropiativo } ModoTrabajo;
extern int NUM_HILOS;
extern long * CANT_TRABAJO;
extern int * CANT_TIQUETES;
extern int * CANT_TIQUETES_ACUM;
extern long * ITERACION_ACTUAL;
extern long double * RESPUESTAS;
extern int QUANTUM;
extern ModoTrabajo modoActual;

void inicializarVariables (const char *);
void finalizarVariables ();

#endif
