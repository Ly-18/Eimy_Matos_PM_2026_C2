#include <stdio.h>
#include <stdlib.h>

/*Elección
El programa obtiene el total de votos de cada cadidato y el pueblo y el porcentaje
correspondiente. Tambien considera votos nulos.

VOT, C1, C2, C3, C4, C5, NU, SVO: variables de tipo entero.
P01, P02, P03, P04, P05, PON: variables de tipo real. */

void main(void)
{
    int VOT, C1 = 0, C2 = 0, C3 = 0, C4 = 0, C5 = 0, NU = 0, SVO;
    float P01, P02, P03, P04, P05, P0N, SV0;
    printf("Ingrese el primer voto: ");
    scanf("%d", &VOT);
    while (VOT)
    {
        switch(VOT)
        {
            case 1: C1++; break;
            case 2: C2++; break;
            case 3: C3++; break;
            case 4: C4++; break;
            case 5: C5++; break;
            default: NU++; break;
        }
        printf("Ingresa el siguiente voto -0 para terminar-: ");
        scanf("%d", &VOT);
    }
    SVO = C1 + C2 + C3 + C4 + C5 + NU;
    P01 =((float) C1 / SV0) * 100;
    P02 =((float) C2 / SV0) * 100;
    P03 =((float) C3 / SV0) * 100;
    P04 =((float) C4 / SV0) * 100;
    P05 =((float) C5 / SV0) * 100;
    P0N =((float) NU / SV0) * 100;
    printf("\nTotal de votos: %d", SV0);
    printf("\n\nCandidato 1: %d votos -- Porcentaje: %5.2f", C1, P01);
    printf("\nCandidato 2: %d votos -- Porcentaje: %5.2f", C2, P02);
    printf("\nCandidato 3: %d votos -- Porcentaje: %5.2f", C3, P03);
    printf("\nCandidato 4: %d votos -- Porcentaje: %5.2f", C4, P04);
    printf("\nCandidato 5: %d votos -- Porcentaje: %5.2f", C5, P05);
    printf("\nNulos: %d votos -- Porcentaje: %5.2f", NU, P0N);
}
