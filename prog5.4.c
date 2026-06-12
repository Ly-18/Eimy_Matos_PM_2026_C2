#include <stdio.h>
#include <stdlib.h>

/*Producto de vectores.
El programa calcula el producto de dos vectores y almacena el resultado
en otro arreglo unidimencional. */

const int MAX = 10;                           //Se define una constante para el tamaño de los arreglos.
void Lectura (int VEC [], int T);
void Imprime (int VEC [], int T);            //Prototipo de funciones.
void Producto (int *X, int *Y, int *Z, int T);     // Observa que en los parámetro, para indicar que lo que se recibe es un arreglo, se puede escribir VEC[] o *VEC. ]

void main (void)
{
int VE1 [MAX], VE2 [MAX], VE3 [MAX];
// se declaran tres arreglos de tipo entero 10 elemntos.
Lectura (VE1, MAX);
//Se llama a la función de lectura. Observa que al paso del arreglo a la funcion es por referencia. SSolo se debe incluir el nombre del arreglo.
Lectura (VE2, MAX);
Producto(VE1, VE2, VE3, MAX);
//Se llama a la funcion producto. Se pasan los nombres de los tres arreglos.
printf("\nProducto de los vectores");
Imprime(VE3, MAX);
}

void Lectura(int VEC[], int T)
//La funcion Lectura se utilizan para leer un arreglo unidimencional de T elementos de tipo entero.

{
int I;
printf("\n");
for (I=0; I<T; I++);

{
    printf("Ingrese el elemento %d: ", I+1);
    scanf("%d", &VEC[I]);
}
}
void Imprime (int VEC [], int T)
//La funcion Imprime se utiliza para imprimir un arreglo unidimensional de T elementos de tipo entero.
{
int I;
 for (I=0; I<T; I++)
printf("\nVEC[%d]: %d", I+1, VEC[I]);
}
void Producto (int *X, int *Y, int *Z, int T)
//Esta funcion se utiliza para calcular el producto de dos arreglos unidimensionales de T para elementos de tipo entero.

{
    int I;
    for (I=0; I<T; I++)
        Z[I] = X[I] * Y[I];
}
