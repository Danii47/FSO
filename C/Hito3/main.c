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

celda_buffer_t *buffer;

sem_t hay_dato;
sem_t hay_espacio;
sem_t mutex_c;
sem_t mutex_l;

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
  sem_post(&mutex_l);
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
    perror("Error al asignar memoria\n");
    exit(EXIT_FAILURE);
  }

  inicio->id = 0;
  inicio->suma_parcial_truncada = 0;
  inicio->siguiente = NULL;

  nodo_lista *actual = inicio;

  for (unsigned short i = 1; i < nhilos; i++) {
    nodo_lista *nuevo_nodo = (nodo_lista *)malloc(sizeof(nodo_lista));
    if (nuevo_nodo == NULL) {
      perror("Error al asignar memoria");
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

  if (argc != 5) {
    fprintf(stderr, "Error en argumentos\n");
    exit(1);
  }

  fe = fopen(argv[1], "r");

  if (fe == NULL) {
    fprintf(stderr, "El primer fichero debe existir\n");
    exit(1);
  }

  fs = fopen(argv[2], "r");
  if (fs != NULL) {
    fprintf(stderr, "El segundo fichero no debe existir\n");
    fclose(fs);
    fclose(fe);
    exit(1);
  }

  char *hilos = argv[3];
  if (!es_numero(hilos)) {
    fprintf(stderr, "El parametro de hilos no es numerico.\n");
    fclose(fs);
    fclose(fe);
    exit(1);
  }

  int nhilos = atoi(hilos);

  if (nhilos < 2 || nhilos > 1000) {
    fprintf(stderr, "El parametro de hilos debe ser un numero en el intervalo [2, 1000].\n");
    fclose(fs);
    fclose(fe);
    exit(1);
  }

  char *tbuffer_str = argv[4];
  if (!es_numero(tbuffer_str)) {
    fprintf(stderr, "El parametro de hilos no es numerico.\n");
    fclose(fs);
    fclose(fe);
    exit(1);
  }

  int tbuffer = atoi(tbuffer_str);

  if (tbuffer < 10 || tbuffer > 1000) {
    fprintf(stderr, "El parametro de tamaño de buffer debe ser un numero en el intervalo [10, 1000].\n");
    fclose(fs);
    fclose(fe);
    exit(1);
  }

  int pid = fork();

  if (pid == 0) {
    char *path = "./procesa.out";
    char *comando = "./procesa.out";
    char *arg1 = argv[1];
    char *arg2 = argv[2];

    int estado = execl(path, comando, arg1, arg2, NULL);
    if (estado == -1) {
      fprintf(stderr, "Algo fue mal en el procesado.\n");
      exit(1);
    }
  } else {
    wait(NULL);

    fs = fopen(argv[2], "r");

    if (fs == NULL) {
      fprintf(stderr, "El segundo fichero debe existir despues de ejecutar procesa\n");
      exit(1);
    }

    buffer = (celda_buffer_t *)malloc(sizeof(celda_buffer_t) * tbuffer);
    if (buffer == NULL) {
      fprintf(stderr, "No fue posible asignar la memoria.");
      fclose(fs);
      exit(1);
    }

    sem_init(&hay_dato, 0, 0);
    sem_init(&hay_espacio, 0, tbuffer);
    sem_init(&mutex_c, 0, 1);
    sem_init(&mutex_l, 0, 1);

    arg_hilo_productor arg_p;
    arg_p.fichero_abierto = fs;
    arg_p.tbuffer = tbuffer;

    pthread_t id_productor;

    arg_hilo_consumidor *arg_c = (arg_hilo_consumidor *)malloc(sizeof(arg_hilo_consumidor) * nhilos);

    pthread_t *ids_consumidor = (pthread_t *)malloc(sizeof(pthread_t) * nhilos);

    if (ids_consumidor == NULL) {
      fprintf(stderr, "No fue posible asignar la memoria.");
      fclose(fs);
      exit(1);
    }

    nodo_lista *nodo_inicial = crear_nodos(nhilos);
    nodo_lista *nodo_recorr = nodo_inicial;

    unsigned short _ = 0;
    for (unsigned short i = 0; i < nhilos; i++) {
      arg_c[i].tbuffer = tbuffer;
      arg_c[i].nodo = &nodo_recorr;
      arg_c[i].i_c = &_;
      arg_c[i].id_hilo = i;
    }

    pthread_create(&id_productor, NULL, productor, (void *)&arg_p);
    for (int i = 0; i < nhilos; i++) {
      pthread_create(&ids_consumidor[i], NULL, consumidor, (void *)&arg_c[i]);
    }

    pthread_join(id_productor, NULL);
    for (int i = 0; i < nhilos; i++) {
      pthread_join(ids_consumidor[i], NULL);
    }

    nodo_lista *nodo_recorrer2 = nodo_inicial;
    while (nodo_recorrer2 != NULL) {
      printf("%d: %ld\n", nodo_recorrer2->id, nodo_recorrer2->suma_parcial_truncada);
      nodo_recorrer2 = nodo_recorrer2->siguiente;
    }

    sem_destroy(&hay_dato);
    sem_destroy(&hay_espacio);
    sem_destroy(&mutex_c);
    sem_destroy(&mutex_l);

    free(ids_consumidor);
    free(buffer);
    fclose(fs);
  }

  printf("main: FINALIZADO.\n");
  exit(0);
}