/**
 * @author Hugo Adan de la Fuente y Daniel Fernandez Varona
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * Funcion que comprueba si una cadena esta compuesta unicamente por numeros
 *
 * @param cadena puntero al inicio de la cadena donde esta el string
 *
 * @return 1 si la cadena esta unicamente compuesta por numeros
 */
unsigned char es_numerico(char *cadena) {

  if (cadena[0] == '\n')
    return 0;

  int i = 0;

  while (cadena[i] != '\n' && cadena[i] != '\0') {
    if (cadena[i] > '9' || cadena[i] < '0') {
      return 0;
    }

    i++;
  }

  return 1;
}

/**
 * Funcion principal del programa
 *
 * @param argc numero de argumentos en la llamada al programa
 * @param argv matriz de caracteres con los valores de cada argumento
 *
 * Los argumentos de procesa (3) son:
 * 0: ejecucion del programa (./procesa.out)
 * 1: nombre del fichero de entrada (ejemplo100.txt)
 * 2: nombre del fichero de salida (salida.txt)
 *
 * @return 0 en caso de que no haya ningun error
 */
int main(int argc, char *argv[]) {
  FILE *fichero_entrada_procesa;
  FILE *fichero_salida_procesa;

  if (argc != 3) {
    fprintf(stderr, "Error en el numero de argumentos.\n");
    exit(1);
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

  fichero_salida_procesa = fopen(argv[2], "w");

  char cadena_leida;
  char *puntero_cadena_leida = &cadena_leida;
  size_t tamano;

  while (getline(&puntero_cadena_leida, &tamano, fichero_entrada_procesa) != -1) {
    if (es_numerico(puntero_cadena_leida)) {
      fprintf(fichero_salida_procesa, "%s", puntero_cadena_leida);
    }
  }

  printf("procesa: Procesado el fichero %s. Resultado escrito en %s.\n", argv[1], argv[2]);

  free(puntero_cadena_leida);
  fclose(fichero_entrada_procesa);
  fclose(fichero_salida_procesa);
  exit(0);
}