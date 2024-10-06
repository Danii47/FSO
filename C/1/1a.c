/*
Escriba un probrama en C por cada apartado siguiente: NOTA: un ejemplo de salida o
impresión de la ejecución del podría ser:
Este programa le permite calcular el area de un circulo dado su radio.
Introduzca el valor del radio: 5
El valor del area del ciruculo es: 78.525

a) Cálculo de área de un círculo.
*/

#define PI 3.1415926535897932384
#include <stdio.h>

int main() {
  double radius;

  printf("Este programa le permite calcular el area de un circulo dado su radio.\nIntroduzca el valor del radio: ");
  scanf("%lf", &radius);

  double area = PI * radius * radius;

  printf("El valor del area del ciruculo es: %lf\n", area);

  return 0;
}