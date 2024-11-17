/*
Ejemplo del operador &.
*/

#include <stdio.h>
// Ejemplo del operador &
int main(void) {
  int b = 5;
  printf("La variable b coniene el valor %d y esta en la direccion de memoria %p\n", b, &b);
  // %p permite imprimir una direccion de memoria (puntero)
  return 0;
}