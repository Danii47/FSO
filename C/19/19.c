/*
Punteros no inicializados
*/

#include <stdio.h>
// ejemplo de uso de un puntero no inicializado
int main(void) {
  int *a;
  int b = 2;
  int c;
  c = b + *a; // Esta expresion genera un error en tiempo de ejecucion
  return 0;
}
/*
¿Por qué se genera un error en tiempo de ejecución? ¿Cómo lo solucionaría?
Se genera ya que el puntero a no está incializado con un valor válido.
La solución sería asignarlo a algo o inicializarlo en NULL y realizar la operacion SOLO si a NO es NULL
*/