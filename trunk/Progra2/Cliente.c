#include <stdio.h>
#include <stdlib.h>

#include "http/ClienteHttp.h"

#define PUERTO_SERVIDOR 8050

static const char * NomServidor = "localhost";
static const char * UrlDocumento = "index.html";

int main()
{

    int numBytes;

    numBytes = solicitarDocumentoPorHttp(UrlDocumento, PUERTO_SERVIDOR, NomServidor);
    if (numBytes < 0)
        printf("Error: no se pudo obtener el documento\n");
    else
        printf("Bytes obtenidos: %d\n", numBytes);

}
