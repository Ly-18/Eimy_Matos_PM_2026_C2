#include <stdio.h>
#include <stdlib.h>

/* Billete de ferrocarril.
El programa calcula el costo de un billete de ferrocaril teniendo en
cuenta la distancia entre dos ciudades y el tiempo de permanencia
del pasajero.

DIS y TIE: variable de tipo entero.
BIL: variable de tipo real. */

void main(void)
{
    int DIS, TIE;
    float BIL;
    printf("Ingrese la distancia entre ciudades y el tiempo de estancia: ");
    scanf("%d %d", &DIS, &TIE);
    if ((DIS*2 > 500) && (TIE > 10))
        BIL = DIS * 2 * 0.19 * 0.8;
    else
        BIL = DIS * 2 * 0.19;
    printf("\n\mCosto del billete: %7.2f", BIL);
}
