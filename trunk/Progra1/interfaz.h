#ifndef INTERFAZ_H
#define INTERFAZ_H

void inicializarDespliegue(int *argc, char ***argv);
void desplegarThreads(void (*funcion)());
void actualizarBarra(int numBarra, double fraccion);

#endif // INTERFAZ_H
