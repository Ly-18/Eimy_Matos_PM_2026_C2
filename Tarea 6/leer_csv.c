#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nombre[50];
    char apellido[50];
    float promedio;
    char materia[50];
} Alumno;

int main() {
    FILE *archivo = fopen("alumnos.csv", "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo alumnos.csv\n");
        return 1;
    }

    char linea[256];
    int n = 0;

    /* PRIMERA LINEA del archivo: cantidad de elementos */
    fgets(linea, sizeof(linea), archivo);
    n = atoi(linea);

    if (n == 0) {
        printf("No se encontro la cantidad de elementos.\n");
        fclose(archivo);
        return 1;
    }

    /* Reservamos memoria dinamica ahora que sabemos cuantos alumnos hay */
    Alumno *alumnos = (Alumno *) malloc(n * sizeof(Alumno));
    if (alumnos == NULL) {
        printf("Error al reservar memoria\n");
        fclose(archivo);
        return 1;
    }

    /* SEGUNDA LINEA: encabezado, la saltamos */
    fgets(linea, sizeof(linea), archivo);

    /* Leemos las n lineas de datos */
    for (int i = 0; i < n; i++) {
        if (fgets(linea, sizeof(linea), archivo) == NULL) break;

        char *token = strtok(linea, ",");
        strcpy(alumnos[i].nombre, token);

        token = strtok(NULL, ",");
        strcpy(alumnos[i].apellido, token);

        token = strtok(NULL, ",");
        alumnos[i].promedio = atof(token);

        token = strtok(NULL, ",\n");
        strcpy(alumnos[i].materia, token);
    }

    fclose(archivo);

    /* Imprimir los alumnos cargados en memoria */
    printf("Cantidad de alumnos leidos: %d\n\n", n);
    for (int i = 0; i < n; i++) {
        printf("Alumno %d:\n", i + 1);
        printf("  Nombre:   %s\n", alumnos[i].nombre);
        printf("  Apellido: %s\n", alumnos[i].apellido);
        printf("  Promedio: %.2f\n", alumnos[i].promedio);
        printf("  Materia:  %s\n\n", alumnos[i].materia);
    }

    free(alumnos);
    return 0;
}
