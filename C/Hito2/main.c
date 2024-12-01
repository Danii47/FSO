#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <string.h>

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

  char* hilos = argv[3];
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

  char *buffer = argv[4];
  if (!es_numero(buffer))
  {
    fprintf(stderr, "El parametro de hilos no es numerico.\n");
    fclose(fs);
    fclose(fe);
    exit(1);
  }

  int tbuffer = atoi(buffer);

  if (tbuffer < 10 || tbuffer > 1000)
  {
    fprintf(stderr, "El parametro de hilos debe ser un numero en el intervalo [10, 1000].\n");
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
    if (estado = -1)
    {
      fprintf(stderr, "Algo fue mal en el procesado.\n");
      exit(1);
    }
    exit(0);
  }
  else
  {
    wait(NULL);
  }
  printf("main: Procesado de fichero terminado.\n");
  // TODO: preguntar por que da error si cierro los ficheros tambien en el proceso padre.
  exit(0);
}