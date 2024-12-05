#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <math.h>

typedef struct
{
  FILE *fichero_abierto;
  int tbuffer;
} arg_hilo_productor;

typedef struct
{
  int tbuffer;
  int *resultado;
} arg_hilo_consumidor;

typedef struct
{
  int longitud;
  char cadena[33];
} celda_buffer_t;

celda_buffer_t *buffer;

sem_t hay_dato;
sem_t hay_espacio;
sem_t mutex_c;

bool es_binario(char *cadena, int tam_cadena)
{
  for (int i = 0; i < tam_cadena; i++)
  {
    if (cadena[i] != '0' && cadena[i] != '1')
    {
      return false;
    }
  }

  return true;
}

int atobtoi(char *cadena)
{
  int n = 0;

  for (int i = strlen(cadena); i >= 0; i--)
  {
    if (cadena[i] == '1')
    {
      n += pow(2, strlen(cadena) - i);
    }
  }

  return n;
}

void *productor(void *arg)
{
  arg_hilo_productor *arg_p = (arg_hilo_productor *)arg;
  char *cadena = NULL;
  int i_p = 0;
  size_t tam_buffer_cadena;
  int tam_cadena;
  celda_buffer_t dato;
  while (getline(&cadena, &tam_buffer_cadena, arg_p->fichero_abierto) != -1)
  {
    

    tam_cadena = cadena[strlen(cadena) - 1] == '\n' ? strlen(cadena) - 1 : strlen(cadena);

    if (tam_cadena <= 32 && tam_cadena >= 1 && es_binario(cadena, tam_cadena))
    {
      sem_wait(&hay_espacio);
      printf("Me reproduzco con la cadena %s\n", cadena);
      strcpy(dato.cadena, cadena);
      dato.longitud = tam_cadena;
      buffer[i_p] = dato;
      i_p = (i_p + 1) % arg_p->tbuffer;

      sem_post(&hay_dato);
    }
  }

  dato.longitud = -1;
  buffer[i_p] = dato;
  sem_post(&hay_dato);
  pthread_exit(NULL);
}

void *consumidor(void *arg)
{
  arg_hilo_consumidor *arg_c = (arg_hilo_consumidor *)arg;
  celda_buffer_t dato;
  int suma = 0;
  int i_c = 0;
  bool parada = false;
  while (!parada)
  {
    sem_wait(&hay_dato);
    sem_wait(&mutex_c);
    dato = buffer[i_c];
    
    if (dato.longitud == -1)
    {
      sem_post(&hay_dato);
      printf("[CON SALIDA] NO consumo dato %s | l: %d\n", dato.cadena, dato.longitud);
      parada = true;
    }
    else
    {
      if (dato.cadena[0] != '1' && dato.longitud == 32)
      {
        printf("Consumo dato %s\n", dato.cadena);
        suma = (suma + atobtoi(dato.cadena)) % (RAND_MAX / 2);
      } else {
        printf("NO consumo dato %s | l: %d\n", dato.cadena, dato.longitud);
      }
      i_c = (i_c + 1) % arg_c->tbuffer;
      sem_post(&hay_espacio);
    }
    sem_post(&mutex_c);
  }
  printf("Se para hilo consumidor\n");
  *(arg_c->resultado) = suma;
  pthread_exit(NULL);
}

bool es_numero(char *cadenaLeida)
{
  for (int i = 0; i < strlen(cadenaLeida); i++)
  {

    if (cadenaLeida[i] > '9' || cadenaLeida[i] < '0')
    {
      return false;
    }
  }

  return true;
}

int main(int argc, char *argv[])
{

  FILE *fe;
  FILE *fs;

  if (argc != 5)
  {
    fprintf(stderr, "Error en argumentos\n");
    exit(1);
  }

  fe = fopen(argv[1], "r");

  if (fe == NULL)
  {
    fprintf(stderr, "El primer fichero debe existir\n");
    exit(1);
  }

  fs = fopen(argv[2], "r");
  if (fs != NULL)
  {
    fprintf(stderr, "El segundo fichero no debe existir\n");
    fclose(fs);
    fclose(fe);
    exit(1);
  }

  char *hilos = argv[3];
  if (!es_numero(hilos))
  {
    fprintf(stderr, "El parametro de hilos no es numerico.\n");
    fclose(fs);
    fclose(fe);
    exit(1);
  }

  int nhilos = atoi(hilos);

  if (nhilos < 2 || nhilos > 1000)
  {
    fprintf(stderr, "El parametro de hilos debe ser un numero en el intervalo [2, 1000].\n");
    fclose(fs);
    fclose(fe);
    exit(1);
  }

  char *tbuffer_str = argv[4];
  if (!es_numero(tbuffer_str))
  {
    fprintf(stderr, "El parametro de hilos no es numerico.\n");
    fclose(fs);
    fclose(fe);
    exit(1);
  }

  int tbuffer = atoi(tbuffer_str);

  if (tbuffer < 10 || tbuffer > 1000)
  {
    fprintf(stderr, "El parametro de tamaño de buffer debe ser un numero en el intervalo [10, 1000].\n");
    fclose(fs);
    fclose(fe);
    exit(1);
  }

  int pid = fork();

  if (pid == 0)
  {
    char *path = "/home/J5/12474427D/FSO/C/Hito2/procesa";
    char *comando = "./procesa";
    char *arg1 = argv[1];
    char *arg2 = argv[2];
    int estado = execl(path, comando, arg1, arg2, NULL);
    if (estado == -1)
    {
      fprintf(stderr, "Algo fue mal en el procesado.\n");
      exit(1);
    }
  }
  else
  {
    wait(NULL);

    fs = fopen(argv[2], "r");

    if (fs == NULL)
    {
      fprintf(stderr, "El primer fichero debe existir\n");
      exit(1);
    }

    buffer = (celda_buffer_t *)malloc(sizeof(celda_buffer_t) * tbuffer);
    if (buffer == NULL)
    {
      fprintf(stderr, "No fue posible asignar la memoria.");
      fclose(fs);
      exit(1);
    }

    sem_init(&hay_dato, 0, 0);
    sem_init(&hay_espacio, 0, tbuffer);
    sem_init(&mutex_c, 0, 1);

    arg_hilo_productor arg_p;
    arg_p.fichero_abierto = fs;
    arg_p.tbuffer = tbuffer;

    pthread_t id_productor;

    int *array_resultados = (int *)malloc(sizeof(int) * nhilos);
    if (array_resultados == NULL)
    {
      fprintf(stderr, "No fue posible asignar la memoria.");
      fclose(fs);
      exit(1);
    }

    arg_hilo_consumidor *arg_c = (arg_hilo_consumidor *)malloc(sizeof(arg_hilo_consumidor) * nhilos);

    pthread_t *ids_consumidor = (pthread_t *)malloc(sizeof(pthread_t) * nhilos);

    if (ids_consumidor == NULL)
    {
      fprintf(stderr, "No fue posible asignar la memoria.");
      fclose(fs);
      exit(1);
    }

    for (int i = 0; i < nhilos; i++)
    {
      arg_c[i].tbuffer = tbuffer;
      arg_c[i].resultado = &array_resultados[i];
    }

    pthread_create(&id_productor, NULL, productor, (void *)&arg_p);
    for (int i = 0; i < nhilos; i++)
    {
      pthread_create(&ids_consumidor[i], NULL, consumidor, (void *)&arg_c[i]);
    }

    pthread_join(id_productor, NULL);
    for (int i = 0; i < nhilos; i++)
    {
      pthread_join(ids_consumidor[i], NULL);
    }

    for (int i = 0; i < nhilos; i++)
    {
      printf("%d: %d\n", i, array_resultados[i]);
    }

    sem_destroy(&hay_dato);
    sem_destroy(&hay_espacio);
    sem_destroy(&mutex_c);

    free(ids_consumidor);
    free(buffer);
    fclose(fs);
  }

  printf("main: Procesado de fichero terminado.\n");
  // TODO: preguntar por que da error si cierro los ficheros tambien en el proceso padre.
  exit(0);
}