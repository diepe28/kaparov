

xmlChar * ConvertInput(const char *in, const char *encoding);

/*=====================================================================
=======================================================================
Funciones que ENVIAN un mensaje del servidor al cliente
=======================================================================
=====================================================================*/
/*Crea un xml que identifica si el login es valido o no*/
char* Enviar_Login(int tipo)
{
    //Aceptado = 1;
	//No Aceptado = 0;
	int rc;
    xmlTextWriterPtr writer;
    xmlBufferPtr buf;//Buffer donde escribo XML
    buf =xmlBufferCreate();
    writer = xmlNewTextWriterMemory(buf, 0);
    if (writer == NULL){
        printf("Enviar Login Aceptado: Error creando el xml-writer\n");
        return NULL;
    }
    ///Empieza el documento con el encabezado
    rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
    if (rc < 0){
        printf("Enviar Login Aceptado: Error en xmlTextWriterStartDocument\n");
        return NULL;
    }
    /*titulo*/
	if (tipo==1){
		rc = xmlTextWriterStartElement(writer, BAD_CAST "LOGIN_ACEPTADO");
		if (rc < 0){
			printf("Enviar Login Aceptado Error en xmlTextWriterStartElement\n");
			return NULL;
		}
	}
	else{
		rc = xmlTextWriterStartElement(writer, BAD_CAST "LOGIN_NO_ACEPTADO");
		if (rc < 0){
			printf("Enviar Login Aceptado Error en xmlTextWriterStartElement\n");
			return NULL;
		}
	}
    /*Termina*/
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0){
        printf("testXmlwriterFilename: Error at xmlTextWriterEndDocument\n");
        return NULL;
    }
    /*Libera*/
    xmlFreeTextWriter(writer);
	
    return (char*)buf->content;
}

/*Crea un xml con la factura del cliente*/
char* Enviar_Factura(char* login,listaClientes* lista)
{
    //Aceptado = 1;
	//No Aceptado = 0;
	int rc;
    xmlTextWriterPtr writer;
    xmlBufferPtr buf;
    buf =xmlBufferCreate();
	//recarga la lista desde el archivo
	Cliente_recargarArchivo(lista);
	ClientesRegistro* temp = Cliente_devuelveCliente(lista,login);
	
    writer = xmlNewTextWriterMemory(buf, 0);
    if (writer == NULL){
        printf("Enviar Login Aceptado: Error creando el xml-writer\n");
        return NULL;
    }
    ///Empieza el documento con el encabezado
    rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
    if (rc < 0){
        printf("Enviar Login Aceptado: Error en xmlTextWriterStartDocument\n");
        return NULL;
    }
    /*titulo*/
	rc = xmlTextWriterStartElement(writer, BAD_CAST "FACTURA");
	if (rc < 0){
		printf("Enviar Login Aceptado Error en xmlTextWriterStartElement\n");
		return NULL;
	}
	ClientesRegistro* temp_nodo=temp->historial;
	char *tempBuff,*bufferfinal="Desgloce: \n",tmpB[10];
	int liberar=0;
	while(temp_nodo!=NULL){
		tempBuff=(char*)malloc(sizeof(char)*(strlen(bufferfinal)+1));
		strcpy(tempBuff,bufferfinal);
		bufferfinal=(char*)malloc(sizeof(char)*(strlen(tempBuff)+strlen(temp_nodo->nombre_transaccion)+sizeof(int)+6));
		strcpy(bufferfinal,tempBuff);
		free(tempBuff);
		strcat(bufferfinal,temp_nodo->nombre_transaccion);
		strcat(bufferfinal,": ");
		sprintf(tmpB,"%d",temp_nodo->precio_transaccion);
		strcat(bufferfinal,tmpB);
		memset(tmpB,0,10*sizeof(char));
		strcat(bufferfinal,"\n");
		temp_nodo=temp_nodo->sig;
		liberar=1;
	}
	rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Info","%s", bufferfinal);
	if (rc < 0){
		printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
		return NULL;
	}
	//libera si hizo algun aumento
	if(liberar) free(bufferfinal);
		
	rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Total","%d", temp->cliente_total_factura);
	if (rc < 0){
		printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
		return NULL;
	}
    /*Termina*/
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0){
        printf("testXmlwriterFilename: Error at xmlTextWriterEndDocument\n");
        return NULL;
    }
    /*Libera*/
    xmlFreeTextWriter(writer);
    return (char*)buf->content;
}

/*Crea un xml con el mensaje de no hay dinero en su cuenta*/
char* Enviar_No_hay_plata()
{
    //Aceptado = 1;
	//No Aceptado = 0;
	int rc;
    xmlTextWriterPtr writer;
    xmlBufferPtr buf;//Buffer donde escribo XML
    buf =xmlBufferCreate();
    writer = xmlNewTextWriterMemory(buf, 0);
    if (writer == NULL){
        printf("Enviar Login Aceptado: Error creando el xml-writer\n");
        return NULL;
    }
    ///Empieza el documento con el encabezado
    rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
    if (rc < 0){
        printf("Enviar Login Aceptado: Error en xmlTextWriterStartDocument\n");
        return NULL;
    }
    /*titulo*/
	rc = xmlTextWriterStartElement(writer, BAD_CAST "NO_MONEY");
		if (rc < 0){
			printf("Enviar Login Aceptado Error en xmlTextWriterStartElement\n");
			return NULL;
		}
    /*Termina*/
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0){
        printf("testXmlwriterFilename: Error at xmlTextWriterEndDocument\n");
        return NULL;
    }
    /*Libera*/
    xmlFreeTextWriter(writer);
    return (char*)buf->content;
}

/*Crea un xml con el inventario de peliculas*/
char* Enviar_Inventario(listaPeliculas* lista){
    int rc;
    xmlTextWriterPtr writer;
    xmlBufferPtr buf;
    buf =xmlBufferCreate();
    writer = xmlNewTextWriterMemory(buf, 0);
    if (writer == NULL){
        printf("AgregarDinero: Error creando el xml-writer\n");
        return NULL;
    }
    PeliculasRegistro* temp = lista->primero;
    ///Empieza el documento con el encabezado
    rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
    if (rc < 0){
        printf("Enviar Inventario: Error en xmlTextWriterStartDocument\n");
        return NULL;
    }
    /*titulo*/
    rc = xmlTextWriterStartElement(writer, BAD_CAST "INVENTARIO");
    if (rc < 0){
        printf("Enviar Inventario: Error en xmlTextWriterStartElement\n");
        return NULL;
    }
    while (temp != NULL){
        /*PELICULA*/
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Pelicula","%s", temp->peli_nombre);
        if (rc < 0){
            printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
            return NULL;
        }
        /*Duracion*/
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Duracion","%d", temp->peli_duracion);
        if (rc < 0){
            printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
            return NULL;
        }
        /*Costo*/
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Costo","%d", temp->peli_costo);
        if (rc < 0){
            printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
            return NULL;
        }
        /*Duracion*/
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Extra","%d", temp->peli_extra);
        if (rc < 0){
            printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
            return NULL;
        }
        temp = temp->sig;
		
    }
    /*Termina*/
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0){
        printf("testXmlwriterFilename: Error at xmlTextWriterEndDocument\n");
        return NULL;
    }
    /*Libera*/
    xmlFreeTextWriter(writer);
    return (char*)(buf->content);
}

/*Crea un xml con la programacion del servidor*/
char* Enviar_Programacion(listaPeliculas* lista){
    int rc;
    xmlTextWriterPtr writer;
    xmlBufferPtr buf;
    buf =xmlBufferCreate();
    writer = xmlNewTextWriterMemory(buf, 0);
    if (writer == NULL){
        printf("AgregarDinero: Error creando el xml-writer\n");
        return NULL;
    }
    ///Empieza el documento con el encabezado
    rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
    if (rc < 0){
        printf("Enviar Inventario: Error en xmlTextWriterStartDocument\n");
        return NULL;
    }
    /*titulo*/
    rc = xmlTextWriterStartElement(writer, BAD_CAST "INVENTARIO");
    if (rc < 0){
        printf("Enviar Inventario: Error en xmlTextWriterStartElement\n");
        return NULL;
    }
    	PeliculasRegistro* temp = Pelicula_devuelveNombrePeliculaPorHorario(lista,Prog_pelicula1);
        /*PELICULA 1*/
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Pelicula","%s", temp->peli_nombre);
        if (rc < 0){
            printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
            return NULL;
        }
        /*Duracion*/
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Duracion","%d", temp->peli_duracion);
        if (rc < 0){
            printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
            return NULL;
        }
        /*Costo*/
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Costo","%d", temp->peli_costo);
        if (rc < 0){
            printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
            return NULL;
        }
        /*Duracion*/
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Extra","%d", temp->peli_extra);
        if (rc < 0){
            printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
            return NULL;
        }
    	temp = Pelicula_devuelveNombrePeliculaPorHorario(lista,Prog_pelicula2);
        /*PELICULA 2*/
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Pelicula","%s", temp->peli_nombre);
        if (rc < 0){
            printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
            return NULL;
        }
        /*Duracion*/
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Duracion","%d", temp->peli_duracion);
        if (rc < 0){
            printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
            return NULL;
        }
        /*Costo*/
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Costo","%d", temp->peli_costo);
        if (rc < 0){
            printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
            return NULL;
        }
        /*Duracion*/
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Extra","%d", temp->peli_extra);
        if (rc < 0){
            printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
            return NULL;
        }
    /*Termina*/
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0){
        printf("testXmlwriterFilename: Error at xmlTextWriterEndDocument\n");
        return NULL;
    }
    /*Libera*/
    xmlFreeTextWriter(writer);
    return (char*)(buf->content);
}

/*Crea un xml con el resultado de la busqueda del usuario*/
char* Enviar_BusquedaPelicula(listaPeliculas* lista,char* titulo){
    int rc;
    xmlTextWriterPtr writer;
    xmlBufferPtr buf;//Buffer donde escribo XML
    buf =xmlBufferCreate();
    writer = xmlNewTextWriterMemory(buf, 0);
    if (writer == NULL){
        printf("AgregarDinero: Error creando el xml-writer\n");
        return NULL;
    }
    PeliculasRegistro* temp = lista->primero;
    ///Empieza el documento con el encabezado
    rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
    if (rc < 0){
        printf("Enviar Inventario: Error en xmlTextWriterStartDocument\n");
        return NULL;
    }
    /*titulo*/
    rc = xmlTextWriterStartElement(writer, BAD_CAST "INVENTARIO");
    if (rc < 0){
        printf("Enviar Inventario: Error en xmlTextWriterStartElement\n");
        return NULL;
    }
    while (temp != NULL){

        if (strcmp(temp->peli_nombre,titulo)==0){
            /*PELICULA*/
            rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Pelicula","%s", temp->peli_nombre);
            if (rc < 0){
                printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
                return NULL;
            }
            /*Duracion*/
            rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Duracion","%d", temp->peli_duracion);
            if (rc < 0){
                printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
                return NULL;
            }
            /*Costo*/
            rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Costo","%d", temp->peli_costo);
            if (rc < 0){
                printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
                return NULL;
            }
            /*Duracion*/
            rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Extra","%d", temp->peli_extra);
            if (rc < 0){
                printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
                return NULL;
            }
        }
        temp = temp->sig;
    }
    /*Termina*/
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0){
        printf("testXmlwriterFilename: Error at xmlTextWriterEndDocument\n");
        return NULL;
    }
    /*Libera*/
    xmlFreeTextWriter(writer);
    return (char*)buf->content;
}

/*Crea un xml para el envio de los horarios*/
char* Enviar_Horario(listaPeliculas *pelis,listaHorarios *hora){
    //Buffer donde escribo XML
    int rc;
    xmlTextWriterPtr writer;
    xmlBufferPtr buf;//Buffer donde escribo XML
    buf =xmlBufferCreate();
    writer = xmlNewTextWriterMemory(buf, 0);
    if (writer == NULL){
        printf("AgregarDinero: Error creando el xml-writer\n");
        return NULL;
    }
    ///Empieza el documento con el encabezado
    rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
    if (rc < 0){
        printf("Enviar Inventario: Error en xmlTextWriterStartDocument\n");
        return NULL;
    }
    /*titulo*/
    rc = xmlTextWriterStartElement(writer, BAD_CAST "HORARIO");
    if (rc < 0){
        printf("Enviar Inventario: Error en xmlTextWriterStartElement\n");
        return NULL;
    }
	/********************************************************/
	//Peli A
        PeliculasRegistro* pelicula;
		pelicula = Pelicula_devuelveNombrePeliculaPorHorario(pelis,Prog_pelicula1);
	
	
	
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Pelicula","%s",pelicula->peli_nombre);
        if (rc < 0){
            printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
            return NULL;
        }
		 /*Duracion*/
		
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Duracion","%d", pelicula->peli_duracion);
        if (rc < 0){
            printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
            return NULL;
        }
		/*Costo*/
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Costo","%d",mem->Prog_tiempo1);
        if (rc < 0){
            printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
            return NULL;
        }
		
		int duracionVideo = pelicula->peli_duracion;//segundos
		//calculo datos
		float porcentaje_realizado = ((float)mem->Prog_tiempo1/(float)duracionVideo)*100;
		int porcentajeFinal = (int)porcentaje_realizado;
		/*Extra*/
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Extra","%d", Prog_porcentaje1);
        if (rc < 0){
            printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
            return NULL;
        }
		
		rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Extra","%d", porcentajeFinal);
        if (rc < 0){
            printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
            return NULL;
        }
    /******************************************************************/
		//Peli B
		pelicula = Pelicula_devuelveNombrePeliculaPorHorario(pelis,Prog_pelicula2);
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Pelicula","%s",pelicula->peli_nombre);
        if (rc < 0){
            printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
            return NULL;
        }
		 /*Duracion*/
		
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Duracion","%d", pelicula->peli_duracion);
        if (rc < 0){
            printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
            return NULL;
        }
		/*Costo*/
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Costo","%d",mem->Prog_tiempo2);
        if (rc < 0){
            printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
            return NULL;
        }
		
		duracionVideo = pelicula->peli_duracion;//segundos
		//calculo datos
		porcentaje_realizado = ((float)mem->Prog_tiempo2/(float)duracionVideo)*100;
		porcentajeFinal = (int)porcentaje_realizado;
		/*Extra*/
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Extra","%d", Prog_porcentaje2);
        if (rc < 0){
            printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
            return NULL;
        }
		
		rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "Extra","%d", porcentajeFinal);
        if (rc < 0){
            printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
            return NULL;
        }
    /*Termina*/
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0){
        printf("testXmlwriterFilename: Error at xmlTextWriterEndDocument\n");
        return NULL;
    }
    /*Libera*/
    xmlFreeTextWriter(writer);
    return (char*)buf->content;
}

/*Crea un xml para el envio del video*/
char* XML_escribirvideo(const char *video){
    int rc;
    xmlTextWriterPtr writer;
    xmlChar *tmp;
    xmlBufferPtr buf;/* Create a new XML buffer, to which the XML document will be
        * written */
    buf =xmlBufferCreate();

    /* Create a new XmlWriter for uri, with no compression. */
    writer = xmlNewTextWriterMemory(buf, 0);
    if (writer == NULL){
        printf("VIDEO: Error creando el xml-writer\n");
        return NULL;
    }
    /* Start the document with the xml default for the version,
     * encoding ISO 8859-1 and the default for the standalone
     * declaration. */
    rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
    if (rc < 0){
        printf("VIDEO: Error en xmlTextWriterStartDocument\n");
        return NULL;
    }
    /*titulo cartas*/
    rc = xmlTextWriterStartElement(writer, BAD_CAST "VIDEO");
    if (rc < 0){
        printf("VIDEO: Error en xmlTextWriterStartElement\n");
        return NULL;
    }
    /*escribe las cartas*/
    tmp = ConvertInput(video, MY_ENCODING);
    rc = xmlTextWriterWriteElement(writer, BAD_CAST "INFO", tmp);
    if (rc < 0){
        printf("VIDEO testXmlwriterFilename: Error at xmlTextWriterWriteElement\n");
        return NULL;
    }

    /* Here we could close the elements ORDER and EXAMPLE using the
     * function xmlTextWriterEndElement, but since we do not want to
     * write any other elements, we simply call xmlTextWriterEndDocument,
     * which will do all the work. */
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0){
        printf("VIDEO testXmlwriterFilename: Error at xmlTextWriterEndDocument\n");
        return NULL;
    }
    xmlFreeTextWriter(writer);
    return (char*)buf->content;
}

/*=====================================================================
=======================================================================
Funciones que RECIBEN un mensaje del servidor y toman los datos
=======================================================================
=====================================================================*/

/*Lee un XML y retorna un identificador para procesar el XML con alguna funcion especialista*/
int XML_leer(const char* msj)
{
    /*
	PROCESAR_LOGIN			1
	PROCESAR_AGREGARDINERO 	2
	PROCESAR_FACTURACION 	3
	PROCESAR_INVENTARIO 	4
	PROCESAR_BUSQUEDA 		5
	PROCESAR_VIDEO 			6
	PROCESAR_HORARIO 		7	
	PROCESAR_NUEVO_PASS 	8	
    */	
    xmlTextReaderPtr reader;
    const char* mensaje = msj;
    int ret;
    const xmlChar *name;
    reader = xmlReaderForMemory(mensaje,strlen(mensaje), NULL,NULL, 1);
    if (reader != NULL) {
        ret = xmlTextReaderRead(reader);
        while (ret == 1){
            name = xmlTextReaderConstName(reader);
            if(xmlStrcasecmp(name,(const xmlChar*)"LOGIN")==0)
            {
                return PROCESAR_LOGIN;
            }
            else if(xmlStrcasecmp(name,(const xmlChar*)"NUEVOPASS")==0)
            {
                return PROCESAR_NUEVO_PASS;
            }
			else if(xmlStrcasecmp(name,(const xmlChar*)"AGREGAR_DINERO")==0)
            {
                return PROCESAR_AGREGARDINERO;
            }
            else if(xmlStrcasecmp(name,(const xmlChar*)"FACTURACION")==0)
            {
                return PROCESAR_FACTURACION;
            }
            else if(xmlStrcasecmp(name,(const xmlChar*)"INVENTARIO")==0)
            {
                return PROCESAR_INVENTARIO;
            }
            else if(xmlStrcasecmp(name,(const xmlChar*)"BUSQUEDA_PELICULA")==0)
            {
                return PROCESAR_BUSQUEDA;
            }
            else if(xmlStrcasecmp(name,(const xmlChar*)"PELICULA")==0)
            {
                return PROCESAR_VIDEO;
            }
            else if(xmlStrcasecmp(name,(const xmlChar*)"HORARIO_PELICULA")==0)
            {
                return PROCESAR_HORARIO;
            }
			else if(xmlStrcasecmp(name,(const xmlChar*)"PROGRAMACION")==0)
            {
                return PROCESAR_PROGRAMACION;
            }
        }
		xmlFreeTextReader(reader);
    }
	return -1;
}

/*Procesa el xml y verifica que el usuario este registrado*/
int XML_procesarLogin(char* msj,char* tiposerver,listaClientes* lista)
{
    xmlTextReaderPtr reader;
    const char* mensaje = msj;
    const xmlChar *value;
    reader = xmlReaderForMemory(mensaje,strlen(mensaje), NULL,NULL, 1);
    xmlTextReaderRead(reader);
    xmlTextReaderConstName(reader);
    char login[30], passwd[30];
    xmlTextReaderRead(reader);//USUARIO
    xmlTextReaderRead(reader);//CONTENIDO USUARIO
    value = xmlTextReaderConstValue(reader);//dfernandez
    strcpy(login,(char*)value);
    xmlTextReaderRead(reader);//FINUSUARIO
    xmlTextReaderRead(reader);//PASSWORD
    xmlTextReaderRead(reader);//CONTENIDO PASSWORD
    value = xmlTextReaderConstValue(reader);//dfernandez
    strcpy(passwd,(char*)value);
	printf("%s-%s: Consulta-> Login\n",login,tiposerver);
    if (Cliente_validarCliente(lista,login,passwd)){
        printf("Login y password Correctos\n");
        return 1;
    }
    else{
        printf("Login o password invalido\n");
        return 0;
    }
}

/*Procesa el xml y Cambia el password del cliente por el nuevo*/
void XML_procesarNuevoPass(char* msj,char* tiposerver,listaClientes* lista)
{
    xmlTextReaderPtr reader;
    const char* mensaje = msj;
    const xmlChar *value;
    reader = xmlReaderForMemory(mensaje,strlen(mensaje), NULL,NULL, 1);
    xmlTextReaderRead(reader);
    xmlTextReaderConstName(reader);
    char login[30], passwd[30];
    xmlTextReaderRead(reader);//USUARIO
    xmlTextReaderRead(reader);//CONTENIDO USUARIO
    value = xmlTextReaderConstValue(reader);//dfernandez
    strcpy(login,(char*)value);
    xmlTextReaderRead(reader);//FINUSUARIO
    xmlTextReaderRead(reader);//PASSWORD
    xmlTextReaderRead(reader);//CONTENIDO PASSWORD
    value = xmlTextReaderConstValue(reader);//dfernandez
    strcpy(passwd,(char*)value);
	printf("%s-%s: Consulta-> Login\n",login,tiposerver);
    Cliente_cambiarPassword(lista,login,passwd);
    printf("Login y password Cambiados\n");    
}

/*Procesa el xml y aumenta el dinero de la cuenta del usuario*/
int XML_procesarAgregarDinero(char* msj,char* tiposerver,listaClientes* lista)
{
    xmlTextReaderPtr reader;
    const char* mensaje = msj;
    const xmlChar *value;
    reader = xmlReaderForMemory(mensaje,strlen(mensaje), NULL,NULL, 1);
    xmlTextReaderRead(reader);
    xmlTextReaderConstName(reader);
    char login[30], passwd[30];
    xmlTextReaderRead(reader);//USUARIO
    xmlTextReaderRead(reader);//CONTENIDO USUARIO
    value = xmlTextReaderConstValue(reader);//dfernandez
    strcpy(login,(char*)value);
    xmlTextReaderRead(reader);//FINUSUARIO
    xmlTextReaderRead(reader);//PASSWORD
    xmlTextReaderRead(reader);//CONTENIDO PASSWORD
    value = xmlTextReaderConstValue(reader);//cantidad
    strcpy(passwd,(char*)value);
    int x = atoi(passwd);
	printf("%s-%s: Consulta-> Aumento Dinero\n",login,tiposerver);
    if (Cliente_aumentarDinero(lista,login,x)){
        printf("Agregue al usuario: %s el monto de %d\n",login,x);
        return 1;
    }
    else{
        printf("No se agrego el dinero al usuario: %s\n",login);
        return 0;
    }
}

/*Procesa el xml y envia el inventario de peliculas del servidor*/
char* XML_procesarInventario(char* msj,char* tiposerver,listaClientes* lista,listaPeliculas* peli)
{
    xmlTextReaderPtr reader;
    const char* mensaje = msj;
    const xmlChar *value;
    reader = xmlReaderForMemory(mensaje,strlen(mensaje), NULL, NULL, 1);
	xmlTextReaderRead(reader);
    xmlTextReaderConstName(reader);
    char login[LARGO_CLIENTE];
    xmlTextReaderRead(reader);//USUARIO
    xmlTextReaderRead(reader);//CONTENIDO USUARIO
    value = xmlTextReaderConstValue(reader);//dfernandez
    strcpy(login,(char*)value);
    xmlTextReaderRead(reader);//FINUSUARIO
	printf("%s-%s: Consulta-> Listar Peliculas\n",login,tiposerver);
	if (Cliente_disminuirDinero(lista,login,PRECIO_INVENTARIO)){
		Cliente_agregarfactura(lista,login,"Consulta de Peliculas",PRECIO_INVENTARIO);
		return Enviar_Inventario(peli);
	}
	else
		return Enviar_No_hay_plata();
}

/*Procesa el xml y envia la lista de peliculas que se estan transmitiendo actualmente*/
char* XML_procesarProgramacion(char* msj,char* tiposerver,listaClientes* lista,listaPeliculas* peli)
{
    xmlTextReaderPtr reader;
    const char* mensaje = msj;
    const xmlChar *value;
    reader = xmlReaderForMemory(mensaje,strlen(mensaje), NULL, NULL, 1);
	xmlTextReaderRead(reader);
    xmlTextReaderConstName(reader);
    char login[LARGO_CLIENTE];
    xmlTextReaderRead(reader);//USUARIO
    xmlTextReaderRead(reader);//CONTENIDO USUARIO
    value = xmlTextReaderConstValue(reader);//dfernandez
    strcpy(login,(char*)value);
    xmlTextReaderRead(reader);//FINUSUARIO
	printf("%s-%s: Consulta-> Listar Peliculas\n",login,tiposerver);
	if (Cliente_disminuirDinero(lista,login,PRECIO_INVENTARIO)){
		Cliente_agregarfactura(lista,login,"Programacion",PRECIO_PROGRAMACION);
		return Enviar_Programacion(peli);
	}
	else
		return Enviar_No_hay_plata();
}

/*Procesa el xml y envia el resultado de la busqueda*/
char* XML_procesarBusqueda(char* msj,char* tiposerver,listaClientes* lista,listaPeliculas* peli)
{
    xmlTextReaderPtr reader;
    const char* mensaje = msj;
    const xmlChar *value;
    reader = xmlReaderForMemory(mensaje,strlen(mensaje), NULL,NULL, 1);
    xmlTextReaderRead(reader);
    xmlTextReaderConstName(reader);
    char login[LARGO_CLIENTE], pelicula[LARGO_PELICULA];
    xmlTextReaderRead(reader);//USUARIO
    xmlTextReaderRead(reader);//CONTENIDO USUARIO
    value = xmlTextReaderConstValue(reader);//dfernandez
    strcpy(login,(char*)value);
    xmlTextReaderRead(reader);//FINUSUARIO
    xmlTextReaderRead(reader);//PELI
    xmlTextReaderRead(reader);//CONTENIDO PELI
    value = xmlTextReaderConstValue(reader);//peli
    strcpy(pelicula,(char*)value);
	printf("%s-%s: Consulta-> Buscar Pelicula\n",login,tiposerver);
    if (Cliente_disminuirDinero(lista,login,PRECIO_BUSQUEDA)){
		Cliente_agregarfactura(lista,login,"Consulta de Busqueda",PRECIO_BUSQUEDA);
		return Enviar_BusquedaPelicula(peli,pelicula);
	}
	else
		return Enviar_No_hay_plata();
}

/*Procesa el xml y envia el horario al cliente*/
char* XML_procesarHorario(char* msj, char* tiposerver,listaClientes* lista,listaPeliculas* peli,listaHorarios *hora)
{
    xmlTextReaderPtr reader;
    const char* mensaje = msj;
    const xmlChar *value;
    reader = xmlReaderForMemory(mensaje,strlen(mensaje), NULL,NULL, 1);
    xmlTextReaderRead(reader);
    xmlTextReaderConstName(reader);
    char login[LARGO_CLIENTE];
    xmlTextReaderRead(reader);//USUARIO
    xmlTextReaderRead(reader);//CONTENIDO USUARIO
    value = xmlTextReaderConstValue(reader);//dfernandez
    strcpy(login,(char*)value);
    xmlTextReaderRead(reader);//FINUSUARIO
	printf("%s-%s: Consulta-> Listar Horarios\n",login,tiposerver);
    if (Cliente_disminuirDinero(lista,login,PRECIO_HORARIO)){
		Cliente_agregarfactura(lista,login,"Consulta Horario",PRECIO_HORARIO);
		return Enviar_Horario(peli,hora);
	}
	else
		return Enviar_No_hay_plata();
}

/*Procesa el xml del video (programacion o demanda) y envia en pedazos el video*/
char* XML_procesarVideo(char* msj,char *tiposerver,listaClientes* lista,listaPeliculas* pelic,int idsocket)
{
    xmlTextReaderPtr reader;
    const char* mensaje = msj;
    const xmlChar *value;
    reader = xmlReaderForMemory(mensaje,strlen(mensaje), NULL,NULL, 1);
    xmlTextReaderRead(reader);
    xmlTextReaderConstName(reader);
    char login[30], peli[30];
	int tipo;
    xmlTextReaderRead(reader);//USUARIO
    xmlTextReaderRead(reader);//CONTENIDO USUARIO
    value = xmlTextReaderConstValue(reader);//dfernandez
    strcpy(login,(char*)value);
    xmlTextReaderRead(reader);//FINUSUARIO
	xmlTextReaderRead(reader);//PELICULA
    xmlTextReaderRead(reader);//CONTENIDO PELICULA
    value = xmlTextReaderConstValue(reader);//peli
    strcpy(peli,(char*)value);
    xmlTextReaderRead(reader);//FINPELI
	xmlTextReaderRead(reader);//Tipo
    xmlTextReaderRead(reader);//CONTENIDO TIPO
    value = xmlTextReaderConstValue(reader);//tipo
    tipo = atoi((char*)value);
    xmlTextReaderRead(reader);//FINPELI
	if (tipo == 1){//Procesar Desde El Principio
		PeliculasRegistro * nodo = Pelicula_devuelvePelicula(pelic,peli);
		if (nodo->peli_simultaneos > 0){
			Pelicula_disminuirSimultaneos(pelic,peli);
			nodo = Pelicula_devuelvePelicula(pelic,peli);
			printf("%s-%s: Consulta-> Envio de Stream %s\n",login,tiposerver,nodo->peli_ruta);
			int precioPelicula = Pelicula_obtenerPrecioPelicula(nodo,tipo);
			if (Cliente_disminuirDinero(lista, login,precioPelicula)){
				Cliente_agregarfactura(lista,login,nodo->peli_nombre,nodo->peli_costo);
				FILE* archivo = fopen(nodo->peli_ruta,"rb");
				void* video = (void*)malloc(MAX_BUFFER);
				int x=0;
				char buf[MAX_BUFFER];
				SendM(idsocket,"ok",3);
				RecibirM(idsocket,buf,3);
				//recibio la confirmacion
				while ((x=fread(video,MAX_BUFFER,1,archivo))== 1)
				{
					SendM(idsocket, video, MAX_BUFFER);
					RecibirM(idsocket,buf,3);
					memset(video,0,MAX_BUFFER);
				}
				SendM(idsocket, video, MAX_BUFFER);
				RecibirM(idsocket,buf,3);
				fclose(archivo);
				memset(video,0,MAX_BUFFER);
				memcpy(video,"fin",3);
				SendM(idsocket,video,MAX_BUFFER);
				RecibirM(idsocket,buf,3);
				close(idsocket);
				close(idsocket);
				free(video);
				printf("Fin pelicula\n");
				Pelicula_aumentarSimultaneos(pelic,peli);
				return "";
			}
			else{
				return Enviar_No_hay_plata();
			}
		}
		else{
			printf("%s-%s: Limite alcanzado en pelicula: %s\n",login,tiposerver,nodo->peli_ruta);
			send(idsocket,"Limite",7,0);
			return "";
		}
	}
	else{//Procesar Programacion
		PeliculasRegistro * nodo = Pelicula_devuelvePelicula(pelic,peli);
		int precioPelicula = 0;
		long bytesxsegundo = 0;
		int tiempo = 0;
		if (nodo->peli_id == Prog_pelicula1)//Se usan datos de pelicula
		{
			printf("%s-%s: Consulta-> Envio de Stream %s\n",login,tiposerver,nodo->peli_ruta);
			int duracionVideo = nodo->peli_duracion;//segundos
			FILE* arch = fopen(nodo->peli_ruta,"r+");
			fseek(arch,0,SEEK_END);
			long tamaNo = ftell(arch);
			fclose(arch);
			precioPelicula = Pelicula_obtenerPrecioPelicula(nodo,tipo);
			//calculo datos
			bytesxsegundo = tamaNo/(long)duracionVideo;
			float porcentaje_realizado = ((float)mem->Prog_tiempo1/(float)duracionVideo)*100;
			printf("%f\n",(float)bytesxsegundo);
			printf("%d\n",(int) bytesxsegundo);
			printf("%f\n",porcentaje_realizado);		
			if (porcentaje_realizado > Prog_porcentaje1){//Ya paso el porcentaje, se espera nueva emision
				printf("Espera de una nueva emision\n");
				while(mem->Prog_tiempo1!=0);
			}
			tiempo = mem->Prog_tiempo1;
		}
		else{
			printf("%s-%s: Consulta-> Envio de Stream %s\n",login,tiposerver,nodo->peli_ruta);
			int duracionVideo = nodo->peli_duracion;//segundos
			FILE* arch = fopen(nodo->peli_ruta,"r+");
			fseek(arch,0,SEEK_END);
			long tamaNo = ftell(arch);
			fclose(arch);
			precioPelicula = Pelicula_obtenerPrecioPelicula(nodo,tipo);
			//calculo datos
			bytesxsegundo  = tamaNo/(long)duracionVideo;
			float porcentaje_realizado = ((float)mem->Prog_tiempo2/(float)duracionVideo)*100;
			printf("%f\n",(float)bytesxsegundo);
			printf("%d\n",(int) bytesxsegundo);
			printf("%f\n",porcentaje_realizado);		
			if (porcentaje_realizado > Prog_porcentaje2){//Ya paso el porcentaje, se espera nueva emision
				printf("Espera de una nueva emision\n");
				while(mem->Prog_tiempo2!=0);
			}
			tiempo = mem->Prog_tiempo2;
		}
		if (Cliente_disminuirDinero(lista, login,precioPelicula)){
				printf("Empiezo a reproducir la pelicula\n");
				Cliente_agregarfactura(lista,login,nodo->peli_nombre,nodo->peli_costo);
				//recibio la confirmacion
				FILE* archivo = fopen(nodo->peli_ruta,"rb");
				void* video = (void*)malloc(MAX_BUFFER);
				void* header = (void*)malloc(MAX_BUFFER);
				int x=0;
				char buf[MAX_BUFFER];
				SendM(idsocket,"ok",3);
				RecibirM(idsocket,buf,3);
				fread(header,MAX_BUFFER,1,archivo);
			    
				SendM(idsocket,header,MAX_BUFFER);
				RecibirM(idsocket,buf,3);
				free(header);
				fseek(archivo,bytesxsegundo*(long)tiempo,SEEK_SET);
				while ((x=fread(video,MAX_BUFFER,1,archivo))== 1)
				{
					SendM(idsocket, video, MAX_BUFFER);
					RecibirM(idsocket,buf,3);
					memset(video,0,MAX_BUFFER);
				}
				SendM(idsocket, video, MAX_BUFFER);
				RecibirM(idsocket,buf,3);			
				fclose(archivo);
				memset(video,0,MAX_BUFFER);
				memcpy(video,"fin",3);
				SendM(idsocket,video,MAX_BUFFER);
				RecibirM(idsocket,buf,3);
				free(video);
				close(idsocket);
				close(idsocket);
				printf("Fin pelicula\n");
				return "";
			}
			else{
				return Enviar_No_hay_plata();
			}
	}
	return "";
}

/*Procesa la peticion de la factura y envia la factura al cliente especifico*/
char* XML_procesarFacturacion(char* msj,char *tiposerver,listaClientes* lista)
{
    xmlTextReaderPtr reader;
    const char* mensaje = msj;
    const xmlChar *value;
    reader = xmlReaderForMemory(mensaje,strlen(mensaje), NULL,NULL, 1);
    xmlTextReaderRead(reader);
    xmlTextReaderConstName(reader);
    char login[30];
    xmlTextReaderRead(reader);//USUARIO
    xmlTextReaderRead(reader);//CONTENIDO USUARIO
    value = xmlTextReaderConstValue(reader);//dfernandez
    strcpy(login,(char*)value);
    xmlTextReaderRead(reader);//FINUSUARIO
	xmlFreeTextReader(reader);
	printf("%s-%s: Consulta-> Mostrar Factura\n",login,tiposerver);
	return Enviar_Factura(login,lista);
}

/*Convierte un char* a xmlChar**/
xmlChar * ConvertInput(const char *in, const char *encoding){
    xmlChar *out;
    int ret;
    int size;
    int out_size;
    int temp;
    xmlCharEncodingHandlerPtr handler;

    if (in == 0)
        return 0;

    handler = xmlFindCharEncodingHandler(encoding);

    if (!handler){
        printf("ConvertInput: no encoding handler found for '%s'\n",
               encoding ? encoding : "");
        return 0;
    }

    size = (int) strlen(in) + 1;
    out_size = size * 2 - 1;
    out = (unsigned char *) xmlMalloc((size_t) out_size);

    if (out != 0) {
        temp = size - 1;
        ret = handler->input(out, &out_size, (const xmlChar *) in, &temp);
        if (ret || temp - size + 1) {
            if (ret) {
                printf("ConvertInput: conversion wasn't successful.\n");
            } else{
                printf("ConvertInput: conversion wasn't successful. converted: %i octets.\n",
                       temp);
            }
            xmlFree(out);
            out = 0;
        }
        else{
            out = (unsigned char *) xmlRealloc(out, out_size + 1);
            out[out_size] = 0;  /*null terminating out */
        }
    }
    else{
        printf("ConvertInput: no mem\n");
    }
    return out;
}

//Captura el mensaje que se mando por el socket
//y envia una respuesta con XML
void EnviarRespuesta(int idsocket,char* mensaje,char* tiposerver,listaClientes* lista,listaPeliculas* peli,listaHorarios *hora){
    int x = -1,loginvalido=-1;
	printf("Recibo Mensaje ENCRIPTADO:\n%s\n",mensaje);
	criptografia_Desencriptar(mensaje);
	printf("Desencripto mensaje:\n%s\n",mensaje);	
	int QUE_HACER = XML_leer(mensaje);
	char* XML_cadena;
	
	switch(QUE_HACER){
		case PROCESAR_LOGIN:
			loginvalido = XML_procesarLogin(mensaje,tiposerver,lista);
			if(loginvalido){
				XML_cadena = Enviar_Login(loginvalido);
				criptografia_Encriptar(XML_cadena);
				send(idsocket, XML_cadena, strlen(XML_cadena), 0);
				free(XML_cadena);
			}
			else{
				XML_cadena = Enviar_Login(loginvalido);
				criptografia_Encriptar(XML_cadena);
				send(idsocket, XML_cadena, strlen(XML_cadena), 0);
				free(XML_cadena);
			}
			break;
		case PROCESAR_NUEVO_PASS:
			{
				XML_procesarNuevoPass(mensaje,tiposerver,lista);
			}
			break;
		
			
		case PROCESAR_AGREGARDINERO:
			x = XML_procesarAgregarDinero(mensaje,tiposerver,lista);
			break;
		
		case PROCESAR_INVENTARIO:
			XML_cadena = XML_procesarInventario(mensaje,tiposerver,lista,peli);
			criptografia_Encriptar(XML_cadena);
			send(idsocket, XML_cadena, strlen(XML_cadena), 0);
			free(XML_cadena);
			break;
			
		case PROCESAR_BUSQUEDA:
			XML_cadena = XML_procesarBusqueda(mensaje,tiposerver,lista,peli);
			criptografia_Encriptar(XML_cadena);
			send(idsocket, XML_cadena, strlen(XML_cadena), 0);
			free(XML_cadena);
			break;
		
		case PROCESAR_HORARIO:
			XML_cadena = XML_procesarHorario(mensaje,tiposerver,lista,peli,hora);
			criptografia_Encriptar(XML_cadena);
			send(idsocket, XML_cadena, strlen(XML_cadena), 0);
			free(XML_cadena);
			break;
	
		case PROCESAR_VIDEO:
			XML_cadena = XML_procesarVideo(mensaje,tiposerver,lista,peli,idsocket);
			if(strlen(XML_cadena)!=0){
				criptografia_Encriptar(XML_cadena);
				send(idsocket, XML_cadena, strlen(XML_cadena), 0);
				free(XML_cadena);
			}
			break;
		
		case PROCESAR_FACTURACION:
			XML_cadena = XML_procesarFacturacion(mensaje,tiposerver,lista);
			criptografia_Encriptar(XML_cadena);
			send(idsocket, XML_cadena, strlen(XML_cadena), 0);
			free(XML_cadena);
		break;
		
		case PROCESAR_PROGRAMACION:
			XML_cadena = XML_procesarProgramacion(mensaje,tiposerver,lista,peli);
			criptografia_Encriptar(XML_cadena);
			send(idsocket, XML_cadena, strlen(XML_cadena), 0);
			free(XML_cadena);
		break;
	}
}
