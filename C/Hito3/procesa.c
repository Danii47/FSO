#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool es_numerico(char *cadenaLeida) {

  if (cadenaLeida[0] == '\n')
    return false;

  int i = 0;

  while (cadenaLeida[i] != '\n' && cadenaLeida[i] != '\0') {

    if (cadenaLeida[i] > '9' || cadenaLeida[i] < '0') {
      return false;
    }

    i++;
  }

  return true;
}

int main(int argc, char *argv[]) {

  FILE *fe;
  FILE *fs;

  if (argc != 3) {
    fprintf(stderr, "Error en el numero de argumentos.\n");
    exit(1);
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

  fs = fopen(argv[2], "w");

  char cadenaLeida;
  char *punteroCadenaLeida = &cadenaLeida;
  size_t tam;

  while (getline(&punteroCadenaLeida, &tam, fe) != -1) {
    if (es_numerico(punteroCadenaLeida)) {
      fprintf(fs, "%s", punteroCadenaLeida);
    }
  }

  printf("procesa: Procesado el fichero %s. Resultado escrito en %s.\n", argv[1], argv[2]);

  free(punteroCadenaLeida);
  fclose(fe);
  fclose(fs);
  exit(0);
}