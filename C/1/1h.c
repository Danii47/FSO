/*
Cálculo del área de un polígono regular dado el número de lados, la longitud del lado
y la apotema.
*/

#include <stdio.h>

int main() {
  int sides;
  float sideLength, apothem;

  printf("Este programa calcula el área de un polígono regular dado el número de lados, la longitud del lado y la apotema.");
  printf("Escribe el número de lados, la longitud y la apotema: ");

  scanf("%i %f %f", &sides, &sideLength, &apothem);

  float perimeter = sides * sideLength;
  float area = .5 * perimeter * apothem;

  printf("El area es: %f unidades cuadradas.\n", area);

  return 0;
}