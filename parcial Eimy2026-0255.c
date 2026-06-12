#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int Opciones;
    int Jugar;
    int Elegir = 0;
    float Dinero;
    int NumeroGanador;
    int i;

    printf("Bienvenido a LoteDom!\n");
    srand(time(NULL));

    do
    {
        printf("\nMenu:\n1.Jugar\n2.Elegir cantidad de jugadas\n3.Salir\n");
        scanf("%d", &Opciones);

        if (Opciones == 1)
        {
            if (Elegir == 0)
            {
                printf("Primero elige la cantidad de jugadas: ");
                scanf("%d", &Elegir);
            }

            for (i = 0; i < Elegir; i++)
            {
                printf("Ingresa tu numero (1-40): ");
                scanf("%d", &Jugar);

                if (Jugar < 1 || Jugar > 40)
                {
                    printf("Numero invalido!\n");
                }
                else
                {
                    printf("Ingresa el dinero a jugar: ");
                    scanf("%f", &Dinero);

                    NumeroGanador = rand() % 40 + 1;
                    printf("Numero ganador: %d\n", NumeroGanador);

                    if (Jugar == NumeroGanador)
                        printf("Ganaste $%.2f!\n", Dinero * 1000);
                    else
                        printf("Perdiste. Suerte la proxima!\n");
                }
            }
        }
        else if (Opciones == 2)
        {
            printf("Elegir cantidad de jugadas: ");
            scanf("%d", &Elegir);
        }
        else
        {
            printf("Gracias por jugar!\n");
        }

    } while (Opciones != 3);

    return 0;
}
