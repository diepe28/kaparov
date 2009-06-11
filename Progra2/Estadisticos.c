
/**
  *  Estadísticos
  */
unsigned long int tiempoEsperaEnSerAtendido = 0;
unsigned long int cantidadDeVecesQueEsIgnorado = 0;
unsigned long int tiempoEnRecibirArchivo = 0;
unsigned long int cantidadDeVecesAtendido = 0;
unsigned long int cantidadDeBytesRecibidos = 0;

/**
  *  Semáforos
  */
pthread_mutex_t esperaRecibir;
pthread_mutex_t esperaConexion;