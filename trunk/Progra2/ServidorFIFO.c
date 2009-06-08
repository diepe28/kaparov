#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "http/ProtocoloHttp.h"

ServidorHttp * crearFIFO(int puerto, int maxSolicitudes)
{

    ServidorHttp * servidor;
    servidor = crearServidorHttp(puerto, maxSolicitudes);
    return servidor;
}

RespuestaHttp * manejarSolicitudHttp(SolicitudHttp * solicitudHttp)
{
    RespuestaHttp * respuestaHttp;

    respuestaHttp = malloc(sizeof(RespuestaHttp));
    respuestaHttp->versionMayor = 1;
    respuestaHttp->versionMenor = 1;
    respuestaHttp->codigoError = OK;

    respuestaHttp->descripcionError = malloc(3);
    strcpy(respuestaHttp->descripcionError, "OK");

    respuestaHttp->mensaje = malloc(255);
    strcpy(respuestaHttp->mensaje, "<html><head><title>Prueba</title></head><body>Pagina de prueba.</body></html>");
    respuestaHttp->longitudMensaje = strlen(respuestaHttp->mensaje);

    return respuestaHttp;
}

int main()
{
    ServidorHttp * servidor;

    servidor = crearFIFO(8050, 15);

    if (servidor == NULL) printf("No se pudo crear el servidor");

    while (1) {
        if (aceptarSolicitudHttp(servidor, manejarSolicitudHttp) != 0 )
            printf("No se pudo aceptar la solicitud");
    }

    finalizarServidorHttp(servidor);
}
