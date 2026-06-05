#include <stdio.h>
#include <stdlib.h>

/*Cubo -1
El programa calcula el cubo de los 10 primeros números naturales con ayuda de una función.
En la solucion del problema se utiliza una variable global,
aunque esto, como veremos mas adelante, no es muy recomendable. */

int cubo(void);        /*Prototipo de función. */
int I;                 /*Variable global. */

void main(void)
{
int CUB;
for (I = 1; I <= 10; I++)
{
    CUB = cubo();      /*Llamada a la función cubo. */

    printf("\nEl cubo de %d es: %d", I, CUB);
}
}
int cubo (void)      /*Declaracion de la funcion. */
/*La funcion calcula el cubo de la variable global I. */
{
    return(I*I*I);
}
