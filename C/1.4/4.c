/*
Desarrolle un algoritmo que dado un número entero positivo, n, muestre por pantalla los n
primeros términos de la sucesión de Fibonacci: 0, 1, 1, 2, 3, 5, 8, . . .. n debe ser menor o igual
a 50.
*/

#include <stdio.h>

int main() {
  int number;

  printf("Este programa muestra los n primeros números de la sucesión de fibonacci.\n");
  printf("Introduce n (0 - 50): ");

  scanf("%i", &number);

  if (number <= 0 || number > 50) {
    printf("El número introducido no es válido.\n");
    return 1; // Devuelvo 1 para indicar que ocurrió un error
  }

  printf("0\n");

  if (number >= 2) {
    long n1 = 0, n2 = 1, aux;
    printf("1\n");
    for (int i = 2; i < number; i++) {
      aux = n2;
      n2 += n1;
      n1 = aux;
      printf("%ld\n", n2);
    }
  }

  return 0;
}
