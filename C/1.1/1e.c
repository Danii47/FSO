/*
e) Cálculo de la velocidad de un móvil dado el espacio que ha recorrido y el tiempo que
ha tardado. Considere que el movimiento es rectilíneo uniforme.
*/

#include <stdio.h>

int main() {
  float space, time;

  printf("Este programa calcula la velocidad de un movil a partir del espacio y tiempo que ha tardado.\n");
  printf("Introduce el espacio (m) y el tiempo que ha tardado (s): ");

  scanf("%f %f", &space, &time);

  float velocity = space / time;

  printf("El móvil iba a una velocidad de %f m/s\n", velocity);

  return 0;
}