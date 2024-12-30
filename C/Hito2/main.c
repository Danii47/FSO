/**
 * @author Hugo Adan de la Fuente y Daniel Fernandez Varona
 */

#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * Estructura que se almacenara en cada posicion del buffer circular
 *
 * Los datos que contiene son:
 * `unsigned char` `longitud` - longitud de la cadena almacenada (el numero maximo sera 32)
 * `char[33]` `cadena` - cadena de caracteres almacenada en el buffer (la cadena tendra [1, 32] caracteres,
 * por lo que la longitud maxima es 33 contando el caracter fin de cadena)
 */
typedef struct {
  unsigned char longitud;
  char cadena[33];
} celda_buffer_t;

/**
 * Estructura que contiene los argumentos para el hilo productor
 *
 * Los datos que contiene son:
 * `FILE *` `fichero_abierto` - un puntero a un fichero abierto
 * `unsigned short` `tamano_buffer` - tamano del buffer circular
 */
typedef struct {
  FILE *fichero_abierto;
  unsigned short tamano_buffer;
} arg_hilo_productor;

/**
 * Estructura que contiene los argumentos para el hilo consumidor
 *
 * Los datos que contiene son:
 * `unsigned short` `tamano_buffer` - tamano del buffer circular
 * `unsigned short *` `indice_consumidor` - direccion de memoria donde se encuentra el indice al que acceder en el buffer, todos los hilos consumidores acceden a esa direccion
 * de memoria `int *` `resultado` - array de resultados del hilo consumidor, donde cada hilo dejara el resultado de su suma
 */
typedef struct {
  unsigned short tamano_buffer;
  unsigned short *indice_consumidor;
  int *resultado;
} arg_hilo_consumidor;

/**
 * Buffer circular donde el hilo productor guardara datos y los hilos consumidores los consumiran
 */
celda_buffer_t *buffer;

/**
 * Semaforo que representa si hay algun dato consumible en el buffer, en caso de no haber los hilos consumidores quedan bloqueados
 */
sem_t hay_dato;

/**
 * Semaforo que representa si hay espacio disponible en el buffer, en caso de no haber espacio el hilo productor se queda bloqueado
 */
sem_t hay_espacio;

/**
 * Semaforo binario que controla el acceso a la zona critica de los hilos consumidores, se encarga de que no intenten acceder dos hilos consumidores a la vez al mismo indice
 */
sem_t mutex_buffer;

/**
 * Funcion que comprueba si una cadena es un numero binario
 *
 * @param cadena puntero al inicio de la cadena donde esta el string
 *
 * @return true en caso de que solo contenga 1's y 0's
 */
bool es_binario(char *cadena) {
  for (int i = 0; i < strlen(cadena); i++) {
    if (cadena[i] != '0' && cadena[i] != '1') {
      return false;
    }
  }

  return true;
}

/**
 * ASCII to BINARY to INTEGER
 *
 * Convierte un string que solo contiene numeros binarios a su equivalente en decimal en entero
 *
 * @param cadena puntero al inicio de la cadena donde esta el string
 *
 * @return numero entero correspondiente a la conversion
 */
int atobintoi(char *cadena) {
  int n = 0;

  for (int i = 0; i < strlen(cadena); i++) {
    if (cadena[i] == '1') {
      n += pow(2, strlen(cadena) - i - 1);
    }
  }

  return n;
}

/**
 * Funcion del hilo productor, se encarga de leer un fichero,
 * comprobar que la longitud de la linea esta en el intervalo [1, 32] (excluyendo el salto de linea),
 * que el numero sea binario y lo almacena en el buffer circular, informando a traves de los semaforos
 * que hay un hueco menos en el buffer y un dato mas
 *
 * Al terminar de leer el fichero anade un dato al final que contiene longitud 255 (una situacion imposible)
 * para marcar que ya no se deben leer mas datos
 *
 * @param arg del tipo `arg_hilo_productor *` contiene todos los argumentos necesarios para el hilo productor
 */
void *productor(void *arg) {
  arg_hilo_productor *argumentos_hilo_productor = (arg_hilo_productor *)arg;

  char *cadena = NULL;
  
  int indice_productor = 0;
  unsigned int tamano_cadena;

  unsigned short tamano_buffer = argumentos_hilo_productor->tamano_buffer;
  FILE *fichero_abierto = fichero_abierto;

  size_t tam_buffer_cadena;
  celda_buffer_t dato;
  
  while (getline(&cadena, &tam_buffer_cadena, fichero_abierto) != -1) {

    if (cadena[strlen(cadena) - 1] == '\n') {
      cadena[strlen(cadena) - 1] = '\0';
    }
    
    tamano_cadena = strlen(cadena);

    if (tamano_cadena >= 1 && tamano_cadena <= 32 && es_binario(cadena)) {
      sem_wait(&hay_espacio);
      strcpy(dato.cadena, cadena);
      dato.longitud = tamano_cadena;
      buffer[indice_productor] = dato;
      indice_productor = (indice_productor + 1) % tamano_buffer;

      sem_post(&hay_dato);
    }
  }
  sem_wait(&hay_espacio);
  dato.longitud = 255;
  buffer[indice_productor] = dato;
  sem_post(&hay_dato);

  pthread_exit(NULL);
}

/**
 * Funcion de los hilos consumidores, se encarga de leer del buffer,
 * comprobar que el numero binario leido no sea negativo (el primer bit distinto de 1),
 * que la longitud de la cadena sea exactamente 32, informando a traves de los semaforos
 * que hay un hueco mas en el buffer y un dato menos
 *
 * Este bucle ocurre hasta que se encuentra con una cadena de longitud igual a 255 (situacion imposible que genera el hilo productor al acabar de leer todos los datos)
 * para marcar que ya no hay mas datos que leer y que ese hilo consumidor pueda finalizar
 *
 * A continuacion el hilo consumidor almacena su suma truncada en el array de resultados
 *
 * @param arg del tipo `arg_hilo_consumidor *` contiene todos los argumentos necesarios para el hilo consumidor
 */
void *consumidor(void *arg) {

  arg_hilo_consumidor *argumentos_hilo_consumidor = (arg_hilo_consumidor *)arg;
  celda_buffer_t dato;

  unsigned short* indice_consumidor = argumentos_hilo_consumidor->indice_consumidor;
  unsigned short tamano_buffer = argumentos_hilo_consumidor->tamano_buffer;

  int suma = 0;
  unsigned char parada = 0;

  while (!parada) {
    sem_wait(&hay_dato);
    sem_wait(&mutex_buffer);
    dato = buffer[*(indice_consumidor)];
    if (dato.longitud != 255) {
      if (dato.cadena[0] == '0' && dato.longitud == 32) {
        suma = (suma + atobintoi(dato.cadena)) % (RAND_MAX / 2);
      }
      *(indice_consumidor) = (*(indice_consumidor) + 1) % tamano_buffer;
      sem_post(&hay_espacio);
    } else {
      sem_post(&hay_dato);
      parada = 1;
    }
    sem_post(&mutex_buffer);
  }
  *(argumentos_hilo_consumidor->resultado) = suma;
  pthread_exit(NULL);
}

/**
 * Funcion que comprueba si una cadena de caracteres esta compuesta unicamente por numeros
 *
 * @param cadena puntero al inicio de la cadena donde esta el string
 *
 * @return true si la cadena esta unicamente compuesta por numeros
 */
unsigned char todo_numeros(char *cadena) {
  for (int i = 0; i < strlen(cadena); i++) {

    if (cadena[i] > '9' || cadena[i] < '0') {
      return 0;
    }
  }

  return 1;
}

/**
 * Funcion principal del programa
 *
 * @param argc numero de argumentos en la llamada al programa
 * @param argv matriz de caracteres con los valores de cada argumento
 *
 * Los argumentos en el Hito 2 (5) son:
 * 0: ejecucion del programa (./main.out)
 * 1: nombre del fichero de entrada (ejemplo100.txt)
 * 2: nombre del fichero de salida (salida.txt)
 * 3: numero de hilos ([2, 1000])
 * 4: tamano del buffer ([10, 1000])
 *
 * @return EXIT_SUCCESS en caso de que no haya ningun error
 */
int main(int argc, char *argv[]) {

  FILE *fichero_entrada_procesa;
  FILE *fichero_salida_procesa;

  if (argc != 5) {
    fprintf(stderr, "Error en el numero argumentos.\n\n");
    exit(EXIT_FAILURE);
  }

  fichero_entrada_procesa = fopen(argv[1], "r");

  if (fichero_entrada_procesa == NULL) {
    fprintf(stderr, "El primer fichero debe existir\n");
    exit(EXIT_FAILURE);
  }

  fichero_salida_procesa = fopen(argv[2], "r");
  if (fichero_salida_procesa != NULL) {
    fprintf(stderr, "El segundo fichero no debe existir\n");
    fclose(fichero_salida_procesa);
    fclose(fichero_entrada_procesa);
    exit(EXIT_FAILURE);
  }

  char *hilos = argv[3];
  if (!todo_numeros(hilos)) {
    fprintf(stderr, "El parametro de hilos no es numerico.\n");
    fclose(fichero_entrada_procesa);
    return EXIT_FAILURE;
  }

  int numero_hilos = atoi(hilos);

  if (numero_hilos < 2 || numero_hilos > 1000) {
    fprintf(stderr, "El parametro de hilos debe ser un numero en el intervalo [2, 1000].\n");
    fclose(fichero_entrada_procesa);
    exit(EXIT_FAILURE);
  }

  char *string_tamano_buffer = argv[4];
  if (!todo_numeros(string_tamano_buffer)) {
    fprintf(stderr, "El parametro de tamano de buffer no es numerico.\n");
    fclose(fichero_entrada_procesa);
    return EXIT_FAILURE;
  }

  int tamano_buffer = atoi(string_tamano_buffer);

  if (tamano_buffer < 10 || tamano_buffer > 1000) {
    fprintf(stderr, "El parametro de tamaño de buffer debe ser un numero en el intervalo [10, 1000].\n");
    fclose(fichero_entrada_procesa);
    exit(EXIT_FAILURE);
  }

  pid_t id_proceso = fork();

  if (id_proceso == -1) {
    perror("Error al hacer fork");
    return EXIT_FAILURE;

  } else if (id_proceso == 0) {
    char *ruta_proceso = "./procesa";
    char *comando = "./procesa";
    char *argumento_1 = argv[1];
    char *argumento_2 = argv[2];

    execl(ruta_proceso, comando, argumento_1, argumento_2, NULL);
    perror("Algo fue mal en el procesado");
    exit(EXIT_FAILURE);

  } else {
    int estado;
    if (wait(&estado) == -1) {
      perror("Error al esperar al hijo");
      exit(EXIT_FAILURE);
    }

    if (!WIFEXITED(estado)) {
      fprintf(stderr, "El proceso hijo terminó de forma anormal.\n");
      exit(EXIT_FAILURE);
    }

    fichero_salida_procesa = fopen(argv[2], "r");

    if (fichero_salida_procesa == NULL) {
      perror("Ocurrio un error al intentar abrir el segundo fichero despues de ejecutar procesa");
      exit(EXIT_FAILURE);
    }

    // Creacion del buffer circular de datos
    buffer = (celda_buffer_t *)malloc(sizeof(celda_buffer_t) * tamano_buffer);
    if (buffer == NULL) {
      perror("Hubo un problema al intentar asignar memoria para el buffer de datos");
      fclose(fichero_salida_procesa);
      exit(EXIT_FAILURE);
    }
    //

    // Inicializacion de semaforos
    sem_init(&hay_dato, 0, 0);
    sem_init(&hay_espacio, 0, tamano_buffer);
    sem_init(&mutex_buffer, 0, 1);

    // Hilo productor
    pthread_t id_productor;

    // Asignacion argumentos hilo productor
    arg_hilo_productor arg_p;
    arg_p.fichero_abierto = fichero_salida_procesa;
    arg_p.tamano_buffer = tamano_buffer;
    //

    // Hilos consumidores
    pthread_t *ids_consumidores = (pthread_t *)malloc(sizeof(pthread_t) * numero_hilos);

    if (ids_consumidores == NULL) {
      fprintf(stderr, "No fue posible asignar la memoria.");
      fclose(fichero_salida_procesa);
      exit(EXIT_FAILURE);
    }

    arg_hilo_consumidor *argumentos_consumidores = (arg_hilo_consumidor *)malloc(sizeof(arg_hilo_consumidor) * numero_hilos);

    if (argumentos_consumidores == NULL) {
      perror("Hubo un problema al intentar asignar memoria para el array de argumentos de los consumidores");
      free(ids_consumidores);
      fclose(fichero_salida_procesa);
      exit(EXIT_FAILURE);
    }

    int *array_resultados = (int *)malloc(sizeof(int) * numero_hilos);

    if (array_resultados == NULL) {
      perror("Hubo un problema al intentar asignar memoria para el array de argumentos de resultados");
      free(ids_consumidores);
      free(argumentos_consumidores);
      fclose(fichero_salida_procesa);
      exit(EXIT_FAILURE);
    }

    // Asignacion argumentos hilos consumidores
    unsigned short _ = 0;
    for (unsigned short i = 0; i < numero_hilos; i++) {
      argumentos_consumidores[i].tamano_buffer = tamano_buffer;
      argumentos_consumidores[i].resultado = &array_resultados[i];
      argumentos_consumidores[i].indice_consumidor = &_;
    }
    //

    // Creacion de los hilos productor y consumidores
    pthread_create(&id_productor, NULL, productor, (void *)&arg_p);
    for (unsigned short i = 0; i < numero_hilos; i++) {
      pthread_create(&ids_consumidores[i], NULL, consumidor, (void *)&argumentos_consumidores[i]);
    }
    //

    // Espera de los hilos productor y consumidores
    pthread_join(id_productor, NULL);
    for (unsigned short i = 0; i < numero_hilos; i++) {
      pthread_join(ids_consumidores[i], NULL);
    }
    //

    // Mostrado de sumas parciales truncadas de cada hilo
    for (unsigned short i = 0; i < numero_hilos; i++) {
      printf("Hilo %d: %d\n", i, array_resultados[i]);
    }
    //

    // Mostrado de la suma total truncada
    int suma = 0;
    for (unsigned short i = 0; i < numero_hilos; i++) {
      suma = (suma + array_resultados[i]) % (RAND_MAX / 2);
    }
    printf("Suma total truncada: %ld\n", suma);
    //

    // Destruccion de semaforos
    sem_destroy(&hay_dato);
    sem_destroy(&hay_espacio);
    sem_destroy(&mutex_buffer);

    // Liberacion de memoria
    free(ids_consumidores);
    free(argumentos_consumidores);
    free(buffer);

    // Cerrado de ficheros
    fclose(fichero_salida_procesa);
  }

  printf("main: Procesado de fichero terminado.\n");
  return EXIT_SUCCESS;
}