#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct {
  unsigned short id;
  unsigned int suma_parcial_truncada;
  struct nodo_lista *siguiente;
} nodo_lista;

typedef struct {
  unsigned int longitud;
  char cadena[33];
} celda_buffer_t;

typedef struct {
  FILE *fichero_abierto;
  unsigned short tbuffer;
} arg_hilo_productor;

typedef struct {
  unsigned short tbuffer;
  unsigned short *i_c;
  unsigned short id_hilo;
  nodo_lista **nodo;
} arg_hilo_consumidor;

typedef struct {
  FILE *arc_resultados;
  nodo_lista *nodo;
} arg_hilo_sumador;

celda_buffer_t *buffer;

sem_t hay_dato;
sem_t hay_espacio;
sem_t mutex_c;
sem_t mutex_l;
sem_t hay_suma;

bool es_binario(char *cadena) {
  for (int i = 0; i < strlen(cadena); i++) {
    if (cadena[i] != '0' && cadena[i] != '1') {
      return false;
    }
  }

  return true;
}

int atobtoi(char *cadena) {
  int n = 0;

  for (int i = 0; i < strlen(cadena); i++) {
    if (cadena[i] == '1') {
      n += pow(2, strlen(cadena) - i - 1);
    }
  }

  return n;
}

void *productor(void *arg) {
  arg_hilo_productor *arg_p = (arg_hilo_productor *)arg;
  char *cadena = NULL;
  int i_p = 0;
  size_t tam_buffer_cadena;
  int tam_cadena;
  celda_buffer_t dato;
  while (getline(&cadena, &tam_buffer_cadena, arg_p->fichero_abierto) != -1) {

    cadena[strlen(cadena) - 1] = (cadena[strlen(cadena) - 1] == '\n') ? '\0' : cadena[strlen(cadena) - 1];
    tam_cadena = strlen(cadena);

    if (tam_cadena <= 32 && tam_cadena >= 1 && es_binario(cadena)) {
      sem_wait(&hay_espacio);
      strcpy(dato.cadena, cadena);
      dato.longitud = tam_cadena;
      buffer[i_p] = dato;
      i_p = (i_p + 1) % arg_p->tbuffer;

      sem_post(&hay_dato);
    }
  }
  sem_wait(&hay_espacio);
  dato.longitud = -1;
  strcpy(dato.cadena, "");
  buffer[i_p] = dato;
  sem_post(&hay_dato);

  pthread_exit(NULL);
}

void *consumidor(void *arg) {
  arg_hilo_consumidor *arg_c = (arg_hilo_consumidor *)arg;
  celda_buffer_t dato;
  long suma = 0;
  bool parada = false;

  nodo_lista **nodo = arg_c->nodo;

  while (!parada) {
    sem_wait(&hay_dato);
    sem_wait(&mutex_c);

    dato = buffer[*(arg_c->i_c)];
    if (dato.longitud != -1) {
      if (dato.cadena[0] != '1' && dato.longitud == 32) {

        if ((dato.cadena[dato.longitud - 1] == '0' && arg_c->id_hilo % 2 == 0) || (dato.cadena[dato.longitud - 1] == '1' && arg_c->id_hilo % 2 == 1)) {

          suma = (suma + atobtoi(dato.cadena)) % (RAND_MAX / 2);

          *(arg_c->i_c) = (*(arg_c->i_c) + 1) % arg_c->tbuffer;
          sem_post(&hay_espacio);

        } else {
          sem_post(&hay_dato);
        }

      } else {
        *(arg_c->i_c) = (*(arg_c->i_c) + 1) % arg_c->tbuffer;
        sem_post(&hay_espacio);
      }
    } else {
      sem_post(&hay_dato);
      parada = true;
    }
    sem_post(&mutex_c);
  }
  sem_wait(&mutex_l);
  (*nodo)->id = arg_c->id_hilo;
  (*nodo)->suma_parcial_truncada = suma;
  *nodo = (*nodo)->siguiente;
  sem_post(&hay_suma);
  sem_post(&mutex_l);
  pthread_exit(NULL);
}

void *sumador(void *arg) {
  arg_hilo_sumador *arg_s = (arg_hilo_sumador *)arg;

  nodo_lista *nodo = arg_s->nodo;
  long suma_total_truncada = 0;

  while (nodo != NULL) {
    sem_wait(&hay_suma);

    fprintf(arg_s->arc_resultados, "Hilo %d | Suma parcial: %d\n", nodo->id, nodo->suma_parcial_truncada);
    suma_total_truncada = (suma_total_truncada + nodo->suma_parcial_truncada) % (RAND_MAX / 2);
    nodo = nodo->siguiente;
  }

  fprintf(arg_s->arc_resultados, "Suma total: %d\n", suma_total_truncada);

  pthread_exit(NULL);
}

bool es_numero(char *cadenaLeida) {
  for (int i = 0; i < strlen(cadenaLeida); i++) {

    if (cadenaLeida[i] > '9' || cadenaLeida[i] < '0') {
      return false;
    }
  }

  return true;
}

nodo_lista *crear_nodos(unsigned short nhilos) {
  nodo_lista *inicio = (nodo_lista *)malloc(sizeof(nodo_lista));

  if (inicio == NULL) {
    perror("Hubo un error al asignar memoria para el nodo inicial de la lista enlazada");
    exit(EXIT_FAILURE);
  }

  inicio->id = 0;
  inicio->suma_parcial_truncada = 0;
  inicio->siguiente = NULL;

  nodo_lista *actual = inicio;

  for (unsigned short i = 1; i < nhilos; i++) {
    nodo_lista *nuevo_nodo = (nodo_lista *)malloc(sizeof(nodo_lista));
    if (nuevo_nodo == NULL) {
      perror("Hubo un error al asignar memoria para un nodo de la lista enlazada");
      exit(EXIT_FAILURE);
    }

    nuevo_nodo->id = i;
    nuevo_nodo->suma_parcial_truncada = 0;
    nuevo_nodo->siguiente = NULL;

    actual->siguiente = nuevo_nodo;
    actual = nuevo_nodo;
  }

  return inicio;
}

int main(int argc, char *argv[]) {

  FILE *fe;
  FILE *fs;
  FILE *fsum;

  if (argc != 6) {
    fprintf(stderr, "Error en el numero argumentos.\n");
    exit(EXIT_FAILURE);
  }

  fe = fopen(argv[1], "r");

  if (fe == NULL) {
    fprintf(stderr, "El primer fichero debe existir.\n");
    exit(EXIT_FAILURE);
  }

  fs = fopen(argv[2], "r");
  if (fs != NULL) {
    fprintf(stderr, "El segundo fichero no debe existir.\n");
    fclose(fs);
    fclose(fe);
    exit(EXIT_FAILURE);
  }

  char *hilos = argv[3];
  if (!es_numero(hilos)) {
    fprintf(stderr, "El parametro de hilos no es numerico.\n");
    fclose(fs);
    fclose(fe);
    exit(EXIT_FAILURE);
  }

  int nhilos = atoi(hilos);

  if (nhilos < 2 || nhilos > 1000) {
    fprintf(stderr, "El parametro de hilos debe ser un numero en el intervalo [2, 1000].\n");
    fclose(fs);
    fclose(fe);
    exit(EXIT_FAILURE);
  }

  char *tbuffer_str = argv[4];
  if (!es_numero(tbuffer_str)) {
    fprintf(stderr, "El parametro de hilos no es numerico.\n");
    fclose(fs);
    fclose(fe);
    exit(EXIT_FAILURE);
  }

  int tbuffer = atoi(tbuffer_str);

  if (tbuffer < 10 || tbuffer > 1000) {
    fprintf(stderr, "El parametro de tamaño de buffer debe ser un numero en el intervalo [10, 1000].\n");
    fclose(fs);
    fclose(fe);
    exit(EXIT_FAILURE);
  }

  fsum = fopen(argv[5], "w");
  if (fsum == NULL) {
    fprintf(stderr, "Hubo un problema al abrir el fichero de resultados.\n");
    fclose(fs);
    fclose(fe);
    exit(EXIT_FAILURE);
  }

  int pid = fork();

  if (pid == 0) {
    char *path = "./procesa.out";
    char *comando = "./procesa.out";
    char *arg1 = argv[1];
    char *arg2 = argv[2];

    execl(path, comando, arg1, arg2, NULL);
    perror("Algo fue mal en el procesado");
    exit(EXIT_FAILURE);

  } else {
    int estado;
    if (wait(&estado) == -1) {
      perror("Error al esperar al hijo");
      return EXIT_FAILURE;
    }

    if (!WIFEXITED(estado)) {
      fprintf(stderr, "El proceso hijo terminó de forma anormal.\n");
      return EXIT_FAILURE;
    }

    fs = fopen(argv[2], "r");

    if (fs == NULL) {
      perror("Ocurrio un error al intentar abrir el segundo fichero despues de ejecutar procesa");
      exit(EXIT_FAILURE);
    }

    buffer = (celda_buffer_t *)malloc(sizeof(celda_buffer_t) * tbuffer);
    if (buffer == NULL) {
      perror("Hubo un problema al intentar asignar memoria para el buffer de datos");
      fclose(fs);
      exit(EXIT_FAILURE);
    }

    sem_init(&hay_dato, 0, 0);
    sem_init(&hay_espacio, 0, tbuffer);
    sem_init(&mutex_c, 0, 1);
    sem_init(&mutex_l, 0, 1);
    sem_init(&hay_suma, 0, 0);

    arg_hilo_productor arg_p;
    arg_p.fichero_abierto = fs;
    arg_p.tbuffer = tbuffer;

    pthread_t id_productor;

    arg_hilo_consumidor *arg_c = (arg_hilo_consumidor *)malloc(sizeof(arg_hilo_consumidor) * nhilos);

    if (arg_c == NULL) {
      perror("Hubo un problema al intentar asignar memoria para el array de argumentos de los consumidores");
      fclose(fs);
      exit(EXIT_FAILURE);
    }

    pthread_t *ids_consumidor = (pthread_t *)malloc(sizeof(pthread_t) * nhilos);

    if (ids_consumidor == NULL) {
      perror("Hubo un problema al intentar asignar memoria para el array de ids de los consumidores");
      fclose(fs);
      exit(EXIT_FAILURE);
    }

    pthread_t id_sumador;

    nodo_lista *nodo_inicial = crear_nodos(nhilos);
    nodo_lista *nodo_inicial_consumidor = nodo_inicial;

    unsigned short _ = 0;
    for (unsigned short i = 0; i < nhilos; i++) {
      arg_c[i].tbuffer = tbuffer;
      arg_c[i].nodo = &nodo_inicial_consumidor;
      arg_c[i].i_c = &_;
      arg_c[i].id_hilo = i;
    }

    nodo_lista *nodo_incial_sumador = nodo_inicial;

    arg_hilo_sumador arg_s;
    arg_s.arc_resultados = fsum;
    arg_s.nodo = nodo_incial_sumador;

    pthread_create(&id_productor, NULL, productor, (void *)&arg_p);
    for (unsigned short i = 0; i < nhilos; i++) {
      pthread_create(&ids_consumidor[i], NULL, consumidor, (void *)&arg_c[i]);
    }
    pthread_create(&id_sumador, NULL, sumador, (void *)&arg_s);

    pthread_join(id_productor, NULL);
    for (unsigned short i = 0; i < nhilos; i++) {
      pthread_join(ids_consumidor[i], NULL);
    }
    pthread_join(id_sumador, NULL);

    sem_destroy(&hay_dato);
    sem_destroy(&hay_espacio);
    sem_destroy(&mutex_c);
    sem_destroy(&mutex_l);
    sem_destroy(&hay_suma);

    free(ids_consumidor);
    free(buffer);
    fclose(fs);
  }

  printf("main: Procesado de fichero terminado.\n");

  return EXIT_SUCCESS;
}