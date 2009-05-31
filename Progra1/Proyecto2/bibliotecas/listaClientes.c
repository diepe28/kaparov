#include "listaClientes.h"

int FINarchivo;

/*Lee una linea del archivo*/
char* read_line(FILE *p){
	char *linea=(char*)malloc(sizeof(char)*101);
	int iT=0,i=0;
	char caracter;
	char* buftmp;
	FINarchivo=1;
	while((caracter=getc(p)) != EOF){
		FINarchivo=0;
		if(i==100){
			buftmp=(char*)malloc(sizeof(char)*(strlen(linea)+101));
			strcpy(buftmp,linea);
			free(linea);
			linea=buftmp;
			i=0;
		}
		if(caracter=='\n'){
			linea[iT]='\0';
			break;
		}
		else{
			linea[iT]=caracter;
		}
		i++;
		iT++;
	}
	//caso de llegar al final del archivo
	if(caracter==EOF){
		FINarchivo=1;
		linea[iT]='\0';
	}
	return linea;
}

/*Escribe el nodo en pantalla*/
void Cliente_escribir_nodo (ClientesRegistro * pn){
    printf ("Nombre: %s\n", pn->cliente_nombre);
    printf ("Password: %s\n", pn->cliente_password);
    printf ("Dinero Disponible: %i\n", pn->cliente_dinero);
}

/*Borra la lista y la vuelve a cargar del archivo*/
void Cliente_recargarArchivo(listaClientes* pl)
{
	//pthread_mutex_lock(&mutexModificacionClientes);
    Cliente_borrar_lista(pl);
	Cliente_cargarArchivo(pl);
	//pthread_mutex_unlock(&mutexModificacionClientes);
}

/*Guarda la lista en el archivo*/
void Cliente_GuardarArchivo(listaClientes* pl)
{
	//pthread_mutex_lock(&mutexModificacionClientes);
    Cliente_guardarLista(pl);
	//pthread_mutex_unlock(&mutexModificacionClientes);
}

/*Carga el archivo*/
void Cliente_cargarArchivo(listaClientes* lista){
	FILE *puntero;
	puntero=fopen ("bd/clientes.txt", "r");
	char buff[60],buff2[10];
	int i=0,ibuf=0,ibuf2=0;
	int cliente_id;
	char *cliente_password;
	char *cliente_nombre;
	int cliente_dinero;
	ClientesRegistro *nuevo_nodo_historial,*temp,*nodo;
	char *linea=read_line(puntero),*dinstring;
	while(!FINarchivo){
		cliente_id=atoi(linea);
		free(linea);
		cliente_nombre=read_line(puntero);
		cliente_password=read_line(puntero);
		dinstring=read_line(puntero);
		cliente_dinero=atoi(dinstring);
		free(dinstring);
		Cliente_insertar_Cliente(lista,cliente_id,cliente_nombre, cliente_password,cliente_dinero);
		linea=read_line(puntero);
		if(strcmp(linea,"#")!=0){
			i=0;
			while(linea[i]!='\0'){
				if(linea[i]==';'){
					//ya esta la info del historial
					buff[ibuf]='\0';
					ibuf=0;
					i++;
					ibuf2=0;
					while(linea[i]!='-'){
						buff2[ibuf2]=linea[i];
						ibuf2++;
						i++;
					}
					//ya esta el precio del historial
					nuevo_nodo_historial = (ClientesRegistro *) malloc (sizeof (ClientesRegistro));
					nuevo_nodo_historial->nombre_transaccion=(char*)malloc(sizeof(char)*(strlen(buff)+1));
					strcpy(nuevo_nodo_historial->nombre_transaccion,buff);
					nuevo_nodo_historial->precio_transaccion=atoi(buff2);
					nuevo_nodo_historial->sig=NULL;
					temp = Cliente_devuelveCliente (lista, cliente_nombre);
					temp->cliente_total_factura = temp->cliente_total_factura+atoi(buff2);
					if(temp->historial==NULL){
						temp->historial=nuevo_nodo_historial;
					}
					else{
						nodo=temp->historial;
						while(nodo->sig!=NULL){
							nodo=nodo->sig;
						}
						nodo->sig=nuevo_nodo_historial;
					}
					memset(buff2,0,10);
					memset(buff,0,60);
				}
				else{
					buff[ibuf]=linea[i];
					ibuf++;
				}
				i++;
			}
			linea=read_line(puntero);
		}
		free(linea);
		linea=read_line(puntero);
		free(cliente_nombre);
		free(cliente_password);
	}
    fclose(puntero);
}

/*Guarda la lista*/
void Cliente_guardarLista(listaClientes* lista){
    FILE* archivoClientes;
    archivoClientes = fopen ("bd/clientes.txt", "w");
    ClientesRegistro *temp2=NULL;
	ClientesRegistro *temp = lista->primero;
	ClientesRegistro *temp3=NULL;
	while (temp!=NULL){
		fprintf(archivoClientes,"%d\n",temp->cliente_id);
		fprintf(archivoClientes,"%s\n",temp->cliente_nombre);
        fprintf(archivoClientes,"%s\n",temp->cliente_password);
        fprintf(archivoClientes,"%d\n",temp->cliente_dinero);

		temp2 = Cliente_devuelveCliente (lista, temp->cliente_nombre);

		if(temp2->historial!=NULL){
			temp3=temp2->historial;
			while(temp3!=NULL){
				fprintf(archivoClientes,"%s;",temp3->nombre_transaccion);
				fprintf(archivoClientes,"%d-",temp3->precio_transaccion);
				temp3=temp3->sig;
			}
			fprintf(archivoClientes,"\n#\n");
		}
		else{
			fprintf(archivoClientes,"#\n");
		}
		temp = temp->sig;
	}
    fclose(archivoClientes);
}

/*Borra la lista y sus historiales */
void Cliente_borrar_lista (listaClientes * pl){
	 if (pl->primero != NULL){
        ClientesRegistro *temp=pl->primero, *aux=pl->primero->sig;
		ClientesRegistro *temp2=NULL,*aux2=NULL;
		while(aux!=NULL){
			//borra la lista de historiales
			if(temp->historial!=NULL){
				temp2=temp->historial;
				aux2=temp->historial->sig;
				while(aux2!=NULL){
					free(temp2);
					temp2=aux2;
					aux2=aux2->sig;
				}
				free(temp2);
			}
			free(temp);
			temp=aux;
			aux=aux->sig;
		}
		//borra la lista de historiales
		if(temp->historial!=NULL){
			ClientesRegistro *temp2=temp->historial, *aux2=temp->historial->sig;
			while(aux2!=NULL){
				free(temp2);
				temp2=aux2;
				aux2=aux2->sig;
			}
			free(temp2);
		}
		free(temp);
		pl->primero = NULL;
		pl->final = NULL;
    }
}

/* Insertar nodo al final. */
void Cliente_insertar_Cliente (listaClientes * pl, int cliente_id, char *cliente_nombre,char *passwd, int cliente_dinero){
    //Si no existe se inserta con valores default
    ClientesRegistro *temp;
    temp = (ClientesRegistro *) malloc (sizeof (ClientesRegistro));
    strcpy (temp->cliente_nombre, cliente_nombre);
    strcpy (temp->cliente_password, passwd);
    temp->cliente_id = cliente_id;
    temp->cliente_dinero = cliente_dinero;
	temp->cliente_total_factura = 0;
	temp->historial=NULL;
    if (NULL == pl->primero){
        pl->primero = temp;
        pl->final = temp;
        pl->primero->sig = NULL;
    }
    else{
        pl->final->sig = temp;
        pl->final = temp;		/* El nodo pasa a ser el nuevo fin */
        temp->sig = NULL;		/* No hay un siguiente del último elemento */
    }
}

/*Le agrega un registro (factura) al cliente*/
ClientesRegistro* Cliente_agregarfactura(listaClientes *pl,char* nombre, char *info, int precio){
	Cliente_recargarArchivo(pl);
	ClientesRegistro *nuevo_nodo_historial,*temp;
	nuevo_nodo_historial = (ClientesRegistro *) malloc (sizeof (ClientesRegistro));
    nuevo_nodo_historial->nombre_transaccion=info;
	nuevo_nodo_historial->precio_transaccion=precio;
	nuevo_nodo_historial->sig=NULL;

	temp = Cliente_devuelveCliente (pl, nombre);

	if(temp->historial==NULL){
		temp->historial=nuevo_nodo_historial;
	}
	else{
		ClientesRegistro *nodo=temp->historial;
		while(nodo->sig!=NULL){
			nodo=nodo->sig;
		}
		nodo->sig=nuevo_nodo_historial;
	}

	temp->cliente_total_factura = temp->cliente_total_factura+precio;
	Cliente_GuardarArchivo(pl);
	return temp;
}

/*Aumenta el dinero del cliente*/
int Cliente_aumentarDinero (listaClientes *pl, char *buscado, int cliente_dinero){
	Cliente_recargarArchivo(pl);
    ClientesRegistro *temp;
    temp = Cliente_devuelveCliente (pl, buscado);
    temp->cliente_dinero = temp->cliente_dinero + cliente_dinero;
    //Cliente_escribir_nodo(temp);
	Cliente_GuardarArchivo(pl);
    return 1;
}

/*Cambia el password*/
void Cliente_cambiarPassword (listaClientes *pl, char *buscado, char* password){
	Cliente_recargarArchivo(pl);
    ClientesRegistro *temp;
    temp = Cliente_devuelveCliente (pl, buscado);
    memset((void*)temp->cliente_password,0,30);
	strcpy(temp->cliente_password,password);
	//printf("Cambie Password %s\n",temp->cliente_password);
    //Cliente_escribir_nodo(temp);
  	Cliente_GuardarArchivo(pl);
}

/*Disminuye el dinero del cliente*/
int Cliente_disminuirDinero (listaClientes *pl,char *buscado, int cliente_dinero){
	Cliente_recargarArchivo(pl);
    ClientesRegistro *temp;
    temp = Cliente_devuelveCliente (pl, buscado);
	if ((temp->cliente_dinero - cliente_dinero)<0)//no tiene plata
		return 0;
	else{
	    temp->cliente_dinero = temp->cliente_dinero - cliente_dinero;
		Cliente_GuardarArchivo(pl);
		return 1;
	}
}

/*Devuelve el nodo donde esta ese cliente*/
ClientesRegistro *Cliente_devuelveCliente (listaClientes * pl, char *buscado){
    if (pl->primero != NULL){
        ClientesRegistro *temp;
        temp = pl->primero;
        while (temp != NULL){
            if (!strcmp (temp->cliente_nombre, buscado)){
                return temp;
            }
            else{
                temp = temp->sig;
            }
        }
        return NULL;
    }
    else
        return NULL;
}

/*Valida que el cliente sea valido*/
int Cliente_validarCliente (listaClientes * pl, char *buscado, char* password){
    if (pl->primero != NULL){
        ClientesRegistro *temp;
        temp = pl->primero;
        while (temp != NULL){
            if (strcmp (temp->cliente_nombre, buscado) == 0){
                if (strcmp (temp->cliente_password, password) == 0){
                    return 1;
                }
                else{
                    return 0;
                }
            }
            else{
                temp = temp->sig;
            }
        }
        return 0;
    }
    else
        return 0;
}

/*Imprime la lista*/
void Cliente_ver_lista (listaClientes * pl)
{
    ClientesRegistro *temp;
    if (NULL == pl->primero){
        printf ("No hay clientes\n");
    }
    else{
        temp = pl->primero;
        while (NULL != temp){
            Cliente_escribir_nodo (temp);
            temp = temp->sig;
        }
    }
}

/*Inicializa un Cliente*/
void Cliente_inicializar (listaClientes *lista){
    lista = (listaClientes *)malloc (sizeof (listaClientes));
    lista->primero = NULL;	/* Al comenzar NO hay nodos */
    lista->final = NULL;	/* Al comenzar NO hay nodos */
}
