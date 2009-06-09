#include <stdio.h>
#include <stdlib.h>

#include "http/ServidorHttp.h"

#define PUERTO_FIFO 8050
#define MAX_CONEXIONES_ESPERA 10

ServidorHttp * crearFIFO(int puerto, int maxSolicitudes)
{

    ServidorHttp * servidor;
    servidor = crearServidorHttp(puerto, maxSolicitudes);
    return servidor;
}

int main()
{
    ServidorHttp * servidor;

    servidor = crearFIFO(PUERTO_FIFO, MAX_CONEXIONES_ESPERA);

    if (servidor == NULL) {
        printf("No se pudo crear el servidor FIFO\n");
        exit(EXIT_FAILURE);
    }

    printf("Servidor FIFO iniciado, escuchando en el puerto %d\n", PUERTO_FIFO);
    printf("Conexiones en espera permitidas: %d\n", MAX_CONEXIONES_ESPERA);

    while (1) {
        if (aceptarSolicitudHttp(servidor) != EXIT_SUCCESS)
            printf("No se pudo aceptar la solicitud\n");
    }

    finalizarServidorHttp(servidor);
}
