#ifndef PROTOCOLO_HTTP_H
#define PROTOCOLO_HTTP_H

typedef struct {
    int idSocket;
} ServidorHttp;

typedef enum {
    OPTIONS, GET, HEAD, POST, PUT, DELETE, TRACE, CONNECT
} MetodoHttp;

typedef struct {
    MetodoHttp metodo;
    char * url;
    int versionMayor;
    int versionMenor;
} SolicitudHttp;

typedef enum {
    OK, NOT_FOUND
} CodigoErrorHttp;

typedef struct {
    int versionMayor;
    int versionMenor;
    CodigoErrorHttp codigoError;
    char * descripcionError;

    int longitudMensaje;
    char * mensaje;

    void * documento;

} RespuestaHttp;

typedef RespuestaHttp * (*ManejadorHttp)(SolicitudHttp *);

ServidorHttp * crearServidorHttp(short puerto, int maxSolicitudes);
void finalizarServidorHttp(ServidorHttp * servidor);
int aceptarSolicitudHttp(ServidorHttp * servidor, ManejadorHttp manejador);

char * solicitudHttpABytes (SolicitudHttp * solicitudHttp, int tamSolicitud);

#endif // PROTOCOLO_HTTP_H
