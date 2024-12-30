/**
 * @author Hugo Adan de la Fuente y Daniel Fernandez Varona
 */

#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * Estructura que almacena un nodo de la lista enlazada
 *
 * Los datos que contiene son:
 * `unsigned short` `id` - id del hilo
 * `unsigned int` `suma_parcial_truncada` - suma parcial truncada del hilo
 * `struct nodo_lista *` `siguiente_nodo` - puntero al siguiente nodo, del mismo tipo
 */
typedef struct nodo_lista {
  unsigned short id;
  unsigned int suma_parcial_truncada;
  struct nodo_lista *siguiente_nodo;
} nodo_lista;

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
 * `unsigned short` `id_hilo` - indice del hilo lanzado
 * `unsigned short *` `indice_consumidor` - direccion de memoria donde se encuentra el indice al que acceder en el buffer, todos los hilos consumidores acceden a esa direccion
 * de memoria `nodo_lista **` `nodo` - direccion de memoria a una direccion de memoria que apunta a un nodo de la lista enlazada
 */
typedef struct {
  unsigned short tamano_buffer;
  unsigned short id_hilo;
  unsigned short *indice_consumidor;
  nodo_lista **nodo;
} arg_hilo_consumidor;

/**
 * Estructura que contiene los argumentos para el hilo sumador
 *
 * Los datos que contiene son:
 * `FILE *` `arc_resultados` - un puntero a un fichero abierto donde almacenar los resultados
 * `nodo_lista *` `nodo` - direccion de memoria a un nodo de la lista enlazada
 */
typedef struct {
  FILE *arc_resultados;
  nodo_lista *nodo;
} arg_hilo_sumador;

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
 * Semaforo binario que controla el acceso a la zona critica donde los hilos consumidores acceden a la lista enlazada
 */
sem_t mutex_lista_enlazada;

/**
 * Semaforo que representa si hay dato en la lista enlazada, en caso de no haber el hilo sumador queda bloqueado esperando
 */
sem_t hay_suma;

/**
 * Funcion que comprueba si una cadena es un numero binario
 *
 * @param cadena puntero al inicio de la cadena donde esta el string
 *
 * @return 1 en caso de que solo contenga 1's y 0's
 */
unsigned char es_binario(char *cadena) {
  for (int i = 0; i < strlen(cadena); i++) {
    if (cadena[i] != '0' && cadena[i] != '1') {
      return 0;
    }
  }

  return 1;
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
int atobtoi(char *cadena) {
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

  size_t tam_buffer_cadena;
  celda_buffer_t dato;

  while (getline(&cadena, &tam_buffer_cadena, argumentos_hilo_productor->fichero_abierto) != -1) {
    cadena[strlen(cadena) - 1] = (cadena[strlen(cadena) - 1] == '\n') ? '\0' : cadena[strlen(cadena) - 1];
    tamano_cadena = strlen(cadena);

    if (tamano_cadena <= 32 && tamano_cadena >= 1 && es_binario(cadena)) {
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
 * Solo consume el dato si:
 * - el numero sea par (termine en 0) y el id del hilo sea par
 * - el numero sea impar (termine en 1) y el id del hilo sea impar
 *
 * en caso contrario no lo consume ni mueve el indice de acceso
 *
 * Este bucle ocurre hasta que se encuentra con una cadena de longitud igual a 255 (situacion imposible que genera el hilo productor al acabar de leer todos los datos)
 * para marcar que ya no hay mas datos que leer y que ese hilo consumidor pueda finalizar
 *
 * A continuacion el hilo consumidor almacena su suma truncada en nodo correspondiente de la lista enlazada
 *
 * @param arg del tipo `arg_hilo_consumidor *` contiene todos los argumentos necesarios para el hilo consumidor
 */
void *consumidor(void *arg) {
  arg_hilo_consumidor *argumentos_hilo_consumidor = (arg_hilo_consumidor *)arg;
  celda_buffer_t dato;
  int suma = 0;

  unsigned short id_hilo = argumentos_hilo_consumidor->id_hilo;
  unsigned short *indice_consumidor = argumentos_hilo_consumidor->indice_consumidor;
  unsigned short tamano_buffer = argumentos_hilo_consumidor->tamano_buffer;

  unsigned short parada = 0;

  nodo_lista **nodo = argumentos_hilo_consumidor->nodo;

  while (!parada) {
    sem_wait(&hay_dato);
    sem_wait(&mutex_buffer);

    dato = buffer[*(indice_consumidor)];
    if (dato.longitud != 255) {
      if (dato.cadena[0] != '1' && dato.longitud == 32) {

        if ((dato.cadena[dato.longitud - 1] == '0' && id_hilo % 2 == 0) || (dato.cadena[dato.longitud - 1] == '1' && id_hilo % 2 == 1)) {

          suma = (suma + atobtoi(dato.cadena)) % (RAND_MAX / 2);

          *(indice_consumidor) = (*(indice_consumidor) + 1) % tamano_buffer;
          sem_post(&hay_espacio);

        } else {
          sem_post(&hay_dato);
        }

      } else {
        *(indice_consumidor) = (*(indice_consumidor) + 1) % tamano_buffer;
        sem_post(&hay_espacio);
      }
    } else {
      sem_post(&hay_dato);
      parada = 1;
    }
    sem_post(&mutex_buffer);
  }
  sem_wait(&mutex_lista_enlazada);
  (*nodo)->id = id_hilo;
  (*nodo)->suma_parcial_truncada = suma;
  *nodo = (*nodo)->siguiente_nodo;
  sem_post(&hay_suma);
  sem_post(&mutex_lista_enlazada);
  pthread_exit(NULL);
}

/**
 * Funcion de los hilos sumadores, se encarga de leer de la lista dinamica
 *
 * Este bucle ocurre hasta que se encuentra con un nodo que vale NULL, eso significa que ya habran acabado todos los hilos
 *
 * A continuacion el hilo sumador guarda la suma total truncada en el fichero de resultados
 *
 * @param arg del tipo `arg_hilo_sumador *` contiene todos los argumentos necesarios para el hilo sumador
 */
void *sumador(void *arg) {
  arg_hilo_sumador *argumentos_hilo_sumador = (arg_hilo_sumador *)arg;

  nodo_lista *nodo = argumentos_hilo_sumador->nodo;
  int suma_total_truncada = 0;

  while (nodo != NULL) {
    sem_wait(&hay_suma);

    fprintf(argumentos_hilo_sumador->arc_resultados, "Hilo %d | Suma parcial: %d\n", nodo->id, nodo->suma_parcial_truncada);
    suma_total_truncada = (suma_total_truncada + nodo->suma_parcial_truncada) % (RAND_MAX / 2);
    nodo = nodo->siguiente_nodo;
  }

  fprintf(argumentos_hilo_sumador->arc_resultados, "Suma total: %d\n", suma_total_truncada);

  pthread_exit(NULL);
}

/**
 * Funcion que comprueba si una cadena de caracteres esta compuesta unicamente por numeros
 *
 * @param cadena puntero al inicio de la cadena donde esta el string
 *
 * @return 1 si la cadena esta unicamente compuesta por numeros
 */
unsigned char es_numero(char *cadena) {
  for (int i = 0; i < strlen(cadena); i++) {

    if (cadena[i] > '9' || cadena[i] < '0') {
      return 0;
    }
  }

  return 1;
}

/**
 * Funcion que se encarga de crear la lista enlazada
 *
 * @param numero_nodos numero de nodos en la lista enlazada, coincide con el numero de hilos
 *
 * @return un puntero al primer nodo de la lista enlazada
 */
nodo_lista *crear_nodos(unsigned short numero_nodos) {
  nodo_lista *inicio = (nodo_lista *)malloc(sizeof(nodo_lista));

  if (inicio == NULL) {
    perror("Hubo un error al asignar memoria para el nodo inicial de la lista enlazada");
    exit(EXIT_FAILURE);
  }

  inicio->id = 0;
  inicio->suma_parcial_truncada = 0;
  inicio->siguiente_nodo = NULL;

  nodo_lista *actual = inicio;

  for (unsigned short i = 1; i < numero_nodos; i++) {
    nodo_lista *nuevo_nodo = (nodo_lista *)malloc(sizeof(nodo_lista));
    if (nuevo_nodo == NULL) {
      perror("Hubo un error al asignar memoria para un nodo de la lista enlazada");
      exit(EXIT_FAILURE);
    }

    nuevo_nodo->id = i;
    nuevo_nodo->suma_parcial_truncada = 0;
    nuevo_nodo->siguiente_nodo = NULL;

    actual->siguiente_nodo = nuevo_nodo;
    actual = nuevo_nodo;
  }

  return inicio;
}

/**
 * Funcion principal del programa
 *
 * @param argc numero de argumentos en la llamada al programa
 * @param argv matriz de caracteres con los valores de cada argumento
 *
 * Los argumentos en el Hito 3 (6) son:
 * 0: ejecucion del programa (./main.out)
 * 1: nombre del fichero de entrada (ejemplo100.txt)
 * 2: nombre del fichero de salida (salida.txt)
 * 3: numero de hilos ([2, 1000])
 * 4: tamano del buffer ([10, 1000])
 * 5: nombre del fichero de resultados (resultados.txt)
 *
 * @return 0 en caso de que no haya ningun error
 */
int main(int argc, char *argv[]) {

  FILE *fichero_entrada_procesa;
  FILE *fichero_salida_procesa;
  FILE *fichero_resultados;

  if (argc != 6) {
    fprintf(stderr, "Error en el numero argumentos.\n");
    exit(EXIT_FAILURE);
  }

  fichero_entrada_procesa = fopen(argv[1], "r");

  if (fichero_entrada_procesa == NULL) {
    fprintf(stderr, "El primer fichero debe existir.\n");
    exit(EXIT_FAILURE);
  }

  fichero_salida_procesa = fopen(argv[2], "r");
  if (fichero_salida_procesa != NULL) {
    fprintf(stderr, "El segundo fichero no debe existir.\n");
    fclose(fichero_salida_procesa);
    fclose(fichero_entrada_procesa);
    exit(EXIT_FAILURE);
  }

  char *hilos = argv[3];
  if (!es_numero(hilos)) {
    fprintf(stderr, "El parametro de hilos no es numerico.\n");
    fclose(fichero_entrada_procesa);
    exit(EXIT_FAILURE);
  }

  int numero_hilos = atoi(hilos);

  if (numero_hilos < 2 || numero_hilos > 1000) {
    fprintf(stderr, "El parametro de hilos debe ser un numero en el intervalo [2, 1000].\n");
    fclose(fichero_entrada_procesa);
    exit(EXIT_FAILURE);
  }

  char *string_tamano_buffer = argv[4];
  if (!es_numero(string_tamano_buffer)) {
    fprintf(stderr, "El parametro de tamano de buffer no es numerico.\n");
    fclose(fichero_entrada_procesa);
    exit(EXIT_FAILURE);
  }

  int tamano_buffer = atoi(string_tamano_buffer);

  if (tamano_buffer < 10 || tamano_buffer > 1000) {
    fprintf(stderr, "El parametro de tamaño de buffer debe ser un numero en el intervalo [10, 1000].\n");
    fclose(fichero_entrada_procesa);
    exit(EXIT_FAILURE);
  }

  fichero_resultados = fopen(argv[5], "w");
  if (fichero_resultados == NULL) {
    fprintf(stderr, "Hubo un problema al abrir el fichero de resultados.\n");
    fclose(fichero_entrada_procesa);
    exit(EXIT_FAILURE);
  }

  pid_t id_proceso = fork();

  if (id_proceso == -1) {
    perror("Error al hacer fork");
    return EXIT_FAILURE;

  } else if (id_proceso == 0) {
    char *ruta_proceso = "./procesa.out";
    char *comando = "./procesa.out";
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
    sem_init(&mutex_lista_enlazada, 0, 1);
    sem_init(&hay_suma, 0, 0);

    // Hilo productor
    pthread_t id_productor;

    // Asignacion argumentos hilo productor
    arg_hilo_productor argumentos_productor;
    argumentos_productor.fichero_abierto = fichero_salida_procesa;
    argumentos_productor.tamano_buffer = tamano_buffer;
    //

    nodo_lista *nodo_inicial = crear_nodos(numero_hilos);

    // Hilos consumidores
    pthread_t *ids_consumidores = (pthread_t *)malloc(sizeof(pthread_t) * numero_hilos);

    if (ids_consumidores == NULL) {
      perror("Hubo un problema al intentar asignar memoria para el array de ids de los consumidores");
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

    nodo_lista *nodo_inicial_consumidor = nodo_inicial;

    // Asignacion argumentos hilos consumidores
    unsigned short _ = 0;
    for (unsigned short i = 0; i < numero_hilos; i++) {
      argumentos_consumidores[i].tamano_buffer = tamano_buffer;
      argumentos_consumidores[i].nodo = &nodo_inicial_consumidor;
      argumentos_consumidores[i].indice_consumidor = &_;
      argumentos_consumidores[i].id_hilo = i;
    }
    //

    // Hilo sumador
    pthread_t id_sumador;

    // Asignacion argumentos hilo sumador
    nodo_lista *nodo_incial_sumador = nodo_inicial;

    arg_hilo_sumador argumentos_sumador;
    argumentos_sumador.arc_resultados = fichero_resultados;
    argumentos_sumador.nodo = nodo_incial_sumador;
    //

    // Creacion de los hilos productor, consumidores y sumador
    pthread_create(&id_productor, NULL, productor, (void *)&argumentos_productor);
    for (unsigned short i = 0; i < numero_hilos; i++) {
      pthread_create(&ids_consumidores[i], NULL, consumidor, (void *)&argumentos_consumidores[i]);
    }
    pthread_create(&id_sumador, NULL, sumador, (void *)&argumentos_sumador);
    //

    // Espera de los hilos productor, consumidores y sumador
    pthread_join(id_productor, NULL);
    for (unsigned short i = 0; i < numero_hilos; i++) {
      pthread_join(ids_consumidores[i], NULL);
    }
    pthread_join(id_sumador, NULL);
    //

    // Destruccion de semaforos
    sem_destroy(&hay_dato);
    sem_destroy(&hay_espacio);
    sem_destroy(&mutex_buffer);
    sem_destroy(&mutex_lista_enlazada);
    sem_destroy(&hay_suma);

    // Liberacion de memoria
    // TODO: Liberar la memoria de la lista enlazada
    free(ids_consumidores);
    free(argumentos_consumidores);
    free(buffer);

    // Cerrado de ficheros
    fclose(fichero_salida_procesa);
    fclose(fichero_resultados);
  }

  printf("main: Procesado de fichero terminado.\n");
  return EXIT_SUCCESS;
}