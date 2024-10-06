/*
Punteros: el operador *
*/

#include <stdio.h>
// ejemplo de uso de punteros
int main(void) {
  int a = 55;
  int *b;
  b = &a;
  printf("El valor de a es %d\n", a);
  *b = *b + 10;
  printf("Ahora el valor de a es %d\n", a);
  return 0;
}
/*
¿Por qué cambia el valor de la variable a? Por que se está cambiando el valor de la variable a la que apunta b, en este caso a.
*/