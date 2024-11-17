/*
2. Repita el ejercicio 1 pero con 4 parámetros de entrada:
  a) Número de filas de la matriz A.
  b) Número de columnas de la matriz A y número de filas de la matriz B.
  c) Número de columnas de la matriz B.
  d) Nombre del fichero de salida.

Valide la entrada, del mismo modo que el ejercicio 1, y pida los valores de ambas matices
por teclado. A continuacián, multiplique las matrices, muestre el resultado por pantalla y
guárdelo en el fichero de salida.

Todas las matrices deben ser memoria alojada de manera dinámica y la multiplicación de
matrices debe hacerse en una función.
Los siguientes ejercicios permiten ver ejemplos de los distintos conceptos presentados en las
sesiones de prácticas. Como trabajo autónomo, codifique, entienda y responda a las preguntas que
se formulan en alguno de ellos.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool isNumber(char *str) {
  for (int i = 0; str[i] != 0; i++) {
    if (str[i] < '0' || str[i] > '9') {
      return false;
    }
  }

  return true;
}

int main(int argc, char **argv)
{
  if (argc != 5)
  {
    fprintf(stderr, "ERROR: El número de argumentos es incorrecto.\n");
    exit(-1);
  }

  char *matrixARowsString = argv[1];
  char *matrixABRowsColumnsString = argv[2];
  char *matrixBcolumnsString = argv[3];

  if (!isNumber(matrixARowsString) || !isNumber(matrixABRowsColumnsString) || !isNumber(matrixBcolumnsString)) {
    fprintf(stderr, "ERROR: El número de filas y/o columnas no es correcto.\n");
    exit(-1);
  }

  int matrixARows = atoi(matrixARowsString);
  int matrixABColumnsRows = atoi(matrixABRowsColumnsString);
  int matrixBcolumns = atoi(matrixBcolumnsString);

  if (matrixARows > 10 || matrixARows < 1 || matrixABColumnsRows > 10 || matrixABColumnsRows < 1 || matrixBcolumns > 10 || matrixBcolumns < 1) {
    fprintf(stderr, "ERROR: El número de filas / columnas es mayor que 10 o menor que 1.\n");
    exit(-1);
  }

  float *matrixA = (float *) malloc(sizeof(float) * (matrixARows * matrixABColumnsRows));
  float *matrixB = (float *) malloc(sizeof(float) * (matrixABColumnsRows * matrixBcolumns));

  if (matrixA == NULL || matrixB == NULL) {
    fprintf(stderr, "ERROR: No fue posible asignar la memoria.\n");
    exit(-1);
  }

  char *fileName = argv[4];
  FILE *file = fopen(fileName, "w");

  if (file == NULL) {
    fprintf(stderr, "ERROR: No fue posible abrir el fichero.\n");
    exit(-1);
  }

  for (int i = 0; i < matrixARows; i++) {
    fprintf(file, "(");
    for (int j = 0; j < matrixABColumnsRows; j++) {
      printf("Escribe el valor [%i][%i] de la matriz A.\n", i, j);
      scanf("%f", &matrixA[i * matrixABColumnsRows + j]);
      fprintf(file, "%f", matrixA[i * matrixABColumnsRows + j]);
      if (j != matrixABColumnsRows - 1)
        fprintf(file, "\t");
    }
    fprintf(file, ")\n");
  }

  fprintf(file, "\n");

  for (int i = 0; i < matrixABColumnsRows; i++) {
    fprintf(file, "(");
    for (int j = 0; j < matrixBcolumns; j++) {
      printf("Escribe el valor [%i][%i] de la matriz B.\n", i, j);
      scanf("%f", &matrixB[i * matrixBcolumns + j]);
      fprintf(file, "%f", matrixB[i * matrixBcolumns + j]);
      if (j != matrixBcolumns - 1)
        fprintf(file, "\t");
    }
    fprintf(file, ")\n");
  }

  float sum = 0;

  for (int i = 0; i < matrixARows; i++) {
    for (int j = 0; j < matrixBcolumns; j++) {
      for (int k = 0; k < matrixABColumnsRows; k++) {
        sum += matrixA[i * matrixABColumnsRows + j] * matrixB[j * matrixABColumnsRows + k];
      }
      fprintf(file, "%f ", sum);
      sum = 0;
    }
  }

  fclose(file);

  free(matrixA);
  free(matrixB);

  printf("Matriz guardada en %s!\n", fileName);

  return 0;
}
