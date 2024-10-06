/*
Uso de la función printf con enteros:
*/

#include <stdio.h>

// Uso del printf con enteros
int main(void) {
  int a = 5, b = 6, c = 44;

  printf ("La suma de %d + %d es %d\n", a, b, a + b);
  printf ("%d en octal es %o y en hexadecimal %x\n", c, c, c);
  return 0;
}

/*
El programa hace la suma de dos números y lo muestra por pantalla.
Además, muestra el valor de c en octal y hexadecimal.
*/