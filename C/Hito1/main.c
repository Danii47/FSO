#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{

  FILE *fe;
  FILE *fs;

  if (argc != 3)
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

  int pid = fork();

  if (pid == 0)
  {
    char *path = "/home/J5/12474427D/FSO/C/Hito1/procesa";
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
  else {
    wait(NULL);
  }
  printf("main: Procesado de fichero terminado.\n");
  // TODO: preguntar por que da error si cierro los ficheros tambien en el proceso padre.
  exit(0);
}