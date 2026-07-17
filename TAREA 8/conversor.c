#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define MAX_ARCHIVOS 100
#define CARPETA "imagenes"

/* Revisa si el nombre del archivo termina en .png, .jpg, .jpeg o .bmp (sin importar mayusculas/minusculas) */
int esImagenValida(const char *nombre) {
    const char *punto = strrchr(nombre, '.');
    if (punto == NULL) return 0;

    char ext[10];
    strcpy(ext, punto);
    for (int i = 0; ext[i]; i++) {
        ext[i] = tolower(ext[i]);
    }

    if (strcmp(ext, ".png") == 0) return 1;
    if (strcmp(ext, ".jpg") == 0) return 1;
    if (strcmp(ext, ".jpeg") == 0) return 1;
    if (strcmp(ext, ".bmp") == 0) return 1;

    return 0;
}

/* Llena el arreglo 'lista' con los nombres de archivos de imagen encontrados en la carpeta.
   Regresa la cantidad de archivos encontrados. */
int listarImagenes(const char *carpeta, char lista[][256]) {
    DIR *dir = opendir(carpeta);
    if (dir == NULL) {
        printf("No se pudo abrir la carpeta '%s'\n", carpeta);
        return 0;
    }

    int cantidad = 0;
    struct dirent *entrada;

    while ((entrada = readdir(dir)) != NULL && cantidad < MAX_ARCHIVOS) {
        if (esImagenValida(entrada->d_name)) {
            strcpy(lista[cantidad], entrada->d_name);
            cantidad++;
        }
    }

    closedir(dir);
    return cantidad;
}

/* Convierte el buffer RGB cargado a un buffer de un solo canal (gris),
   usando la formula de luminancia estandar. */
unsigned char *convertirAGris(unsigned char *imagenRGB, int ancho, int alto, int canalesOriginales) {
    unsigned char *gris = malloc(ancho * alto);
    if (gris == NULL) return NULL;

    for (int i = 0; i < ancho * alto; i++) {
        unsigned char r = imagenRGB[i * canalesOriginales + 0];
        unsigned char g = imagenRGB[i * canalesOriginales + 1];
        unsigned char b = imagenRGB[i * canalesOriginales + 2];

        /* Formula de luminancia (le da mas peso al verde, que el ojo percibe mas) */
        gris[i] = (unsigned char)(0.299 * r + 0.587 * g + 0.114 * b);
    }

    return gris;
}

/* Guarda el buffer en gris con el mismo formato (extension) que el archivo original */
void guardarImagenGris(const char *nombreOriginal, unsigned char *gris, int ancho, int alto) {
    char rutaSalida[300];
    sprintf(rutaSalida, "%s/gris_%s", CARPETA, nombreOriginal);

    const char *punto = strrchr(nombreOriginal, '.');
    char ext[10];
    strcpy(ext, punto);
    for (int i = 0; ext[i]; i++) {
        ext[i] = tolower(ext[i]);
    }

    if (strcmp(ext, ".png") == 0) {
        stbi_write_png(rutaSalida, ancho, alto, 1, gris, ancho);
    } else if (strcmp(ext, ".bmp") == 0) {
        stbi_write_bmp(rutaSalida, ancho, alto, 1, gris);
    } else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) {
        stbi_write_jpg(rutaSalida, ancho, alto, 1, gris, 90);
    }

    printf("Imagen guardada como: %s\n", rutaSalida);
}

int main() {
    char listaArchivos[MAX_ARCHIVOS][256];
    int cantidad = listarImagenes(CARPETA, listaArchivos);

    if (cantidad == 0) {
        printf("No se encontraron imagenes (.png, .jpg, .bmp) en la carpeta '%s'\n", CARPETA);
        return 1;
    }

    printf("Imagenes encontradas en '%s':\n\n", CARPETA);
    for (int i = 0; i < cantidad; i++) {
        printf("  %d. %s\n", i + 1, listaArchivos[i]);
    }

    int opcion;
    printf("\nSelecciona el numero de la imagen a convertir: ");
    scanf("%d", &opcion);

    if (opcion < 1 || opcion > cantidad) {
        printf("Opcion invalida.\n");
        return 1;
    }

    char rutaEntrada[300];
    sprintf(rutaEntrada, "%s/%s", CARPETA, listaArchivos[opcion - 1]);

    int ancho, alto, canalesOriginales;
    unsigned char *imagenRGB = stbi_load(rutaEntrada, &ancho, &alto, &canalesOriginales, 3);

    if (imagenRGB == NULL) {
        printf("No se pudo cargar la imagen '%s'\n", rutaEntrada);
        return 1;
    }

    printf("\nImagen cargada: %dx%d pixeles\n", ancho, alto);

    unsigned char *imagenGris = convertirAGris(imagenRGB, ancho, alto, 3);
    if (imagenGris == NULL) {
        printf("Error al reservar memoria para la imagen en gris.\n");
        stbi_image_free(imagenRGB);
        return 1;
    }

    guardarImagenGris(listaArchivos[opcion - 1], imagenGris, ancho, alto);

    stbi_image_free(imagenRGB);
    free(imagenGris);

    return 0;
}
