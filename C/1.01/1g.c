/*
g) Cálculo del perímetro de un polígono regular dado el número de lados y la longitud del
lado.
*/

#include <stdio.h>

int main() {
  int sides;
  float sideLength;

  printf("Este programa calcula el perímetro de un polígono regular dado el número de lados y la longitud del lado.\n");
  printf("Introduce el número de lados y la longitud del lado: ");

  scanf("%i %f", &sides, &sideLength);

  float perimeter = sides * sideLength;

  printf("El perimetro es: %f unidades.\n", perimeter);

  return 0;
}