/*
Cree una función para cada una de las siguientes funciones:
a) f1(x) = {
  x + 1, x < 0
  −x, x > 0
}

b) f2(x) = {
  2x + 4, x ∈ [0, 10)
  3x^2 + 5x − 1, x ∈ [10, 20)
}

c) f3(x) = {
  cos(x), x < −π/2
  −sin(x), x > π/2
}

d) f4(x) = {
  x^3, x ≤ 0
  sqrt(x), x > 0
}

e) f5(x, y) = x^2 + y^2
*/

#include <stdio.h>
#include <math.h>
#define M_PI 3.14159265358979323846

float f1(float x) {
  if (x < 0)
    return x + 1;
  else if (x > 0)
    return -x;
  
  return -1;
}

float f2(float x) {
  if (x >= 0 && x < 10)
    return (2 * x) + 4;
  else if (x >= 10 && x < 20)
    return (3 * (x * x)) + (5 * x) - 1;

  return -1;
}

float f3(float x) {
  if (x < M_PI / 2)
    return cos(x);
  else if (x > M_PI / 2)
    return -sin(x);

  return -1;
}

float f4(float x) {
  if (x <= 0)
    return x * x * x;

  return sqrt(x);
}

float f5(float x, float y) {
  return (x * x) + (y * y);
}

int main() {

  // Definir un tipo de puntero a función
  typedef float (*FunctionPointer)(float);
    
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
    float x, y;

    printf("Valor X: ");
    scanf("%f", &x);

    printf("Valor Y: ");
    scanf("%f", &y);

    float result = f5(x, y);

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

  for (float i = inf; i <= sup; i += delta) {
    float result = functions[functionIndex](i);
    printf("f%i(%g) = %g\n", functionIndex, i, result);
  }

  return 0;
}