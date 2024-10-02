/*
b) Cálculo de circunferencia de un círculo.
*/

#define PI 3.14159265358979323846
#include <stdio.h>

int main() {
  double radius;

  printf("Este programa le permite calcular el perimetro de un circulo dado su radio.\nIntroduzca el valor del radio: ");
  scanf("%lf", &radius);

  double perimeter = 2 * PI * radius;

  printf("El valor del perimetro del ciruculo es: %lf\n", perimeter);

  return 0;
}