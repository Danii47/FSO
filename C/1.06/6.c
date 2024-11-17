/*
Desarrolle un algoritmo que calcule la raíz cuadrada r entera más próxima a un número
dado n. n será proporcionado por el usuario y estará dentro del intervalo [0,4000]. Además
r^2 ≤ n.

Modifique el ejercicio 5 para que pueda aproximar la raíz del número con 1, 2, 3, . . . , d decimales,
siendo d un número entre 1 y 8 que proporcione el usuario. El procedimiento debe
acercarse a la solución de forma progresiva, por ejemplo: para el número 12 debe indicar:
Con 0 cifras decimales la raíz mas proxima es 3
Con 1 cifras decimales la raíz mas proxima es 3.4
Con 2 cifras decimales la raíz mas proxima es 3.46
Con 3 cifras decimales la raíz mas proxima es 3.464
*/

#include <stdio.h>
#define SQUARE(x) ((x) * (x))

int main() {
  int number, decimals;

  printf("Este programa calcula la raiz cuadrada con decimales más próxima a un número.\n");
  printf("Escribe el número y el número de decimales: ");

  scanf("%i %i", &number, &decimals);

  float closeRoot = 1;
  float division = 1;

  for (int i = 0; i <= decimals; i++) {
    while (SQUARE(closeRoot + (1.f / division)) <= number) {
      closeRoot += (1.f / division);
    }
    division *= 10;
    printf("Con %i cifras decimales la raiz mas proxima es %.*f\n", i, i, closeRoot);
  }

  return 0;
}