/*
Repita el ejercicio 7 pero las funciones recibirán dos parámetros: el valor numérico x y un
parámetro por referencia que tomará el valor de f(x) si f está definida para el x dado, o -1,
en caso contrario. Además la función devolverá el valor lógico true si la función está
definida en el x dado o false, en caso contrario.
Durante el recorrido de los valores del intervalo, el programa principal detectará si la función
está definida en cada punto. Si lo está se mostrará el valor de la función, y si no, el mensaje:
no definida.

Un ejemplo de salida sería:

Introduzca un intervalo valido:
  Valor de a0: -0.2
  Valor de b0: 0.2
Introduzca el valor de delta [1 e -5 ,1]: 0.1
Evaluando f1(x) en el intervalo [ -0.2 , 0.2] con delta =0.1
f1(-0.2): 0.8
f1(-0.1): 0.9
f1(0.0): no definida
f1(0.1): -0.1
f1(0.2): -0.2

*/

#include <stdio.h>
#include<stdbool.h>
#include <math.h>
#define M_PI 3.14159265358979323846

float f1(float x, float* resultPointer) {
  if (x < 0) {
    *resultPointer = x + 1;
    return true;
  } else if (x > 0) {
    *resultPointer = -x;
    return true;
  }
  
  return false;
}

float f2(float x, float* resultPointer) {
  if (x >= 0 && x < 10) {
    *resultPointer = (2 * x) + 4;
    return true;
  } else if (x >= 10 && x < 20) {
    *resultPointer = (3 * (x * x)) + (5 * x) - 1;
    return true;
  }

  *resultPointer = -1;
  return false;
}

float f3(float x, float* resultPointer) {
  if (x == M_PI) {
    *resultPointer = -1;
    return false;
  }

  if (x < M_PI / 2) {
    *resultPointer = cos(x);
  } else if (x > M_PI / 2) {
    *resultPointer = -sin(x);
  }

  return true;
}

float f4(float x, float* resultPointer) {
  if (x <= 0) {
    *resultPointer = x * x * x;
    return true;
  }

  *resultPointer = sqrt(x);
  return true;
}

float f5(float x, float y, float* resultPointer) {
  *resultPointer = (x * x) + (y * y); 
  return true;
}

int main() {

  // Definir un tipo de puntero a función
  typedef float (*FunctionPointer)(float, float*);
    
  FunctionPointer functions[] = {f1, f2, f3, f4}; // Array de punteros a funciones
  int function;
  float sup, inf, delta;

  printf("¿Qué función quieres evaluar? ");
  scanf("%i", &function);

  if (function < 1 || function > 5) {
    printf("Función inválida.");
    return 1; // Devuelvo 1 para marcar el error
  }

  if (function == 5) {
    float x, y, result;

    printf("Valor X: ");
    scanf("%f", &x);

    printf("Valor Y: ");
    scanf("%f", &y);

    f5(x, y, &result);

    printf("f5(%g, %g) = %g\n", x, y, result);

    return 0;
  }

  printf("Introduzca un intervalo válido:\n");
  
  printf("\tValor de a0: ");
  scanf("%f", &inf);

  printf("\tValor de b0: ");
  scanf("%f", &sup);

  printf("Introduzca el valor de delta [1e-5, 1]: ");
  scanf("%f", &delta);

  if (delta < .00005 || delta > 1) {
    printf("Valor de delta inválido.\n");
    return 1; // Devuelvo 1 para marcar el error
  }

  int functionIndex = function - 1;

  printf("Evaluando f%i(x) en el intervalo [%g, %g] con delta = %g\n", functionIndex, inf, sup, delta);

  float result;
  for (float i = inf; i <= sup; i += delta)
  {
    bool okay = functions[functionIndex](i, &result);
    if (okay) {
      printf("f%i(%g) = %g\n", functionIndex, i, result);
    } else {
      printf("f%i(%g) = no definida\n", functionIndex, i);
    }
  }

  return 0;
}