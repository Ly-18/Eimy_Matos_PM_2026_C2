#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nombre[50];
    char apellido[50];
    float promedio;
    char materia[50];
} Alumno;

/* Extrae el texto que esta entre <Tag> y </Tag> en una linea */
void extraerValor(const char *linea, const char *tag, char *destino) {
    char aperturaTag[30], cierreTag[30];
    sprintf(aperturaTag, "<%s>", tag);
    sprintf(cierreTag, "</%s>", tag);

    char *inicio = strstr(linea, aperturaTag);
    char *fin = strstr(linea, cierreTag);

    if (inicio != NULL && fin != NULL) {
        inicio += strlen(aperturaTag);
        int longitud = fin - inicio;
        strncpy(destino, inicio, longitud);
        destino[longitud] = '\0';
    } else {
        destino[0] = '\0';
    }
}

int main() {
    FILE *archivo = fopen("alumnos.xml", "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo alumnos.xml\n");
        return 1;
    }

    char linea[256];
    int n = 0;

    /* PRIMERA PASADA: buscar el atributo total="N" para saber cuantos elementos hay */
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        char *pos = strstr(linea, "total=\"");
        if (pos != NULL) {
            sscanf(pos, "total=\"%d\"", &n);
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
    char temp[50];

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        if (strstr(linea, "<Alumno>") != NULL) {
            indice++;
        } else if (strstr(linea, "<Nombre>") != NULL) {
            extraerValor(linea, "Nombre", alumnos[indice].nombre);
        } else if (strstr(linea, "<Apellido>") != NULL) {
            extraerValor(linea, "Apellido", alumnos[indice].apellido);
        } else if (strstr(linea, "<Promedio>") != NULL) {
            extraerValor(linea, "Promedio", temp);
            alumnos[indice].promedio = atof(temp);
        } else if (strstr(linea, "<Materia>") != NULL) {
            extraerValor(linea, "Materia", alumnos[indice].materia);
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
