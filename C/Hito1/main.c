#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  FILE *fe;
  FILE *fs;

  if (argc != 3) {
    fprintf(stderr, "Error en el numero argumentos.\n");
    exit(EXIT_FAILURE);
  }

  fe = fopen(argv[1], "r");

  if (fe == NULL) {
    fprintf(stderr, "El primer fichero debe existir.\n");
    exit(1);
  }

  fs = fopen(argv[2], "r");
  if (fs != NULL) {
    fprintf(stderr, "El segundo fichero no debe existir.\n");
    fclose(fs);
    fclose(fe);
    exit(1);
  }

  int pid = fork();

  if (pid == -1) {
    perror("Error al hacer fork");
    return EXIT_FAILURE;
    
  } else if (pid == 0) {
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
  }
  printf("main: Procesado de fichero terminado.\n");
  exit(0);
}