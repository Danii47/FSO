/*
f) Cálculo del número de semanas completas que han transcurrido dado un número de días.
*/

#include <stdio.h>

int main() {
  int days;

  printf("Este programa calcula el número de semanas completas que han pasado dado un número de días.\n");
  printf("Introduce el número de días: ");
  
  scanf("%i", &days);

  int weeks = days / 7;

  printf("El número de semanas que han pasado en %i días son %i semanas.\n", days, weeks);

  return 0;
}