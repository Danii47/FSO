#include <stdio.h>

double fib(int n);

int main() {
  int b[3] = {1, 2, 3};
  int c[3] = {0, 0, 0};
  b[3] = 5;
  printf("%p", &c[0]);
  printf("Hello world!\n");
  printf("%g\n", fib(6));
  return 0;
}

double fib(int n) {
  if (n <= 2) return 1;
  return fib(n - 1) + fib(n - 2);
}