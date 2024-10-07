/*
Diseñe una estructura de datos que contenta los valores de un array de double, el número de
elementos del array y la suma de sus elementos. Asigne un nombre a la nueva estructura de
datos con typedef y modifique la función del ejercicio 9 para que reciba un único parámetro
y calcule la suma. La nueva función no deberá retornar nada mediante return.
*/

#include <stdio.h>

typedef struct {
    double* values;
    int length;
    double sum;
} MyArray;

double arrSum(MyArray *arr) {
  double sum = 0;

  for (int i = 0; i < arr->length; i++) {
    sum += arr->values[i];
  }

  arr->sum = sum;
}

int main() {

  MyArray newMyArray;

  newMyArray.length = 3;

  double values[] = {1.0, 2.5, 3.0};
  newMyArray.values = values;
  arrSum(&newMyArray);

  printf("La suma de los elementos del array es %g\n", newMyArray.sum);

  return 0;
}
