/*
c) Cálculo de la media de tres números.
*/

#include <stdio.h>

int main() {
  double numbers[3] = {1, 2, 3};

  printf("Este programa le permite calcular la media de 3 números.\nIntroduzca el valor de los 3 números: ");
  scanf("%lf %lf %lf", numbers, &numbers[1], numbers + 2);

  double media = (numbers[0] + numbers[1] + numbers[2]) / 3;

  printf("El valor de la media de los 3 números es: %lf\n", media);

  return 0;
}