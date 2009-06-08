#include <stdio.h>

#include "Documentos.h"

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

    if (servidor == 0) printf("No se pudo crear el servidor");

    while (1) {
        if (aceptarSolicitudHttp(servidor, buscarDocumento) != 0 )
            printf("No se pudo aceptar la solicitud");
    }

    finalizarServidorHttp(servidor);
}
