/*
Un año es bisiesto cuando es divisible entre 4 (el año 2012 fue bisiesto), sin embargo, los
años divisibles entre 100 no lo son (el año 1900 no fue bisiesto), pero sí que lo son los años
divisibles entre 400 (el año 2 000 sí fue bisiesto). Escriba un programa que dado un año a,
proporcionado por el usuario, informe si es bisiesto o no. Realice este ejercicio del tal forma
que la expresión lógica de un único condicional, compruebe si el año es bisiesto. Repita el
ejercicio de tal forma que la expresión lógica de un único condicional compruebe que el año
no es bisiesto. Valide que el año introducido sea mayor que 0 y menor que 5 000.
*/

#include <stdio.h>

int main() {
  int year;

  printf("Este programa comprueba si un año es bisiesto o no.\n");
  printf("Escribe el año (0 - 5000): ");

  scanf("%i", &year);

  if (year < 0 || year > 5000) {
    printf("El año introducido no es válido.\n");
    return 1; // Devuelvo 1 para indicar que el programa finaliza con algún error
  }

  if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
    printf("El año introducido SÍ es bisiesto.\n");
  } else if ((year % 4 != 0 || year % 100 == 0) && (year % 400 != 0)) {
    printf("El año introducido NO es bisiesto.\n");
  }

  return 0;
}