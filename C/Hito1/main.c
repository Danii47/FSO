#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * Funcion principal del programa
 *
 * @param argc numero de argumentos en la llamada al programa
 * @param argv matriz de caracteres con los valores de cada argumento
 *
 * Los argumentos en el Hito 1 (3) son:
 * 0: ejecucion del programa (./main.out)
 * 1: nombre del fichero de entrada (ejemplo100.txt)
 * 2: nombre del fichero de salida (salida.txt)
 *
 * @return 0 en caso de que no haya ningun error
 */
int main(int argc, char *argv[]) {

  FILE *fichero_entrada_procesa;
  FILE *fichero_salida_procesa;

  if (argc != 3) {
    fprintf(stderr, "Error en el numero argumentos.\n");
    exit(EXIT_FAILURE);
  }

  fichero_entrada_procesa = fopen(argv[1], "r");

  if (fichero_entrada_procesa == NULL) {
    fprintf(stderr, "El primer fichero debe existir.\n");
    exit(1);
  }

  fichero_salida_procesa = fopen(argv[2], "r");
  if (fichero_salida_procesa != NULL) {
    fprintf(stderr, "El segundo fichero no debe existir.\n");
    fclose(fichero_salida_procesa);
    fclose(fichero_entrada_procesa);
    exit(1);
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
      return EXIT_FAILURE;
    }

    if (!WIFEXITED(estado)) {
      fprintf(stderr, "El proceso hijo terminó de forma anormal.\n");
      return EXIT_FAILURE;
    }
  }
  printf("main: Procesado de fichero terminado.\n");
  return 0;
}