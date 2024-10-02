/*
Desarrolle un algoritmo que calcule la raíz cuadrada r entera más próxima a un número
dado n. n será proporcionado por el usuario y estará dentro del intervalo [0,4000]. Además
r^2 ≤ n.
*/

#include <stdio.h>
#define SQUARE(x) ((x) * (x))

int main() {
  int number;

  printf("Este programa calcula la raiz cuadrada entera más próxima a un número.\n");
  printf("Escribe el número: ");

  scanf("%i", &number);

  int closeRoot = 1;

  while (SQUARE(closeRoot + 1) <= number) {
    closeRoot++;
  }

  printf("La raiz más cercana sin pasarse es: %i\n", closeRoot);

  return 0;
}