#include <stdio.h>
#include <stdlib.h>

/*Prueba de parametros por valor. */

int f1 (int);        /*Prototipo de funcion. El parametro e por valor y de un tipo entero. */
void main(void)
{
    int I, K = 4;
    for (I = 2; I <= 3; I++)
    {
        printf("\n\nValor de K antes de llamar a la funcion:  %d",++K);
        printf("\nValor de K despues de llamar a la funcion: %d", f1(K));
        /*Llamada a ala funcion f1. Se pasa una copia de la variable K. */
    }
}
int f1 (int R)
{
    R += R;
    return (R);
}
