#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nombre[50];
    char apellido[50];
    float promedio;
    char materia[50];
} Alumno;

/* Extrae el valor de tipo texto que esta entre comillas despues de "campo": " */
void extraerTexto(const char *linea, const char *campo, char *destino) {
    char buscar[30];
    sprintf(buscar, "\"%s\"", campo);

    char *pos = strstr(linea, buscar);
    if (pos == NULL) {
        destino[0] = '\0';
        return;
    }

    /* Avanzamos hasta despues de los dos puntos */
    pos = strchr(pos, ':');
    pos = strchr(pos, '"');   /* primera comilla del valor */
    pos++;
    char *fin = strchr(pos, '"'); /* comilla de cierre */

    int longitud = fin - pos;
    strncpy(destino, pos, longitud);
    destino[longitud] = '\0';
}

/* Extrae el valor numerico que viene despues de "campo": (sin comillas) */
float extraerNumero(const char *linea, const char *campo) {
    char buscar[30];
    sprintf(buscar, "\"%s\"", campo);

    char *pos = strstr(linea, buscar);
    if (pos == NULL) return 0.0;

    pos = strchr(pos, ':');
    pos++;
    return atof(pos);
}

int main() {
    FILE *archivo = fopen("alumnos.json", "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo alumnos.json\n");
        return 1;
    }

    char linea[256];
    int n = 0;

    /* PRIMERA PASADA: buscar "total": N para saber cuantos elementos hay */
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        if (strstr(linea, "\"total\"") != NULL) {
            char *pos = strchr(linea, ':');
            pos++;
            n = atoi(pos);
            break;
        }
    }

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

    /* SEGUNDA PASADA: volvemos al inicio del archivo para leer cada alumno */
    rewind(archivo);

    int indice = -1;
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        if (strstr(linea, "\"nombre\"") != NULL) {
            indice++;
            extraerTexto(linea, "nombre", alumnos[indice].nombre);
            extraerTexto(linea, "apellido", alumnos[indice].apellido);
            alumnos[indice].promedio = extraerNumero(linea, "promedio");
            extraerTexto(linea, "materia", alumnos[indice].materia);
        }
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
