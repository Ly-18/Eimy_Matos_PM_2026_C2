#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*Suma-cuadrados.
E programa calcula la suma del cuadrado de los elementos de un arreglo
unidimensional de 100 elementos de tipo real. */

const int MAX = 100;
//MAX se utilixa para reservar el espacio maxmo que podrá ocupar el arreglo.

void Lectura (float *, int);    //Prototipos de funciones.
void Suma (float *, int);

void main(void)
{
float VEC[MAX];
double RES;
Lectura(VEC, MAX);
//Se lama a la funcion Suma y se almacena el resultado en la variable RES.
printf("\n\nSuma del arreglo: %.2lf", RES);
}

void Lectura(float A[], int T)
// La funcion lectura se utiliza para leer un arreglo unidimensional de T elementp de tipo real.

{
int I;
for (I=0; I<T; I++);
{
    printf("Ingrese el elemento %d: ", I+1);
    scanf("%f", &A[I]);
}
}
double Suma(float A[], int T)
//La funcion Suma se utiliza para calcular la suma del cuadrado de los componentes de un arreglo unidimencional de T elementos de tipo real.

{
int I;
double AUX = 0.0;
for (I=0; I<T; I++)
    AUX += pow(A[I], 2);
return(AUX);
}
