/*
La letra del DNI se obtiene calculando el resto de la división del número del DNI entre el
número 23. Una vez obtenido el resto se asigna la letra en función de la siguiente tabla:
Resto 0 1 2 3 4 5 6 7 8 9 10 11
Letra T R W A G M Y F P D X  B
Resto 12 13 14 15 16 17 18 19 20 21 22
Letra N  J  Z  S  Q  V  H  L  C  K  E

Valide que el DNI introducido es mayor que 0 y menor que 99.999.999
Tenga en cuenta al realizar este programa en C que el tipo de dato que selecciones para
guardar el DNI debe permitir valores de hasta 99.999.999
La solución debe realizar el menor número posible de comparaciones, así que seleccione el
esquema condicional más adecuado.
*/

#include <stdio.h>

int main() {
  char table[23] = "TRWAGMYFPDXBNJZSQVHLCKE";
  int dni;

  printf("Este programa determina la letra del DNI\n");
  printf("Introduzca el numero del DNI: ");

  scanf("%i", &dni);

  if (dni < 0 || dni > 99999999) {
    printf("El DNI introducido no es válido.");
    return 1; // Uso 1 para indicar que el programa finalizó con algún error.
  }

  int rest = dni % 23;
  char dniLetter = table[rest];

  printf("La letra del DNI %i es %c.\n", dni, dniLetter);

  return 0;
}