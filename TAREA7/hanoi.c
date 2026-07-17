#include <stdio.h>

/* Funcion recursiva que resuelve las Torres de Hanoi.
   Cada vez que mueve un disco, aumenta el contador y
   graba el movimiento tanto en pantalla como en el archivo. */
void hanoi(int n, char origen, char auxiliar, char destino, int *contador, FILE *archivo) {
    if (n == 0) {
        return;
    }

    /* Mover los n-1 discos superiores del origen al auxiliar */
    hanoi(n - 1, origen, destino, auxiliar, contador, archivo);

    /* Mover el disco n (el mas grande de este grupo) del origen al destino */
    (*contador)++;
    printf("Movimiento %d: disco %d de %c a %c\n", *contador, n, origen, destino);
    fprintf(archivo, "Movimiento %d: disco %d de %c a %c\n", *contador, n, origen, destino);

    /* Mover los n-1 discos del auxiliar al destino */
    hanoi(n - 1, auxiliar, origen, destino, contador, archivo);
}

int main() {
    int discos;

    printf("Ingresa el numero de discos: ");
    scanf("%d", &discos);

    FILE *archivo = fopen("movimientos.txt", "w");
    if (archivo == NULL) {
        printf("No se pudo crear el archivo movimientos.txt\n");
        return 1;
    }

    int contador = 0;
    hanoi(discos, 'A', 'B', 'C', &contador, archivo);

    fclose(archivo);

    printf("\nTotal de movimientos: %d\n", contador);
    printf("Los movimientos se guardaron en movimientos.txt\n");

    return 0;
}
