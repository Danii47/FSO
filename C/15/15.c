/*
Ejemplo con cadena de caracteres.
*/

#include <stdio.h>
#include <stdlib.h>

#define TAM_MAX 200

int stringSize(char *v);

int main(void) {
  char v[] = "Hola mundo";
  char w[TAM_MAX];
  int strSize;

  strSize = stringSize(v);

  printf("El string \"%s\" tiene %d caracteres.\n", v, strSize);
  printf("Escribe algo: ");

  fgets(w, TAM_MAX, stdin);
  strSize = stringSize(w);

  printf("El string \"%s\" tiene %d caracteres.\n", w, strSize);
  return 0;
}

// Escribir int stringSize (char[]) seria equivalente
int stringSize(char *v) {
  int size = 0;
  for (int i = 0; v[i] != 0; i++) { // Recorre el String hasta llegar al caracter nulo (0)
    size ++;
  }
  return size;
}

/*
¿Por qué antes de la declaración de la función main se ha escrito int stringSize(char* v);? Para que el compilador reconozca que está declarada la función
¿Por qué en la función que determina el tamaño de un string se cuenta hasta la aparición del caracter 0 (que no ’0’)? Por que C añade el caracter nulo 0 al final de la cadena
El string w se ha declarado con un tamaño de 200, sin embargo la función que determina el
tamaño de dicho string no dice siempre que ese tamaño sea 200 ¿Por qué?. Por que depende de los caracteres que meta el usuario
Documéntese sobre la función atoi ¿para que sirve? La función atoi sirve para convertir de ASCII to INTEGER, convierte un String como "132" a entero 132
*/