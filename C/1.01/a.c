/*
Problema 9. Realizar un procedimiento en C que me genere en una matriz las
potencias menores que n de los números comprendidos entre a y b. El array/matriz
potencias debe crearse dinámicamente.
void genera_potencias(int a, int b, int n, int*** potencias);
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void genera_potencias(int a, int b, int n, int ***potencias) {

  int longitud = b - a + 1;
  *potencias = (int **)malloc(sizeof(int *) * longitud);

  for (int i = 0; i < longitud; i++) {
    (*potencias)[i] = (int *)malloc(sizeof(int) * (n + 1));
  }

  for (int i = 0; i < longitud; i++) {
    for (int j = 0; j <= n; j++) {
      (*potencias)[i][j] = pow(a + i, j);
    }
  }
}

int main() {
  int a = 2;
  int b = 4;
  int n = 4;
  int **potencias;

  genera_potencias(a, b, n, &potencias);

  for (int i = 0; i < b - a + 1; i++) {
    for (int j = 0; j < n; j++) {
      printf("%d ", potencias[i][j]);
    }
    printf("\n");
  }

  return 0;
}
