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

float *multMatrix(float *matrixA, float *matrixB, int matrixARows, int matrixABColumnsRows, int matrixBColumns) {
  float *matrixResult = (float *)malloc(sizeof(float) * (matrixARows * matrixBColumns));

  if (matrixResult == NULL) {
    fprintf(stderr, "ERROR: No fue posible asignar la memoria.\n");
    exit(-1);
  }

  float sum = 0;

  for (int i = 0; i < matrixARows; i++) {
    for (int j = 0; j < matrixBColumns; j++) {
      for (int k = 0; k < matrixABColumnsRows; k++) {
        sum += matrixA[i * matrixABColumnsRows + k] * matrixB[k * matrixBColumns + j];
      }
      matrixResult[i * matrixBColumns + j] = sum;
      sum = 0;
    }
  }

  return matrixResult;
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
  int matrixBColumns = atoi(matrixBcolumnsString);

  if (matrixARows > 10 || matrixARows < 1 || matrixABColumnsRows > 10 || matrixABColumnsRows < 1 || matrixBColumns > 10 || matrixBColumns < 1) {
    fprintf(stderr, "ERROR: El número de filas / columnas es mayor que 10 o menor que 1.\n");
    exit(-1);
  }

  float *matrixA = (float *) malloc(sizeof(float) * (matrixARows * matrixABColumnsRows));
  float *matrixB = (float *) malloc(sizeof(float) * (matrixABColumnsRows * matrixBColumns));

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
      scanf("%.2f", &matrixA[i * matrixABColumnsRows + j]);
      fprintf(file, "%.2f", matrixA[i * matrixABColumnsRows + j]);
      if (j != matrixABColumnsRows - 1)
        fprintf(file, "\t");
    }
    fprintf(file, ")\n");
  }

  fprintf(file, "\n");

  for (int i = 0; i < matrixABColumnsRows; i++) {
    fprintf(file, "(");
    for (int j = 0; j < matrixBColumns; j++) {
      printf("Escribe el valor [%i][%i] de la matriz B.\n", i, j);
      scanf("%.2f", &matrixB[i * matrixBColumns + j]);
      fprintf(file, "%.2f", matrixB[i * matrixBColumns + j]);
      if (j != matrixBColumns - 1)
        fprintf(file, "\t");
    }
    fprintf(file, ")\n");
  }

  fprintf(file, "\n");

  float *matrixResult = multMatrix(matrixA, matrixB, matrixARows, matrixABColumnsRows, matrixBColumns);

  for (int i = 0; i < matrixARows; i++) {
    fprintf(file, "(");
    printf("(");
    for (int j = 0; j < matrixBColumns; j++) {
      fprintf(file, "%.2f", matrixResult[i * matrixBColumns + j]);
      printf("%.2f", matrixResult[i * matrixBColumns + j]);
      if (j != matrixBColumns - 1) {
        fprintf(file, "\t");
        printf("\t");
      }
    }
    fprintf(file, ")\n");
    printf(")\n");
  }

  fclose(file);

  free(matrixA);
  free(matrixB);
  free(matrixResult);

  printf("Matriz guardada en %s!\n", fileName);

  return 0;
}
