/*
d) Cálculo de los días, horas, minutos y segundos dada una cantidad en segundos. El
programa debería tener una salida o impresión como la siguiente:
Introduzca el número de segundos: 93662
93662 segundo(s) son:
- 1 dia(s)
- 2 hora(s)
- 1 minuto(s)
- 2 segundo(s)
Considere el uso de constantes en este ejercicio.
*/

#include <stdio.h>

// Constantes de tiempo en segundos
#define SECONDS_IN_MINUTE 60
#define SECONDS_IN_HOUR (SECONDS_IN_MINUTE * 60)
#define SECONDS_IN_DAY (SECONDS_IN_HOUR * 24)

int main() {
  int totalSeconds, restSeconds;

  printf("Introduzca el número de segundos: ");
  scanf("%i", &totalSeconds);

  restSeconds = totalSeconds;

  int days = restSeconds / SECONDS_IN_DAY;
  restSeconds %= SECONDS_IN_DAY;

  int hours = restSeconds / SECONDS_IN_HOUR;
  restSeconds %= SECONDS_IN_HOUR;

  int minutes = restSeconds / SECONDS_IN_MINUTE;
  restSeconds %= SECONDS_IN_MINUTE;


  printf("%i segundo(s) son:\n- %i dia(s)\n- %i hora(s)\n- %i minuto(s)\n- %i segundo(s)\n", totalSeconds, days, hours, minutes, restSeconds);

  return 0;
}