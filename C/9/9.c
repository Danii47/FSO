/*
Cree una función que permita calcular la suma de los elementos de un array de tipo double,
teniendo en cuenta que ha de pasar el tamaño del array. El valor de la suma ha de retornarse
usando return.
*/

#include <stdio.h>

int arrSum(double arr[], int length) {
  int sum = 0;

  for (int i = 0; i < length; i++) {
    sum += arr[i];
  }

  return sum;
}

int main() {
  int length = 5;
  double array[] = {1, 2, 3, 4, 5};

  int sum = arrSum(array, length);
  printf("La suma de los elementos del array es %i\n", sum);

  return 0;
}