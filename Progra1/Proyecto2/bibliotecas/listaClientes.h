#ifndef LISTA_CLIENTES_H
#define LISTA_CLIENTES_H

#include "referencias.h"

//definicion del nodo
typedef struct ClientesRegistro {
    char cliente_nombre[LARGO_CLIENTE];
    char cliente_password[LARGO_CLIENTE];
    int cliente_id;
    int cliente_dinero;
	int cliente_total_factura;
	struct ClientesRegistro * historial;
	//estos 2 valores solo pueden ser accesados desde el nodo historial
	char *nombre_transaccion;
	int precio_transaccion;
    struct ClientesRegistro * sig;
} ClientesRegistro;

typedef struct listaClientes {
    ClientesRegistro * primero;
    ClientesRegistro * final;
} listaClientes;

void Cliente_inicializar(listaClientes*);
void Cliente_cambiarPassword(listaClientes*,char*,char*);
ClientesRegistro* Cliente_crearNuevoRegistro(char*,int);
void Cliente_escribir_nodo(ClientesRegistro *);
void Cliente_imprimirlineas(char* linea);
ClientesRegistro* Cliente_devuelveCliente(listaClientes *,char*);
int Cliente_validarCliente(listaClientes *,char*,char*);
void Cliente_insertar_Cliente(listaClientes * pl, int cliente_id, char *cliente_nombre, char *passwd, int cliente_dinero);
int Cliente_aumentarDinero(listaClientes *pl,char* buscado, int cliente_dinero);
int Cliente_disminuirDinero(listaClientes *pl,char* buscado, int cliente_dinero);
void Cliente_ver_lista(listaClientes * pl);
void Cliente_borrar_lista(listaClientes * pl);
void Cliente_cargarArchivo(listaClientes *);
void Cliente_guardarLista(listaClientes *);
void Cliente_recargarArchivo(listaClientes* pl);
void Cliente_GuardarArchivo(listaClientes* pl);

#endif
