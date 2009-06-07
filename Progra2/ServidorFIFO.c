#include <stdio.h>

#include "http/ProtocoloHttp.h"


ServidorHttp * crearFIFO(int puerto, int maxSolicitudes)
{

    ServidorHttp * servidor;
    servidor = crearServidorHttp(puerto, maxSolicitudes);
    return servidor;
}


int main()
{
    ServidorHttp * servidor;

    servidor = crearFIFO(8050, 15);

    if (servidor == NULL) printf("No se pudo crear el servidor");

    if (aceptarSolicitudHttp(servidor, 0) != 0 )
        printf("No se pudo aceptar la solicitud");

    finalizarServidorHttp(servidor);
}
