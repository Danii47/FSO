/*
Uso de la función scanf:
*/

#include <stdio.h>
// ejemplo del uso del scanf
int main(void) {
  int a, b;
  float c;

  printf("Introduzca un numero entero: ");
  scanf("%d", &a);
  printf("Introdujo el numero %d\n", a);
  // Podemos leer varios numeros
  printf("Introduce dos enteros separados por un espacio: ");
  scanf("%d %d", &a , &b);
  printf("Ha introducido los numeros %d y %d\n", a, b);
  // Podemos leer un numero entero y un real
  printf("Introduce un entero y un real separados por un espacio: ");
  scanf("%d %f", &a, &c);
  printf("Ha introducido los numeros %d y %f\n", a, c);
  /* Para leer strings existe una funcion llamada fgets . scanf puede leer
  strings pero no si contienen espacios .*/
  return 0;
}