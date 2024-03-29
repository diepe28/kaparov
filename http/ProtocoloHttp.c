#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "ProtocoloHttp.h"

#define TAM_BUFFER 1024

ServidorHttp * crearServidorHttp(short puerto, int maxSolicitudes)
{

    int idSocket;
    struct sockaddr_in dirSocket;

    // Crear el socket
	if ((idSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		return NULL;

    // Parametros para enlazar el socket al puerto indicado y al nodo local
    memset(&dirSocket, 0, sizeof(struct sockaddr_in));
    dirSocket.sin_family = AF_INET;
    dirSocket.sin_port = htons(puerto);
    dirSocket.sin_addr.s_addr = htonl(INADDR_ANY);

    // Enlazar el socket
    if (bind(idSocket, (struct sockaddr *)&dirSocket, sizeof(struct sockaddr_in)) < 0)
        return NULL;

    // Establer el socket para el maximo numero indicado de conexiones en espera
    if (listen(idSocket, maxSolicitudes) < 0)
        return NULL;

    // Asignar memoria para almacenar los datos del servidor: id del socket
    ServidorHttp * servidor = malloc(sizeof(ServidorHttp));
    servidor->idSocket = idSocket;

    return servidor;
}

void finalizarServidorHttp(ServidorHttp * servidor)
{
    // Cerrar el socket
    close(servidor->idSocket);

    // Liberar memoria del socket
    free(servidor);
}

char * extraerLineaHttp(char * bytes, int tamBytes, int posicion)
{
    char * linea;
    int tamLinea;
    int indice;

    // Recorrer secuencia de bytes hasta encontrar CR LF
    // TODO: reconocer espacios lineales y comprimirlos como espacios simples
    indice = posicion;
    while (indice < tamBytes) {
        if (bytes[indice] == '\x0D' && (indice + 1 < tamBytes && bytes[indice + 1] == '\x0A'))
            break;
        indice++;
    }

    // Asignar espacio para la linea : tamLinea + un espacio de fin de string (0)
    tamLinea = indice - posicion + 1;
    linea = malloc(tamLinea);
    if (linea == NULL) return NULL;

    // Copiar bytes a linea terminada en 0
    memset(linea, 0, tamLinea);
    if (tamLinea > 1)
        memcpy(linea, bytes + posicion, tamLinea - 1);

    return linea;
}

SolicitudHttp * bytesASolicitudHttp(char * bytesSolicitud, int tamSolicitud)
{
    SolicitudHttp * solicitudHttp;
    char * linea;
    char * metodoHttp;
    char * url;
    char * versionHttp;

    // Extraer linea de la solicitud HTTP
    linea = extraerLineaHttp(bytesSolicitud, tamSolicitud, 0);

    // Extraer metodo, url, y version de la solicitud HTTP
    if (linea != NULL) {
        metodoHttp = strtok(linea, " ");
        if (metodoHttp != NULL) url = strtok(NULL, " ");
        if (url != NULL) versionHttp = strtok(NULL, " ");

        solicitudHttp = malloc(sizeof(SolicitudHttp));
        if (strcmp("OPTIONS",metodoHttp) == 0)
            solicitudHttp->metodo = OPTIONS;
        else if (strcmp("GET",metodoHttp) == 0)
            solicitudHttp->metodo = GET;
        else if (strcmp("HEAD",metodoHttp) == 0)
            solicitudHttp->metodo = HEAD;
        else if (strcmp("POST",metodoHttp) == 0)
            solicitudHttp->metodo = POST;
        else if (strcmp("PUT",metodoHttp) == 0)
            solicitudHttp->metodo = PUT;
        else if (strcmp("DELETE",metodoHttp) == 0)
            solicitudHttp->metodo = DELETE;
        else if (strcmp("TRACE",metodoHttp) == 0)
            solicitudHttp->metodo = TRACE;
        else if (strcmp("CONNECT",metodoHttp) == 0)
            solicitudHttp->metodo = CONNECT;

        solicitudHttp->url = malloc(strlen(url + 1));
        strcpy(solicitudHttp->url, url);

        if (strncmp("HTTP/", versionHttp, 5) == 0) {
            char * versionMayor = strtok(versionHttp + 5, ".");
            char * versionMenor = strtok(NULL, ".");
            solicitudHttp->versionMayor = atoi(versionMayor);
            solicitudHttp->versionMenor = atoi(versionMenor);
        }

            // TODO: seguir con los demas campos de la solicitud
    }

    free(linea);

    return solicitudHttp;
}

char * respuestaHttpABytes(RespuestaHttp * respuestaHttp, int * tamBytes)
{
    char * encabezado[TAM_BUFFER];
    char * bytesRespuesta;

    char versionMayor[8];
    char versionMenor[8];
    int tamHilera, tamEncabezado;

    memset(encabezado, 0, sizeof encabezado);
    tamEncabezado = 0;
    strcpy(encabezado, "HTTP/");
    tamEncabezado =+ 5;

    tamHilera = sprintf(versionMayor, "%d", respuestaHttp->versionMayor);
    strncpy(encabezado + tamEncabezado)

    tamHilera = sprintf(versionMenor, "%d", respuestaHttp->versionMenor);

    return bytesRespuesta;
}

void liberarSolicitudHttp(SolicitudHttp * solicitudHttp)
{
    free(solicitudHttp->url);
    free(solicitudHttp);
}

void liberarRespuestaHttp(RespuestaHttp * respuestaHttp)
{
    free(respuestaHttp->descripcionError);
    free(respuestaHttp->mensaje);
    free(respuestaHttp);
}

int aceptarSolicitudHttp(ServidorHttp * servidor, ManejadorHttp manejador)
{

    int idSocket;
    struct sockaddr_in dirSocket;
    socklen_t tamDirSocket;
    int opcionesSocket;

    int numBytes;
    char buffer[TAM_BUFFER];

    int tamSolicitud;
    char * bytesSolicitud;

    int tamRespuesta;
    char * bytesRespuesta;

    SolicitudHttp * solicitudHttp;
    RespuestaHttp * respuestaHttp;

    // Aceptar conexion del cliente
    tamDirSocket = sizeof(struct sockaddr_in);
    idSocket = accept(servidor->idSocket, (struct sockaddr *)&dirSocket, &tamDirSocket);

    if (idSocket < 0) return EXIT_FAILURE;

    // Recibir solicitud del cliente
    memset(buffer, 0, sizeof buffer);
    bytesSolicitud = NULL;
    tamSolicitud = 0;

    opcionesSocket = 0;
    while ((numBytes = recv(idSocket, buffer, sizeof buffer, opcionesSocket)) > 0) {
        bytesSolicitud = realloc(bytesSolicitud, tamSolicitud + numBytes);
        if (bytesSolicitud != NULL) {
            memcpy(bytesSolicitud + tamSolicitud, buffer, numBytes);
            tamSolicitud += numBytes;
        }

        if (numBytes < sizeof buffer) break;
        opcionesSocket = MSG_DONTWAIT;
    }

    // Llamar al manejador HTTP con la solicitud recibida y responder al cliente
    if (tamSolicitud > 0) {

        // Convertir bytes a estructura para la solicitud HTTP
        solicitudHttp = bytesASolicitudHttp(bytesSolicitud, tamSolicitud);

        // Invocar manejador y responder al cliente
        if (solicitudHttp != NULL && manejador != NULL) {

            // Invocar manejador HTTP
            respuestaHttp = manejador(solicitudHttp);
            if (respuestaHttp != NULL) {

                // Responder al socket cliente
                bytesRespuesta = respuestaHttpABytes(respuestaHttp, &tamRespuesta);
                send(idSocket, bytesRespuesta, tamRespuesta, 0);

                // Liberar memoria respueta HTTP
                liberarRespuestaHttp(respuestaHttp);
            }
        }

        // Liberar memoria solicitud HTTP
        liberarSolicitudHttp(solicitudHttp);
    }

    // Liberar memoria de los bytes de la solicitud
    free(bytesSolicitud);

    // Cerrar socket
    close(idSocket);

    return EXIT_SUCCESS;
}

