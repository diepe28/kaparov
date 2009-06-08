#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Documentos.h"

#define TAM_NOM_DOCUMENTO 1024
#define TAM_BUFFER 1024

static const char * CarpertaDocs = "docs";
static const char * DocumentoInicial = "index.html";

RespuestaHttp * buscarDocumento(SolicitudHttp * solicitudHttp)
{
    RespuestaHttp * respuestaHttp;

    char nomDocumento[TAM_NOM_DOCUMENTO];
    FILE * documento;

    char buffer[TAM_BUFFER];
    int numChars;

    char * tmpMensaje;
    int longitudTmpMensaje;

    // Verificar que el metodo sea GET
    if (solicitudHttp == NULL || solicitudHttp->metodo != GET)
        return NULL;

    // Crear respuesta
    respuestaHttp = malloc(sizeof(RespuestaHttp));
    respuestaHttp->versionMayor = 1;
    respuestaHttp->versionMenor = 1;
    respuestaHttp->mensaje = NULL;
    respuestaHttp->longitudMensaje = 0;

    // Convertir URL a nombre de archivo
    memset(nomDocumento, 0, sizeof nomDocumento);
    strcpy(nomDocumento, CarpertaDocs);

    if (strlen(solicitudHttp->url) > 0 && solicitudHttp->url[0] != '/') {
        strcat(nomDocumento, "/");
    }

    strcat(nomDocumento, solicitudHttp->url);

    if (strlen(nomDocumento) > 0 && nomDocumento[strlen(nomDocumento) - 1] == '/') {
        strcat(nomDocumento, DocumentoInicial);
    }

    printf("Archivo solicitado: %s\n", nomDocumento);

    // Buscar y leer archivo
    documento = fopen(nomDocumento, "r");

    if (documento == NULL) {
        // Archivo no encontrado
        respuestaHttp->codigoError = NOT_FOUND;
        respuestaHttp->descripcionError = malloc(10);
        strcpy(respuestaHttp->descripcionError, "Not Found");
        printf("Documento no encontrado\n");

    } else {
        while (!feof(documento)) {
            if ((numChars = fread(buffer, sizeof(char), TAM_BUFFER, documento)) > 0) {

                printf("Chars leidos: %d\n", numChars);

                if (respuestaHttp->mensaje != NULL) {
                    tmpMensaje = malloc(respuestaHttp->longitudMensaje);
                    longitudTmpMensaje = respuestaHttp->longitudMensaje;
                    memcpy(tmpMensaje, respuestaHttp->mensaje, respuestaHttp->longitudMensaje);
                }

                respuestaHttp->longitudMensaje += numChars;
                free(respuestaHttp->mensaje);
                printf("Nueva longitud de mensaje:%d\n", respuestaHttp->longitudMensaje);

                respuestaHttp->mensaje = malloc(respuestaHttp->longitudMensaje);
                printf("Memoria asignada para solicitud\n");

                if (tmpMensaje != NULL)
                    memcpy(respuestaHttp->mensaje, tmpMensaje, longitudTmpMensaje);

                memcpy(respuestaHttp->mensaje + longitudTmpMensaje, buffer, numChars);
                printf("Memoria copiada\n");
            }
        }
        fclose(documento);

        // Codigo de la respuesta
        respuestaHttp->codigoError = OK;
        respuestaHttp->descripcionError = malloc(3);
        strcpy(respuestaHttp->descripcionError, "OK");
        printf("Documento leido exitosamente\n");

    }

    printf("Largo de la respuesta: %d\n", respuestaHttp->longitudMensaje);

    return respuestaHttp;
}

