#ifndef SERVIDOR_HTTP_H
#define SERVIDOR_HTTP_H

typedef struct {
    int idSocket;
} ServidorHttp;

ServidorHttp * crearServidorHttp(short puerto, int maxSolicitudes);
void finalizarServidorHttp(ServidorHttp * servidor);
int aceptarSolicitudHttp(ServidorHttp * servidor);

#endif // SERVIDOR_HTTP_H
