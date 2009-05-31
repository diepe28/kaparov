
int peli_id;
char peli_ruta[5000];
char peli_nombre[LARGO_PELICULA];
int peli_duracion;
int peli_costo;
int peli_extra;
int peli_simultaneos;
float peli_tamano;


//definicion del nodo
typedef struct PeliculasRegistro {
    char peli_nombre[LARGO_PELICULA];
    char *peli_ruta;
    int peli_id;
    int peli_duracion;
    int peli_costo;
    int peli_extra;//porcentaje
	int peli_simultaneos;
	float peli_tamano;
    struct PeliculasRegistro * sig;
} PeliculasRegistro;

typedef struct listaPeliculas {
    PeliculasRegistro * primero;
    PeliculasRegistro * final;
} listaPeliculas;


void Pelicula_inicializar(listaPeliculas*);
PeliculasRegistro* Pelicula_crearNuevoRegistro(char*,int);
void Pelicula_escribir_nodo(PeliculasRegistro *);
void Pelicula_imprimirlineas(char* linea);
PeliculasRegistro* Pelicula_devuelvePelicula(listaPeliculas *,char*);
PeliculasRegistro* Pelicula_devuelveNombrePeliculaPorHorario(listaPeliculas *,int);
int Pelicula_obtenerPrecioPelicula(PeliculasRegistro*, int tipo);
void Pelicula_insertar_Cliente (listaPeliculas * pl, int peli_id, char *peli_nombre,char *peli_ruta, int peli_duracion,int peli_costo, int peli_extra, int peli_simultaneos, float peli_tamano);
void Pelicula_ver_lista(listaPeliculas * pl);
void Pelicula_borrar_lista(listaPeliculas * pl);
void Pelicula_cargarArchivo(listaPeliculas *);
void Pelicula_aumentarSimultaneos(listaPeliculas*,char*);
void Pelicula_disminuirSimultaneos(listaPeliculas*,char*);


void Pelicula_recargarArchivo(listaPeliculas* pl)
{
	//pthread_mutex_lock(&mutexModificacionClientes);
    Pelicula_borrar_lista(pl);
	Pelicula_cargarArchivo(pl);
	//pthread_mutex_unlock(&mutexModificacionClientes);
}

/*Guarda la lista*/
void Pelicula_guardarLista(listaPeliculas* lista){
    FILE* archivoPeliculas;
    archivoPeliculas = fopen ("bd/peliculas.txt", "w");
    PeliculasRegistro *temp = lista->primero;
	while (temp!=NULL){
		fprintf(archivoPeliculas,"%d\n",temp->peli_id);
		fprintf(archivoPeliculas,"%s\n",temp->peli_nombre);
		fprintf(archivoPeliculas,"%s\n",temp->peli_ruta);
		fprintf(archivoPeliculas,"%d\n",temp->peli_duracion);
		fprintf(archivoPeliculas,"%d\n",temp->peli_costo);
		fprintf(archivoPeliculas,"%d\n",temp->peli_extra);
		fprintf(archivoPeliculas,"%d\n",temp->peli_simultaneos);
		fprintf(archivoPeliculas,"%f\n",temp->peli_tamano);
		fprintf(archivoPeliculas,"--------------\n");
		temp = temp->sig;
	}
    fclose(archivoPeliculas);
}


void Pelicula_aumentarSimultaneos(listaPeliculas* pl,  char* buscado)
{
	Pelicula_recargarArchivo(pl);
	PeliculasRegistro* nodo = Pelicula_devuelvePelicula(pl,buscado);
	nodo->peli_simultaneos++;
	Pelicula_guardarLista(pl);
}

void Pelicula_disminuirSimultaneos(listaPeliculas* pl, char* buscado)
{
	Pelicula_recargarArchivo(pl);
	PeliculasRegistro* nodo = Pelicula_devuelvePelicula(pl,buscado);
	printf("Nodo: %s\n",nodo->peli_nombre);
	nodo->peli_simultaneos--;
	Pelicula_guardarLista(pl);
}

/*Imprime un nodo en pantalla*/
void Pelicula_escribir_nodo (PeliculasRegistro * pn){
    printf ("Nombre: %s\n", pn->peli_nombre);
    printf ("Ruta: %s\n", pn->peli_ruta);
    printf ("Duracion: %d,\n",pn->peli_duracion);
    printf ("Costo Basico: %d\n", pn->peli_costo);
    float x = (pn->peli_costo*pn->peli_extra/100)+pn->peli_costo;
    printf ("Costo Demanda: %f\n",x);
	printf ("# simultaneos: %d\n",pn->peli_simultaneos);
	printf("%f\n",pn->peli_tamano);
	printf ("-------------\n");
}

/*Inserta el nodo en la lista de peliculas*/
void Pelicula_insertar_Cliente (listaPeliculas * pl, int peli_id, char *peli_nombre,char *peli_ruta, int peli_duracion,int peli_costo, int peli_extra, int peli_simultaneos, float peli_tamano){
    //Si no existe se inserta con valores default
    PeliculasRegistro *temp;
    temp = (PeliculasRegistro *) malloc (sizeof (PeliculasRegistro));
    strcpy (temp->peli_nombre, peli_nombre);
	temp->peli_ruta = (char*)malloc(sizeof(char)*strlen(peli_ruta));
    strcpy (temp->peli_ruta, peli_ruta);
    temp->peli_id = peli_id;
    temp->peli_duracion = peli_duracion;
    temp->peli_costo = peli_costo;
    temp->peli_extra = peli_extra;
	temp->peli_simultaneos = peli_simultaneos;
	temp->peli_tamano = peli_tamano;
    if (NULL == pl->primero)
    {
        pl->primero = temp;
        pl->final = temp;
        pl->primero->sig = NULL;
    }
    else
    {
        pl->final->sig = temp;
        pl->final = temp;		/* El nodo pasa a ser el nuevo fin */
        temp->sig = NULL;		/* No hay un siguiente del último elemento */
    }
}

/*Devuelve la pelicula*/
PeliculasRegistro * Pelicula_devuelvePelicula (listaPeliculas * pl, char *buscado){
    if (pl->primero != NULL)
    {
        PeliculasRegistro *temp;
        temp = pl->primero;
        while (temp != NULL)
        {
            if (strcmp (temp->peli_nombre, buscado) == 0)
            {
                return temp;
            }
            else
            {
                temp = temp->sig;
            }
        }
        return NULL;
    }
    else
        return NULL;
}

/*Devuelve el nodo de la pelicula buscada */
PeliculasRegistro *Pelicula_devuelveNombrePeliculaPorHorario (listaPeliculas *pl, int buscado){
    if (pl->primero != NULL)
    {
        PeliculasRegistro *temp;
        temp = pl->primero;
        while (temp != NULL)
        {
            if (temp->peli_id == buscado)
            {
                return temp;
            }
            else
            {
                temp = temp->sig;
            }
        }
        return NULL;
    }
    else
        return NULL;
}

/*Devuelve el precio de una pelicula*/
int Pelicula_obtenerPrecioPelicula(PeliculasRegistro* temp, int tipo){
	   if (tipo == DEMANDADA)
		   return (int)(temp->peli_costo*temp->peli_extra/100)+temp->peli_costo;
	   else
		   return (int)(temp->peli_costo); 
		   
}

/*Retorna true si no se han alcanzado el numero maximo de conecciones*/
int puedoVerPelicula(listaPeliculas* pl,char* buscado){
    PeliculasRegistro *temp;
    temp = Pelicula_devuelvePelicula (pl, buscado);
	if (temp->peli_simultaneos == 0)//no puede verla
		return 0;
	else{
		temp->peli_simultaneos--;
	    return 1;
	}
}


/*Carga las peliculas de un archivo*/
void Pelicula_cargarArchivo(listaPeliculas* lista)
{
    FILE* archivoPeliculas;
    archivoPeliculas = fopen ("bd/peliculas.txt", "r+");
    while(fscanf(archivoPeliculas,"%d\n",&peli_id)!=EOF)
    {
        fscanf(archivoPeliculas,"%s\n",peli_nombre);
        fscanf(archivoPeliculas,"%s\n",peli_ruta);
        fscanf(archivoPeliculas,"%d\n",&peli_duracion);
        fscanf(archivoPeliculas,"%d\n",&peli_costo);
        fscanf(archivoPeliculas,"%d\n",&peli_extra);
		fscanf(archivoPeliculas,"%d\n",&peli_simultaneos);
		fscanf(archivoPeliculas,"%f\n",&peli_tamano);
        fscanf(archivoPeliculas,"--------------\n");
		Pelicula_insertar_Cliente(lista,peli_id,peli_nombre, peli_ruta, peli_duracion, peli_costo, peli_extra,peli_simultaneos,peli_tamano);
    }
    fclose(archivoPeliculas);
}

/*Imprime toda la lista*/
void Pelicula_ver_lista (listaPeliculas * pl){
    PeliculasRegistro *temp;
    if (NULL == pl->primero)
    {
        printf ("\n\nPerdón, la lista está vacía.\n\n");
    }
    else
    {
        temp = pl->primero;
        while (NULL != temp)
        {
            Pelicula_escribir_nodo (temp);
            temp = temp->sig;
        }
    }
}

/*Inicializa el nodo de peliculas*/
void Pelicula_inicializar (listaPeliculas *lista){
    lista = (listaPeliculas *)malloc (sizeof (listaPeliculas));
    lista->primero = NULL;	/* Al comenzar NO hay nodos */
    lista->final = NULL;	/* Al comenzar NO hay nodos */
}


void Peliculas_recargarArchivo(listaPeliculas* pl)
{
	//pthread_mutex_lock(&mutexModificacionClientes);
    Pelicula_borrar_lista(pl);
	Pelicula_cargarArchivo(pl);
	//Pelicula_ver_lista(pl);
	//pthread_mutex_unlock(&mutexModificacionClientes);
}



/*Guarda la lista*/
void Peliculas_guardarLista(listaPeliculas* lista){
    FILE* archivoPeliculas;
    archivoPeliculas = fopen ("bd/peliculas.txt", "w");
    PeliculasRegistro *temp = lista->primero;
	while (temp!=NULL){
		fprintf(archivoPeliculas,"%d\n",temp->peli_id);
		fprintf(archivoPeliculas,"%s\n",temp->peli_nombre);
		fprintf(archivoPeliculas,"%s\n",temp->peli_ruta);
		fprintf(archivoPeliculas,"%d\n",temp->peli_duracion);
		fprintf(archivoPeliculas,"%d\n",temp->peli_costo);
		fprintf(archivoPeliculas,"%d\n",temp->peli_extra);
		fprintf(archivoPeliculas,"%d\n",temp->peli_simultaneos);
		fprintf(archivoPeliculas,"%f\n",temp->peli_tamano);
		fprintf(archivoPeliculas,"--------------\n");
		temp = temp->sig;
	}
    fclose(archivoPeliculas);
}



/*Borra la lista de peliculas*/
void Pelicula_borrar_lista (listaPeliculas * pl)
{
    if (pl->primero != NULL){
        PeliculasRegistro *temp=pl->primero, *aux=pl->primero->sig;
		
		while(aux!=NULL){
			free(temp);
			temp=aux;
			aux=aux->sig;
		}
		free(temp);
        pl->primero = NULL;
        pl->final = NULL;
    }
}
