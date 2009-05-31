#ifndef REFERENCIAS_H
#define REFERENCIAS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>   /* various type definitions.            */
#include <sys/ipc.h>     /* general SysV IPC structures          */
#include <sys/shm.h>	 /* semaphore functions and structs.     */
#include <sys/sem.h>	 /* shared memory functions and structs. */

#define MY_ENCODING "ISO-8859-1"
#define PROGRAMADA 1
#define DEMANDADA 2

#define PROCESAR_LOGIN 1
#define PROCESAR_AGREGARDINERO 2
#define PROCESAR_FACTURACION 3
#define PROCESAR_INVENTARIO 4
#define PROCESAR_BUSQUEDA 5
#define PROCESAR_VIDEO 6
#define PROCESAR_HORARIO 7
#define PROCESAR_NUEVO_PASS 8
#define PROCESAR_PROGRAMACION 9

#define LARGO_CLIENTE 30
#define LARGO_PELICULA 40

#define PRECIO_LOGIN 100
#define PRECIO_AGREGARDINERO 200
#define PRECIO_FACTURACION 300
#define PRECIO_INVENTARIO 400
#define PRECIO_BUSQUEDA 500
#define PRECIO_VIDEO 600
#define PRECIO_HORARIO 700
#define PRECIO_PROGRAMACION 800

#endif
