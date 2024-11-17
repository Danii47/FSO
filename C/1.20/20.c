/*
Paso de parámetros por referencia
*/

#include <stdio.h>

void suma10P(int *a);
void suma10(int a);

int main(void) {
  int a1 = 10;
  suma10P(&a1);
  printf("El valor de a despues de llamar a suma10P es %d\n", a1);
  a1 = 10;
  suma10(a1);
  printf("El valor de a despues de llamar a suma10 es %d\n", a1);
  return 0;
}
void suma10P(int *a) {
  *a = *a + 10;
}
void suma10(int a) {
  a = a + 10;
}
/*
¿Cuál de las dos funciones modifica el valor del parámetro? ¿Por qué?
La función que modifica el valor es suma10P, ya que recibe una referencia por 
parametro, y dentro de esta le suma 10 a la variable que apunta está apuntando esa referencia.
*/