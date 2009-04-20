#include <gtk/gtk.h>
#include <stdio.h>
#include "Variables.h"

/* Estructura para los datos de las barras de progreso */
typedef struct {
    GtkWidget *ventana;
    GtkWidget **barras;
    GtkWidget **marcosBarras;
    GtkWidget **etiquetasBarras;
    char **mensajesEtiquetas;
    GtkWidget **marcosEtiquetas;
    GtkWidget **etiquetasPi;
    char **mensajesPi;
    GtkWidget **marcosPi;
    void (*funcion)();
} BarraProgreso;

/* Buffers de los mensajes informativos */
static char mensajeModo[100];
static char mensajeQuantum[100];
static char mensajeNumHilos[100];

/* Datos globales de las barras de progreso */
static BarraProgreso *ptrBarra;

/* Actualiza la barra de progreso indicada */
void actualizarBarra(int numBarra, double fraccion)
{

    gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (ptrBarra->barras[numBarra]), fraccion);
    sprintf(ptrBarra->mensajesPi[numBarra], " Pi: %0.30Lg ", RESPUESTAS[numBarra]);
	gtk_label_set_text(GTK_LABEL(ptrBarra->etiquetasPi[numBarra]), ptrBarra->mensajesPi[numBarra]);
    g_signal_emit_by_name(ptrBarra->ventana, "show");

    while (gtk_events_pending ())
	  gtk_main_iteration ();
}

/* Funcion a invocar al inicial el ciclo principal */
static gboolean despliegueIniciado(gpointer datos)
{
    // Asignar puntero global a los datos de las barras de progreso
    ptrBarra = (BarraProgreso *) datos;

    // Invocar funcion principal de los hilos (ejecutar_scheduler)
    ptrBarra->funcion();

    return TRUE;
}

/* Finaliza la ventana principal */
static void finalizarDespliegue(GtkWidget *widget, BarraProgreso *datosBarra)
{
    int i;

    // Liberar memoria de los hilos
    g_free(datosBarra->marcosPi);
    for (i = 0; i < NUM_HILOS; i++) {
        g_free(datosBarra->mensajesPi[i]);
    }
    g_free(datosBarra->mensajesPi);
    g_free(datosBarra->etiquetasPi);

    g_free(datosBarra->marcosEtiquetas);
    for (i = 0; i < NUM_HILOS; i++) {
        g_free(datosBarra->mensajesEtiquetas[i]);
    }
    g_free(datosBarra->mensajesEtiquetas);
    g_free(datosBarra->etiquetasBarras);

    g_free(datosBarra->marcosBarras);
    g_free(datosBarra->barras);

    datosBarra->ventana = NULL;
    g_free (datosBarra);
    gtk_main_quit ();
}

/* Inicializa la interfaz GTK */
void inicializarDespliegue(int *argc, char ***argv)
{
    gtk_init (argc, argv);
}

/* Desplegar panel con los parametros */
void desplegarPanelInformativo(GtkWidget * panelVertical)
{
    GtkWidget *marcoInformacion;
    GtkWidget *panelInformacion;
    GtkWidget *etiquetaModo;
    GtkWidget *etiquetaNumHilos;
    GtkWidget *etiquetaQuantum;

    // Panel informativo
    marcoInformacion = gtk_frame_new ("SOA - Lottery Scheduling");
    gtk_container_set_border_width (GTK_CONTAINER (marcoInformacion), 0);
    gtk_box_pack_start (GTK_BOX (panelVertical), marcoInformacion, FALSE, FALSE, 0);

    panelInformacion = gtk_vbox_new (FALSE, 5);
    gtk_container_set_border_width (GTK_CONTAINER (panelInformacion), 10);
    gtk_container_add (GTK_CONTAINER (marcoInformacion), panelInformacion);

    sprintf(mensajeModo, "Modo: %s",
        (modoActual == Expropiativo ? "Expropiativo" : "No Expropiativo"));
    etiquetaModo = gtk_label_new(mensajeModo);
    gtk_container_add (GTK_CONTAINER (panelInformacion), etiquetaModo);

    sprintf(mensajeNumHilos, "Número de hilos: %d", NUM_HILOS);
    etiquetaNumHilos = gtk_label_new(mensajeNumHilos);
    gtk_container_add (GTK_CONTAINER (panelInformacion), etiquetaNumHilos);

    sprintf(mensajeQuantum, "%s: %d",
        (modoActual == Expropiativo ? "Tamaño del quantum" : "Factor de trabajo"), QUANTUM);
    etiquetaQuantum = gtk_label_new(mensajeQuantum);
    gtk_container_add (GTK_CONTAINER (panelInformacion), etiquetaQuantum);

}

/* Desplegar boton para salir */
void desplegarBotonSalir(GtkWidget * panelVertical, BarraProgreso * datosBarra)
{
    GtkWidget *lineaSeparacion;
    GtkWidget *marcoBoton;
    GtkWidget *botonSalir;

    // Linea de separacion
    lineaSeparacion = gtk_hseparator_new ();
    gtk_box_pack_start (GTK_BOX (panelVertical), lineaSeparacion, FALSE, FALSE, 0);

    // Marco de alineado del boton
    marcoBoton = gtk_alignment_new (0.5, 0.5, 0, 0);
    gtk_box_pack_start (GTK_BOX (panelVertical), marcoBoton, FALSE, FALSE, 5);

    // Boton para salir
    botonSalir = gtk_button_new_with_label ("Salir");
    g_signal_connect_swapped (G_OBJECT (botonSalir), "clicked",
        G_CALLBACK (gtk_widget_destroy), G_OBJECT (datosBarra->ventana));
    gtk_container_add (GTK_CONTAINER (marcoBoton), botonSalir);
    GTK_WIDGET_SET_FLAGS (botonSalir, GTK_CAN_DEFAULT);
    gtk_widget_grab_default (botonSalir); // establecer como boton por defecto
}

/* Desplegar barras de progreso */
void desplegarBarrasProgreso(GtkWidget * panelVertical, BarraProgreso * datosBarra)
{
    GtkWidget *marcoBarras;
    GtkWidget *tablaBarras;
    int i;

    // Panel de las barras de progreso
    marcoBarras = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (marcoBarras),
        GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start (GTK_BOX (panelVertical), marcoBarras, TRUE, TRUE, 0);
    tablaBarras = gtk_table_new (NUM_HILOS, 3, FALSE);
    gtk_table_set_row_spacings (GTK_TABLE (tablaBarras), 5);
    gtk_table_set_col_spacings (GTK_TABLE (tablaBarras), 5);
    gtk_scrolled_window_add_with_viewport (
        GTK_SCROLLED_WINDOW (marcoBarras), tablaBarras);

    // Etiquetas de las barras
    datosBarra->marcosEtiquetas = g_malloc(NUM_HILOS * sizeof(GtkWidget*));
    datosBarra->mensajesEtiquetas = g_malloc(NUM_HILOS * sizeof(char*));
    datosBarra->etiquetasBarras = g_malloc(NUM_HILOS * sizeof(GtkWidget*));
    for (i = 0; i < NUM_HILOS; i++) {
        datosBarra->marcosEtiquetas[i] = gtk_alignment_new (0, 0.5, 0, 0);
        datosBarra->mensajesEtiquetas[i] = g_malloc(100);
        sprintf(datosBarra->mensajesEtiquetas[i], "  Hilo: %d - Num Tiquetes: %ld - Trabajo: %ld",
            (i+1), CANT_TIQUETES[i], CANT_TRABAJO[i]);
        datosBarra->etiquetasBarras[i] = gtk_label_new(datosBarra->mensajesEtiquetas[i]);
        gtk_container_add (GTK_CONTAINER (datosBarra->marcosEtiquetas[i]), datosBarra->etiquetasBarras[i]);
        gtk_table_attach_defaults (GTK_TABLE (tablaBarras), datosBarra->marcosEtiquetas[i], 0, 1, i, i+1);

    }

    // Barras de progreso
    datosBarra->marcosBarras = g_malloc(NUM_HILOS * sizeof(GtkWidget*));
    datosBarra->barras = g_malloc(NUM_HILOS * sizeof(GtkWidget*));
    for (i = 0; i < NUM_HILOS; i++) {
        datosBarra->marcosBarras[i] = gtk_alignment_new (0.5, 0.5, 0, 0);
        gtk_table_attach_defaults (GTK_TABLE (tablaBarras), datosBarra->marcosBarras[i], 1, 2, i, i+1);
        datosBarra->barras[i] = gtk_progress_bar_new ();
        gtk_container_add (GTK_CONTAINER (datosBarra->marcosBarras[i]), datosBarra->barras[i]);
    }

    // Etiquetas para el calculo de pi
    datosBarra->marcosPi = g_malloc(NUM_HILOS * sizeof(GtkWidget*));
    datosBarra->mensajesPi = g_malloc(NUM_HILOS * sizeof(char*));
    datosBarra->etiquetasPi = g_malloc(NUM_HILOS * sizeof(GtkWidget*));
    for (i = 0; i < NUM_HILOS; i++) {
        datosBarra->marcosPi[i] = gtk_alignment_new (0, 0.5, 0, 0);
        datosBarra->mensajesPi[i] = g_malloc(100);
        sprintf(datosBarra->mensajesPi[i], " Pi: %0.30Lg ", RESPUESTAS[i]);
        datosBarra->etiquetasPi[i] = gtk_label_new(datosBarra->mensajesPi[i]);
        gtk_container_add (GTK_CONTAINER (datosBarra->marcosPi[i]), datosBarra->etiquetasPi[i]);
        gtk_table_attach_defaults (GTK_TABLE (tablaBarras), datosBarra->marcosPi[i], 2, 3, i, i+1);

    }

}

/* Desplegar panel principal de la ventana */
void desplegarPanelPrincipal(BarraProgreso * datosBarra)
{
    GtkWidget *panelVertical;

    // Establecer el panel para los controles
    panelVertical = gtk_vbox_new (FALSE, 5);
    gtk_container_set_border_width (GTK_CONTAINER (panelVertical), 10);
    gtk_container_add (GTK_CONTAINER (datosBarra->ventana), panelVertical);

    // Desplegar panel informativo
    desplegarPanelInformativo(panelVertical);

    // Desplegar barras de progreso
    desplegarBarrasProgreso(panelVertical, datosBarra);

    // Desplegar boton para salir
    desplegarBotonSalir(panelVertical, datosBarra);

}

/* Despliega la ventana principal */
void desplegarThreads(void (*funcion)())
{
    BarraProgreso *datosBarra;

    // Inicializar memoria para los datos de barras de progreso
    datosBarra = g_malloc(sizeof(BarraProgreso));

    // Crear ventana principal
    datosBarra->ventana = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (datosBarra->ventana), "Calcular Pi");
    gtk_container_set_border_width (GTK_CONTAINER (datosBarra->ventana), 0);
    gtk_window_set_position(GTK_WINDOW (datosBarra->ventana), GTK_WIN_POS_CENTER);
    gtk_widget_set_size_request (datosBarra->ventana, 840, (NUM_HILOS < 8 ? 80 * (NUM_HILOS + 1) : 600));

    // Agregar manejador para finalizacion de la ventana
    g_signal_connect (G_OBJECT (datosBarra->ventana), "destroy",
        G_CALLBACK (finalizarDespliegue), (gpointer) datosBarra);

    // Desplegar panel con los controles
    desplegarPanelPrincipal(datosBarra);

    // Desplegar ventana principal y todos sus controles
    gtk_widget_show_all(datosBarra->ventana);

    // Funcion a invocar
    datosBarra->funcion = funcion;
    gtk_init_add(despliegueIniciado, (gpointer) datosBarra);

    // Ciclo principal
    gtk_main();

}
