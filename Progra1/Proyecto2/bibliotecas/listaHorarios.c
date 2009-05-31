
//definicion del nodo
typedef struct HorariosRegistro {
    int horario_id;
    struct HorariosRegistro * sig;
} HorariosRegistro;

typedef struct listaHorarios {
    HorariosRegistro * primero;
    HorariosRegistro * final;
} listaHorarios;

void Horario_inicializar(listaHorarios*);
HorariosRegistro* Horario_crearNuevoRegistro(char*,int);
void Horario_escribir_nodo(HorariosRegistro *,listaPeliculas *);
HorariosRegistro* Horario_devuelveHorario(listaHorarios *,char*);
void Horario_insertar_Horario(listaHorarios * pl, int horario_id);
void Horario_ver_lista(listaHorarios * pl,listaPeliculas *pelic);
void Horario_borrar_lista(listaHorarios * pl);
void Horario_cargarArchivo(listaHorarios *);

/*Imprime en pantalla el nodo*/
void Horario_escribir_nodo (HorariosRegistro * pn,listaPeliculas *pelis){
    printf ("ID: %d\n", pn->horario_id);
	char* nombre = Pelicula_devuelveNombrePeliculaPorHorario(pelis, pn->horario_id)->peli_nombre;
	printf ("Nombre: %s\n-----------\n", nombre);
}

/*Insertar nodo al final*/
void Horario_insertar_Cliente (listaHorarios * pl, int horario_id){
    //Si no existe se inserta con valores default
    HorariosRegistro *temp;
    temp = (HorariosRegistro *) malloc (sizeof (HorariosRegistro));
    temp->horario_id = horario_id;
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

/*Imprime la lista de horarios*/
void Horario_ver_lista (listaHorarios * pl,listaPeliculas *pelic){
    HorariosRegistro *temp;
    if (NULL == pl->primero)
    {
        printf ("\n\nPerdón, la lista está vacía.\n\n");
    }
    else
    {
        temp = pl->primero;
        while (NULL != temp)
        {
            Horario_escribir_nodo (temp,pelic);
            temp = temp->sig;
        }
    }
}

/*Inicializa un nodo horario*/
void Horario_inicializar (listaHorarios *lista){
    lista = (listaHorarios *)malloc (sizeof (listaHorarios));
    lista->primero = NULL;	/* Al comenzar NO hay nodos */
    lista->final = NULL;	/* Al comenzar NO hay nodos */
}

/*Carga el horario de archivo*/
void Horario_cargarArchivo(listaHorarios* lista){
    FILE* archivoHorarios;
	int horario_id;
    archivoHorarios = fopen ("bd/horarios.txt", "r+");
    while(fscanf(archivoHorarios,"%d\n",&horario_id)!=EOF){
		Horario_insertar_Cliente(lista,horario_id);
    }
    fclose(archivoHorarios);
}

/*Borra la lista de horarios*/
void Horario_borrar_lista (listaHorarios * pl){
    if (pl->primero != NULL){
        HorariosRegistro *temp=pl->primero, *aux=pl->primero->sig;
		
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
