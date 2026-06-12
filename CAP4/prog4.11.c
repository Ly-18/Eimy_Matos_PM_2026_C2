#include <stdio.h>
#include <stdlib.h>

/*Mayor divisor.
El programa, al recibir como dato un número entero positivo, calcula
su mayor divisor. */
int mad (int);            /*Prototipo de funcion. */
void main (void)
{
int NUM, RES;
printf("\nIngresa el numero: ");
scanf("%d", &NUM);
RES = mad(NUM);
printf("\nEl mayor divisor de %d es: %d", NUM, RES);
}
int mad(int N1)
/*Esta funcion calcula el mayor divisor del numero N1. */
{
int I = (N1 / 2);
/*I se inicializa con el maximo valor posible puede ser el divisor de N1*/
while(N1 % I)
/*El ciclo se mantiene activo mientras (N1 % I) sea distinto de cero.
Cuando el resultado sea 0, se detiene, ya que habrá encontrado el
mayor divisor de N1. */
I--;
return I;
}
