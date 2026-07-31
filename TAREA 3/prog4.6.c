#include <stdio.h>
#include <stdlib.h>

/*Prueba de parametros por referencia.*/
void f1 (int *);
/*Prototipo de funcion. El parametro es tipo entero y por referncia
-Observa el uso del operador de indireccion. */

void main(void)
{
int I, K = 4;
for (I = 1; I <= 3; I++)
{
    printf("\n\nValor de K antes de llamar a la funcion: %d", ++K);
    f1(&K);
    printf("\nValor de K despues de llamar a la funcion: %d", K);
    /*Llamada a la funcion f1. Se pasa la direccion de la variable K,
    por medio del operador de dirección: &. */

}
}

void f1(int *R)
/*La funcon f1 recibe un parametro por referencia. Cada vez que el parametro
se utiliza en la funcion edebe ir procedido por el operador
de indireccion */
{
*R += *R;
}
