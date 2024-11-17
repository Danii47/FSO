/*
Ejemplo de declaración y uso de vectores.
*/

#include <stdio.h>
// ejemplo del uso de vectores
#define TAM_VECTOR 5 // Declaracion de una constante!
int main(void) {
  float v[TAM_VECTOR];
  for (int i = 0; i < TAM_VECTOR; i ++) {
    v[i] = i * 2; // Pone el vector como [0, 2, 4, 6, 10]
  }

  for (int i = 0; i < TAM_VECTOR; i ++) {
    printf("v[%d] = %g\n", i, v[i]); // Printea los valores del string
  }

  return 0;
}