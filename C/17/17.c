/*
Punteros: el operador *
*/

#include <stdio.h>
// Primer ejemplo de punteros
int main(void)
{
  int b = 55;
  int *c;
  c = NULL;
  c = &b; // Se guarda en c la direccion de memoria donde esta almacenada la variable b
  printf("La variable b contiene el valor %d y esta en la direccion de memoria %p\n", b, &b);
  // Los punteros contienen direcciones de memoria
  printf("La variable c es un puntero que guarda la direccion de memoria %p\n", c);
  // Podemos mostrar el valor apuntado por un puntero (valor contenido en la direccion de memoria que almacena)
  printf("La variable c es un puntero que apunta al valor: %d\n", *c);
  return 0;
}
/*
¿Por qué el contenido de c es la dirección de memoria de la variable b? Por que en la variable c se almacena un puntero a la variable b
¿Cuál es el puntero NULL? ¿Por qué cree que es buena idea inicializar los puteros a NULL? El puntero NULL es aquel que no apunta a nada,
es buena idea inicializarlo en NULL por que inicialmente pueden estar apuntando a contenido "basura" que puede ser un problema para el desarrollador.
*/