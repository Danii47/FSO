/*
Escriba un programa en C que reciba por línea de comandos el número de filas de una matriz,
el número columnas y el nombre de un fichero. Valide que los números estén comprendidos
en el intervalo [1,10] y a continuación pida el valor de cada elemento de la matriz al usuario
por teclado (números reales). Una vez leídos todos los valores, la matriz se guardará en el
fichero cuyo nombre se pasó por línea de comandos, separando las columnas con tabuladores
y las filas por retornos de carro. Si hay algún error al abrir el fichero, se mostrará un error
al usuario. Todas las salidas de errores han de escribirse en la salida estándar de error.
Valide que el número de filas no puedan ser valores no numéricos. Por ejemplo la entrada:
./a.out a2 3b fichero .txt
deberá mostrar un error.
Use una matriz cuya memoria debe alojarse de manera dinámica.
Un ejemplo de uso sería:
./a.out 2 3 fichero.txt
v[0 ,0]: 2.4
v[0 ,1]: 8.4
v[0 ,2]: 17.9
v[1 ,0]: -5.2
v[1 ,1]: 4.97
v[1 ,2]: -2.1
Matriz guardada en fichero.txt!
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool isNumber(char* str) {
  for (int i = 0; str[i] != 0; i++) {
    if (str[i] < '0' || str[i] > '9') {
      return false;
    }
  }

  return true;
}

int main(int argc, char** argv) {
  if (argc != 4) {
    fprintf(stderr, "ERROR: El número de argumentos es incorrecto.");
    exit(-1);
  }

  char* rows = argv[1];
  char* columns = argv[2];
  if (!isNumber(rows) || !isNumber(columns)) {
    fprintf(stderr, "ERROR: El número de filas y/o columnas no es correcto.");
    exit(-1);
  }

  int rowsNumber = atoi(rows);
  int columnsNumber = atoi(columns);

  if (rowsNumber > 10 || rowsNumber < 1 || columnsNumber > 10 || columnsNumber < 1) {
    fprintf(stderr, "ERROR:");
    exit(-1);
  }

  float* matrix = (float*) malloc(sizeof(float) * (rowsNumber * columnsNumber));

  if (matrix != NULL) {
    char* fileName = argv[3];
    FILE* file = fopen(fileName, "wt");

    for (int i = 0; i < rowsNumber; i++) {
      for (int j = 0; j < columnsNumber; j++) {
        printf("Escribe el valor [%i][%i] de la matriz.\n", i, j);
        scanf("%f", &matrix[i * columnsNumber + j]);
        fprintf(file, "%f ", matrix[i * columnsNumber + j]);
      }
      fprintf(file, "\n");
    }

    fclose(file);

    printf("Matriz guardada en %s!\n", fileName);

    return 0;
  }
}
