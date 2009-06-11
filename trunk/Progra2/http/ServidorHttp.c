#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "ServidorHttp.h"

#define TAM_BUFFER 4096
#define TAM_NOM_DOCUMENTO 1024



static const char * CarpertaDocs = "docs";
static const char * DocumentoInicial = "index.html";

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

char * obtenerNomDocumento(char * bytes, int tamBytes, char * nomDocumento, int tamNomDocumento)
{
    int tamLinea;
    int indice;

    char * metodoHttp;
    char * url;
    char * versionHttp;

    // Recorrer secuencia de bytes hasta encontrar CR LF
    // TODO: reconocer espacios lineales y comprimirlos como espacios simples
    indice = 0;
    while (indice < tamBytes) {
        if (bytes[indice] == '\x0D' && (indice + 1 < tamBytes && bytes[indice + 1] == '\x0A'))
            break;
        indice++;
    }

    // Sustituir fin de linea por (0)
    tamLinea = indice;
    if (tamLinea >= tamBytes) return NULL;
    bytes[tamLinea] = 0;

    // Extraer partes de la primera linea
    metodoHttp = strtok(bytes, " ");
    if (metodoHttp != NULL) url = strtok(NULL, " ");
    if (url != NULL) versionHttp = strtok(NULL, " ");
    if (url == NULL) return NULL;

    // Convertir URL a nombre de documento : CarpetaDocs/url
    memset(nomDocumento, 0, tamNomDocumento);
    strcpy(nomDocumento, CarpertaDocs);

    // Agregar "/" si no esta al inicio del url
    if (strlen(url) > 0 && url[0] != '/') {
        strcat(nomDocumento, "/");
    }

    strcat(nomDocumento, url);

    // Agregar documento por defecto si no se indica uno en el url
    if (strlen(nomDocumento) > 0 && nomDocumento[strlen(nomDocumento) - 1] == '/') {
        strcat(nomDocumento, DocumentoInicial);
    }

    return nomDocumento;
}

int aceptarSolicitudHttp(ServidorHttp * servidor)
{
    char buffer[TAM_BUFFER];
    char nomDocumento[TAM_NOM_DOCUMENTO];

    int idSocket;
    struct sockaddr_in dirSocket;
    socklen_t tamDirSocket;
    int numBytes;

    char * docSolicitado;
    FILE * documento;

    // Aceptar conexion del cliente
    tamDirSocket = sizeof(struct sockaddr_in);
    idSocket = accept(servidor->idSocket, (struct sockaddr *)&dirSocket, &tamDirSocket);

    if (idSocket < 0) return EXIT_FAILURE;

    // Recibir solicitud del cliente
    memset(buffer, 0, sizeof buffer);
    numBytes = recv(idSocket, buffer, sizeof buffer, 0);
    if (numBytes <= 0) {
        close(idSocket);
        return EXIT_FAILURE;
    }

    // Extraer url
    docSolicitado = obtenerNomDocumento(buffer, numBytes, nomDocumento, TAM_NOM_DOCUMENTO);

    // Ignorar resto de la solicitud si la hay
    while ((numBytes = recv(idSocket, buffer, sizeof buffer, MSG_DONTWAIT)) > 0) {
        if (numBytes < sizeof buffer) break;
    }

    // Linea de respuesta para el cliente
    memset(buffer, 0, sizeof buffer);
    strcpy(buffer, "HTTP/1.1 ");

    // Buscar y leer archivo
    documento = fopen(docSolicitado, "r");
    if (documento == NULL) {
        // Documento no encontrado
        strcat(buffer, "404 Not Found");
    } else {
        // Documento encontrado
        strcat(buffer, "200 OK");
    }

    // Devolver encabezado
    strcat(buffer, "\x0D\x0A\x0D\x0A");
    numBytes = strlen(buffer);
    if (send(idSocket, buffer, numBytes, 0) != numBytes) {
        close(idSocket);
        return EXIT_FAILURE;
    }

    if (documento == NULL) {
        // Devolver mensaje de archivo no encontrado
        strcpy(buffer,
            "<html>"
            "<head><title>Proyecto 2 SOA</title></head>"
            "<body>"
            "<h1>Documento no encontrado</h1>"
            "<p>El documento solicitado no se ha encontrado.</p>"
            "</body>"
            "</html>");
        numBytes = strlen(buffer);
        send(idSocket, buffer, numBytes, 0);
    } else {
        // Devolver datos del archivo
        while (!feof(documento)) {
            if ((numBytes = fread(buffer, sizeof(char), TAM_BUFFER, documento)) > 0) {
                send(idSocket, buffer, numBytes, 0);
            }
        }
        fclose(documento);
    }

    // Cerrar socket
    close(idSocket);

    return EXIT_SUCCESS;
}


int aceptarSolicitud (ServidorHttp * servidor)
{
    printf ("EStoy aceptando solicitud\n");

    int idSocket;
    struct sockaddr_in dirSocket;
    socklen_t tamDirSocket;
    
        // Aceptar conexion del cliente
    tamDirSocket = sizeof(struct sockaddr_in);
    idSocket = accept(servidor->idSocket, (struct sockaddr *)&dirSocket, &tamDirSocket);

    if (idSocket < 0) return -1;
  
    return idSocket;
}

void * enviarHTTP (void * idSocketParam)
{
    // Recibir solicitud del cliente
    char buffer[TAM_BUFFER];
    char nomDocumento[TAM_NOM_DOCUMENTO];    

    printf ("EStoy enviando HHTP\n");

    int idSocket = *((int *)idSocketParam);
    int numBytes;

    char * docSolicitado;
    FILE * documento;

    memset(buffer, 0, sizeof buffer);
    numBytes = recv(idSocket, buffer, sizeof buffer, 0);
    if (numBytes <= 0) {
        close(idSocket);
        return NULL;
    }

    // Extraer url
    docSolicitado = obtenerNomDocumento(buffer, numBytes, nomDocumento, TAM_NOM_DOCUMENTO);

    printf ("Doc solicitado: %s\n", docSolicitado);

    // Ignorar resto de la solicitud si la hay
    while ((numBytes = recv(idSocket, buffer, sizeof buffer, MSG_DONTWAIT)) > 0) {
        if (numBytes < sizeof buffer) break;
    }

    // Linea de respuesta para el cliente
    memset(buffer, 0, sizeof buffer);
    strcpy(buffer, "HTTP/1.1 ");

    // Buscar y leer archivo
    documento = fopen(docSolicitado, "r");
    if (documento == NULL) {
        // Documento no encontrado
        strcat(buffer, "404 Not Found");
    } else {
        // Documento encontrado
        strcat(buffer, "200 OK");
    }

    printf ("Archivo  arhivo\n");

    // Devolver encabezado
    strcat(buffer, "\x0D\x0A\x0D\x0A");
    numBytes = strlen(buffer);
    if (send(idSocket, buffer, numBytes, 0) != numBytes) {
        close(idSocket);
        return NULL;
    }

    printf ("Encabezado devuelto\n");

    if (documento == NULL) {
        // Devolver mensaje de archivo no encontrado
        strcpy(buffer,
            "<html>"
            "<head><title>Proyecto 2 SOA</title></head>"
            "<body>"
            "<h1>Documento no encontrado</h1>"
            "<p>El documento solicitado no se ha encontrado.</p>"
            "</body>"
            "</html>");
        numBytes = strlen(buffer);
        send(idSocket, buffer, numBytes, 0);
    } else {
        // Devolver datos del archivo
        while (!feof(documento)) {
            if ((numBytes = fread(buffer, sizeof(char), TAM_BUFFER, documento)) > 0) {
		printf ("bytes leidos del archivo:%d\n", numBytes);
	    
                numBytes = send(idSocket, buffer, numBytes, 0);
		if (numBytes == -1 || numBytes < TAM_BUFFER) {
		  printf ("send fallo\n");
		  break;
		}
		printf ("bytes enviados al socket:%d\n", numBytes);
            }
	    if (ferror(documento)){
	      break;
	      printf ("Salida con error de documento\n");
	    }
        }
	printf ("Me dispongo a cerrar el documento\n");
        fclose(documento);
    }

    printf ("Doc devuelto\n");

    // Cerrar socket
    close(idSocket);

    //return EXIT_SUCCESS;
}

