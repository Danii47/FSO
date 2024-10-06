/*
Uso de la función printf con reales:
*/

#include <stdio.h>
// uso del printf con reales
int main() {
  float a = 5.1, b = 4.9, c = 0.0001;
  printf("La suma de %f + %f es %f\n", a, b, a + b);

  printf("El valor de la variable a con 3 decimales es %.3f\n", a);
  printf("La opcion g tambien es util %g\n", a);

  return 0;
}
