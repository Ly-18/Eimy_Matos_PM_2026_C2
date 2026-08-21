/* ============================================================================
   TAREA 11 - MÉTODO MOSLER PARA LA GESTIÓN DE RIESGOS
   ----------------------------------------------------------------------------
   Requisitos cubiertos:
   2. Crear riesgo
   3. Modificar parámetros del riesgo
   4. Calcular riesgos
   5. Imprimir los riesgos
   6. Borrar riesgo
   7. Seleccionar un archivo específico
   8. Crear un archivo específico
   9. Memoria dinámica para cargar los riesgos desde disco
   10. Guardar en archivo plano [texto] [json] [xml] [csv]
   11. Usar IA (módulo de análisis/recomendación basado en reglas expertas)
   12. Usar librerías estándar de C (stdio, stdlib, string, ctype)

   Fórmulas del método Mosler:
     I  = F x S              (Importancia)
     D  = P x E               (Daños)
     C  = I + D               (Carácter del riesgo)
     Pb = A x V                (Probabilidad)
     ER = C x Pb               (Evaluación del Riesgo)

   Clasificación de ER (según tabla estándar del método):
     0    - 250   -> Muy Pequeño
     251  - 500   -> Pequeño
     501  - 750   -> Normal
     751  - 1000  -> Grande
     > 1000       -> Muy Grande
   ============================================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STR      150
#define MAX_NOMBRE   100
#define MAX_ARCHIVO  200
#define CAP_INICIAL  4

/* ---------------------------- Estructuras ------------------------------- */

typedef struct {
    int  id;
    char bien[MAX_NOMBRE];      /* El bien expuesto (Fase 1)               */
    char riesgo[MAX_STR];       /* Descripción del riesgo (Fase 1)         */
    char dano[MAX_STR];         /* Daño esperado (Fase 1)                  */

    /* Fase 2: criterios de análisis (escala 1 a 5) */
    int F; /* Función      */
    int S; /* Sustitución  */
    int P; /* Profundidad  */
    int E; /* Extensión    */
    int A; /* Agresión     */
    int V; /* Vulnerabilidad */

    /* Fase 3 y 4: resultados calculados */
    int  I;   /* Importancia = F*S   */
    int  D;   /* Daños = P*E         */
    int  C;   /* Carácter = I+D      */
    int  Pb;  /* Probabilidad = A*V  */
    int  ER;  /* Evaluación del riesgo = C*Pb */
    char clase[30]; /* Clasificación final */
} Riesgo;

typedef struct {
    Riesgo *riesgos;
    int     cantidad;
    int     capacidad;
    char    archivoActual[MAX_ARCHIVO];
} GestorRiesgos;

/* ------------------------- Utilidades de entrada ------------------------- */

void limpiarBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int leerEntero(const char *prompt, int min, int max) {
    int valor;
    int ok;
    do {
        printf("%s", prompt);
        ok = scanf("%d", &valor);
        limpiarBuffer();
        if (!ok) {
            printf("  -> Entrada invalida. Ingrese un numero.\n");
            valor = min - 1;
            continue;
        }
        if (valor < min || valor > max) {
            printf("  -> El valor debe estar entre %d y %d.\n", min, max);
        }
    } while (valor < min || valor > max);
    return valor;
}

void leerCadena(const char *prompt, char *destino, int tam) {
    printf("%s", prompt);
    if (fgets(destino, tam, stdin) != NULL) {
        size_t len = strlen(destino);
        if (len > 0 && destino[len - 1] == '\n') destino[len - 1] = '\0';
    }
}

/* ---------------------------- Gestor de memoria --------------------------- */

void inicializarGestor(GestorRiesgos *g) {
    g->capacidad = CAP_INICIAL;
    g->cantidad  = 0;
    g->riesgos   = (Riesgo *) malloc(sizeof(Riesgo) * g->capacidad);
    if (g->riesgos == NULL) {
        printf("Error critico: no se pudo asignar memoria.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(g->archivoActual, "riesgos.csv");
}

void asegurarCapacidad(GestorRiesgos *g) {
    if (g->cantidad >= g->capacidad) {
        int nuevaCap = g->capacidad * 2;
        Riesgo *tmp = (Riesgo *) realloc(g->riesgos, sizeof(Riesgo) * nuevaCap);
        if (tmp == NULL) {
            printf("Error critico: no se pudo ampliar la memoria.\n");
            exit(EXIT_FAILURE);
        }
        g->riesgos  = tmp;
        g->capacidad = nuevaCap;
    }
}

void liberarMemoria(GestorRiesgos *g) {
    free(g->riesgos);
    g->riesgos   = NULL;
    g->cantidad  = 0;
    g->capacidad = 0;
}

/* ------------------------------- Cálculo Mosler --------------------------- */

void clasificarRiesgo(int ER, char *clase) {
    if (ER <= 250)      strcpy(clase, "Muy Pequeno");
    else if (ER <= 500)  strcpy(clase, "Pequeno");
    else if (ER <= 750)  strcpy(clase, "Normal");
    else if (ER <= 1000) strcpy(clase, "Grande");
    else                 strcpy(clase, "Muy Grande");
}

void calcularRiesgo(Riesgo *r) {
    r->I  = r->F * r->S;
    r->D  = r->P * r->E;
    r->C  = r->I + r->D;
    r->Pb = r->A * r->V;
    r->ER = r->C * r->Pb;
    clasificarRiesgo(r->ER, r->clase);
}

void calcularTodosLosRiesgos(GestorRiesgos *g) {
    if (g->cantidad == 0) {
        printf("\nNo hay riesgos registrados para calcular.\n");
        return;
    }
    for (int i = 0; i < g->cantidad; i++) {
        calcularRiesgo(&g->riesgos[i]);
    }
    printf("\nSe recalcularon %d riesgo(s) correctamente.\n", g->cantidad);
}

/* --------------------------------- IA (reglas) ----------------------------
   Módulo de análisis inteligente basado en reglas expertas: identifica el
   criterio dominante y genera una recomendación de tratamiento del riesgo.
   No requiere conexión a internet: es un sistema experto local.
   ---------------------------------------------------------------------- */

void moduloIA(const Riesgo *r) {
    printf("\n---- Analisis Inteligente (IA basada en reglas) ----\n");
    printf("Riesgo: %s | Clasificacion: %s (ER=%d)\n", r->riesgo, r->clase, r->ER);

    /* Encontrar el criterio con mayor peso */
    struct { const char *nombre; int valor; } criterios[6] = {
        {"Funcion", r->F}, {"Sustitucion", r->S}, {"Profundidad", r->P},
        {"Extension", r->E}, {"Agresion", r->A}, {"Vulnerabilidad", r->V}
    };
    int idxMax = 0;
    for (int i = 1; i < 6; i++) {
        if (criterios[i].valor > criterios[idxMax].valor) idxMax = i;
    }
    printf("Factor critico dominante: %s (valor %d/5)\n",
           criterios[idxMax].nombre, criterios[idxMax].valor);

    if (strcmp(r->clase, "Muy Grande") == 0 || strcmp(r->clase, "Grande") == 0) {
        printf("Recomendacion: PRIORIDAD ALTA. Se sugiere reduccion inmediata\n");
        printf("del riesgo, planes de contingencia y revision de polizas de\n");
        printf("seguro. Evaluar transferencia o eliminacion del riesgo.\n");
    } else if (strcmp(r->clase, "Normal") == 0) {
        printf("Recomendacion: PRIORIDAD MEDIA. Establecer controles preventivos\n");
        printf("periodicos y monitoreo constante del bien expuesto.\n");
    } else {
        printf("Recomendacion: PRIORIDAD BAJA. Mantener controles actuales y\n");
        printf("revisar el riesgo dentro del ciclo de mejora continua.\n");
    }

    if (r->A >= 4) printf("Nota IA: la probabilidad de agresion es alta; reforzar prevencion.\n");
    if (r->V >= 4) printf("Nota IA: la vulnerabilidad es alta; reforzar proteccion del bien.\n");
    printf("------------------------------------------------------\n");
}

/* ------------------------------- CRUD de riesgos --------------------------- */

void crearRiesgo(GestorRiesgos *g) {
    asegurarCapacidad(g);
    Riesgo *r = &g->riesgos[g->cantidad];

    printf("\n===== CREAR NUEVO RIESGO (Fase 1: Definicion) =====\n");
    r->id = g->cantidad + 1;
    leerCadena("Bien expuesto (ej: vehiculo blindado): ", r->bien, MAX_NOMBRE);
    leerCadena("Descripcion del riesgo (ej: incendio electrico): ", r->riesgo, MAX_STR);
    leerCadena("Dano esperado (ej: perdida total): ", r->dano, MAX_STR);

    printf("\n----- Fase 2: Analisis del riesgo (escala 1 a 5) -----\n");
    r->F = leerEntero("Funcion (gravedad del dano en la organizacion) [1-5]: ", 1, 5);
    r->S = leerEntero("Sustitucion (dificultad de reemplazo) [1-5]: ", 1, 5);
    r->P = leerEntero("Profundidad (afectacion psicologica/imagen) [1-5]: ", 1, 5);
    r->E = leerEntero("Extension (alcance del dano) [1-5]: ", 1, 5);
    r->A = leerEntero("Agresion (probabilidad de manifestarse) [1-5]: ", 1, 5);
    r->V = leerEntero("Vulnerabilidad (probabilidad de dano al ocurrir) [1-5]: ", 1, 5);

    calcularRiesgo(r);
    g->cantidad++;

    printf("\nRiesgo #%d creado y calculado correctamente. ER=%d (%s)\n",
           r->id, r->ER, r->clase);
}

Riesgo* buscarPorId(GestorRiesgos *g, int id) {
    for (int i = 0; i < g->cantidad; i++) {
        if (g->riesgos[i].id == id) return &g->riesgos[i];
    }
    return NULL;
}

void modificarRiesgo(GestorRiesgos *g) {
    if (g->cantidad == 0) {
        printf("\nNo hay riesgos para modificar.\n");
        return;
    }
    int id = leerEntero("\nID del riesgo a modificar: ", 1, 999999);
    Riesgo *r = buscarPorId(g, id);
    if (r == NULL) {
        printf("No se encontro un riesgo con ese ID.\n");
        return;
    }

    int opcion;
    do {
        printf("\n--- Modificando Riesgo #%d (%s) ---\n", r->id, r->riesgo);
        printf("1. Bien expuesto (%s)\n", r->bien);
        printf("2. Descripcion del riesgo (%s)\n", r->riesgo);
        printf("3. Dano esperado (%s)\n", r->dano);
        printf("4. Funcion F (%d)\n", r->F);
        printf("5. Sustitucion S (%d)\n", r->S);
        printf("6. Profundidad P (%d)\n", r->P);
        printf("7. Extension E (%d)\n", r->E);
        printf("8. Agresion A (%d)\n", r->A);
        printf("9. Vulnerabilidad V (%d)\n", r->V);
        printf("0. Terminar edicion\n");
        opcion = leerEntero("Seleccione el campo a modificar: ", 0, 9);

        switch (opcion) {
            case 1: leerCadena("Nuevo bien: ", r->bien, MAX_NOMBRE); break;
            case 2: leerCadena("Nueva descripcion: ", r->riesgo, MAX_STR); break;
            case 3: leerCadena("Nuevo dano: ", r->dano, MAX_STR); break;
            case 4: r->F = leerEntero("Nuevo valor F [1-5]: ", 1, 5); break;
            case 5: r->S = leerEntero("Nuevo valor S [1-5]: ", 1, 5); break;
            case 6: r->P = leerEntero("Nuevo valor P [1-5]: ", 1, 5); break;
            case 7: r->E = leerEntero("Nuevo valor E [1-5]: ", 1, 5); break;
            case 8: r->A = leerEntero("Nuevo valor A [1-5]: ", 1, 5); break;
            case 9: r->V = leerEntero("Nuevo valor V [1-5]: ", 1, 5); break;
        }
    } while (opcion != 0);

    calcularRiesgo(r);
    printf("Riesgo actualizado. Nuevo ER=%d (%s)\n", r->ER, r->clase);
}

void borrarRiesgo(GestorRiesgos *g) {
    if (g->cantidad == 0) {
        printf("\nNo hay riesgos para borrar.\n");
        return;
    }
    int id = leerEntero("\nID del riesgo a borrar: ", 1, 999999);
    int pos = -1;
    for (int i = 0; i < g->cantidad; i++) {
        if (g->riesgos[i].id == id) { pos = i; break; }
    }
    if (pos == -1) {
        printf("No se encontro un riesgo con ese ID.\n");
        return;
    }
    for (int i = pos; i < g->cantidad - 1; i++) {
        g->riesgos[i] = g->riesgos[i + 1];
    }
    g->cantidad--;
    printf("Riesgo #%d eliminado correctamente.\n", id);
}

void imprimirUnRiesgo(const Riesgo *r) {
    printf("\n--------------------------------------------------------\n");
    printf(" Riesgo #%d\n", r->id);
    printf(" Bien:      %s\n", r->bien);
    printf(" Riesgo:    %s\n", r->riesgo);
    printf(" Dano:      %s\n", r->dano);
    printf(" F=%d S=%d P=%d E=%d A=%d V=%d\n", r->F, r->S, r->P, r->E, r->A, r->V);
    printf(" I=%d  D=%d  C=%d  Pb=%d  ER=%d\n", r->I, r->D, r->C, r->Pb, r->ER);
    printf(" Clasificacion: %s\n", r->clase);
    printf("--------------------------------------------------------\n");
}

void imprimirRiesgos(GestorRiesgos *g) {
    if (g->cantidad == 0) {
        printf("\nNo hay riesgos registrados.\n");
        return;
    }
    printf("\n========== LISTADO DE RIESGOS (%d) ==========\n", g->cantidad);
    for (int i = 0; i < g->cantidad; i++) {
        imprimirUnRiesgo(&g->riesgos[i]);
    }
}

/* ------------------------------ Manejo de archivos -------------------------- */

void seleccionarArchivo(GestorRiesgos *g) {
    char nombre[MAX_ARCHIVO];
    leerCadena("\nNombre del archivo a seleccionar (ej: datos.csv): ", nombre, MAX_ARCHIVO);
    FILE *f = fopen(nombre, "r");
    if (f == NULL) {
        printf("El archivo '%s' no existe. Puede crearlo con la opcion correspondiente.\n", nombre);
        return;
    }
    fclose(f);
    strcpy(g->archivoActual, nombre);
    printf("Archivo activo establecido en: %s\n", g->archivoActual);
}

void crearArchivo(GestorRiesgos *g) {
    char nombre[MAX_ARCHIVO];
    leerCadena("\nNombre del nuevo archivo a crear (ej: nuevo.csv): ", nombre, MAX_ARCHIVO);
    FILE *f = fopen(nombre, "w");
    if (f == NULL) {
        printf("No se pudo crear el archivo.\n");
        return;
    }
    fprintf(f, "id,bien,riesgo,dano,F,S,P,E,A,V,I,D,C,Pb,ER,clase\n");
    fclose(f);
    strcpy(g->archivoActual, nombre);
    printf("Archivo '%s' creado y establecido como archivo activo.\n", nombre);
}

/* Obtiene el nombre base (sin extension) del archivo activo */
void obtenerNombreBase(const char *archivo, char *base, int tam) {
    strncpy(base, archivo, tam - 1);
    base[tam - 1] = '\0';
    char *punto = strrchr(base, '.');
    if (punto != NULL) *punto = '\0';
}

void guardarCSV(GestorRiesgos *g, const char *nombreArchivo) {
    FILE *f = fopen(nombreArchivo, "w");
    if (f == NULL) { printf("Error al escribir el archivo CSV.\n"); return; }
    fprintf(f, "id,bien,riesgo,dano,F,S,P,E,A,V,I,D,C,Pb,ER,clase\n");
    for (int i = 0; i < g->cantidad; i++) {
        Riesgo *r = &g->riesgos[i];
        fprintf(f, "%d,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s\n",
                r->id, r->bien, r->riesgo, r->dano, r->F, r->S, r->P, r->E, r->A, r->V,
                r->I, r->D, r->C, r->Pb, r->ER, r->clase);
    }
    fclose(f);
    printf("Guardado en formato CSV: %s\n", nombreArchivo);
}

void guardarTXT(GestorRiesgos *g, const char *nombreArchivo) {
    FILE *f = fopen(nombreArchivo, "w");
    if (f == NULL) { printf("Error al escribir el archivo de texto.\n"); return; }
    fprintf(f, "REPORTE DE RIESGOS - METODO MOSLER\n");
    fprintf(f, "====================================\n\n");
    for (int i = 0; i < g->cantidad; i++) {
        Riesgo *r = &g->riesgos[i];
        fprintf(f, "Riesgo #%d\n", r->id);
        fprintf(f, "  Bien: %s\n", r->bien);
        fprintf(f, "  Riesgo: %s\n", r->riesgo);
        fprintf(f, "  Dano: %s\n", r->dano);
        fprintf(f, "  F=%d S=%d P=%d E=%d A=%d V=%d\n", r->F, r->S, r->P, r->E, r->A, r->V);
        fprintf(f, "  I=%d D=%d C=%d Pb=%d ER=%d\n", r->I, r->D, r->C, r->Pb, r->ER);
        fprintf(f, "  Clasificacion: %s\n\n", r->clase);
    }
    fclose(f);
    printf("Guardado en formato TXT: %s\n", nombreArchivo);
}

void guardarJSON(GestorRiesgos *g, const char *nombreArchivo) {
    FILE *f = fopen(nombreArchivo, "w");
    if (f == NULL) { printf("Error al escribir el archivo JSON.\n"); return; }
    fprintf(f, "{\n  \"riesgos\": [\n");
    for (int i = 0; i < g->cantidad; i++) {
        Riesgo *r = &g->riesgos[i];
        fprintf(f, "    {\n");
        fprintf(f, "      \"id\": %d,\n", r->id);
        fprintf(f, "      \"bien\": \"%s\",\n", r->bien);
        fprintf(f, "      \"riesgo\": \"%s\",\n", r->riesgo);
        fprintf(f, "      \"dano\": \"%s\",\n", r->dano);
        fprintf(f, "      \"F\": %d, \"S\": %d, \"P\": %d, \"E\": %d, \"A\": %d, \"V\": %d,\n",
                r->F, r->S, r->P, r->E, r->A, r->V);
        fprintf(f, "      \"I\": %d, \"D\": %d, \"C\": %d, \"Pb\": %d, \"ER\": %d,\n",
                r->I, r->D, r->C, r->Pb, r->ER);
        fprintf(f, "      \"clase\": \"%s\"\n", r->clase);
        fprintf(f, "    }%s\n", (i < g->cantidad - 1) ? "," : "");
    }
    fprintf(f, "  ]\n}\n");
    fclose(f);
    printf("Guardado en formato JSON: %s\n", nombreArchivo);
}

void guardarXML(GestorRiesgos *g, const char *nombreArchivo) {
    FILE *f = fopen(nombreArchivo, "w");
    if (f == NULL) { printf("Error al escribir el archivo XML.\n"); return; }
    fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<riesgos>\n");
    for (int i = 0; i < g->cantidad; i++) {
        Riesgo *r = &g->riesgos[i];
        fprintf(f, "  <riesgo id=\"%d\">\n", r->id);
        fprintf(f, "    <bien>%s</bien>\n", r->bien);
        fprintf(f, "    <descripcion>%s</descripcion>\n", r->riesgo);
        fprintf(f, "    <dano>%s</dano>\n", r->dano);
        fprintf(f, "    <F>%d</F><S>%d</S><P>%d</P><E>%d</E><A>%d</A><V>%d</V>\n",
                r->F, r->S, r->P, r->E, r->A, r->V);
        fprintf(f, "    <I>%d</I><D>%d</D><C>%d</C><Pb>%d</Pb><ER>%d</ER>\n",
                r->I, r->D, r->C, r->Pb, r->ER);
        fprintf(f, "    <clase>%s</clase>\n", r->clase);
        fprintf(f, "  </riesgo>\n");
    }
    fprintf(f, "</riesgos>\n");
    fclose(f);
    printf("Guardado en formato XML: %s\n", nombreArchivo);
}

void guardarArchivo(GestorRiesgos *g) {
    if (g->cantidad == 0) {
        printf("\nNo hay riesgos para guardar.\n");
        return;
    }
    char base[MAX_ARCHIVO];
    obtenerNombreBase(g->archivoActual, base, MAX_ARCHIVO);

    printf("\n--- Guardar riesgos ---\n");
    printf("Archivo base: %s\n", base);
    printf("1. Texto plano (.txt)\n2. CSV (.csv)\n3. JSON (.json)\n4. XML (.xml)\n5. Todos los formatos\n");
    int opcion = leerEntero("Seleccione formato: ", 1, 5);

    char nombre[MAX_ARCHIVO + 10];
    if (opcion == 1 || opcion == 5) { sprintf(nombre, "%s.txt", base); guardarTXT(g, nombre); }
    if (opcion == 2 || opcion == 5) { sprintf(nombre, "%s.csv", base); guardarCSV(g, nombre); strcpy(g->archivoActual, nombre); }
    if (opcion == 3 || opcion == 5) { sprintf(nombre, "%s.json", base); guardarJSON(g, nombre); }
    if (opcion == 4 || opcion == 5) { sprintf(nombre, "%s.xml", base); guardarXML(g, nombre); }
}

/* Carga los riesgos desde el archivo CSV activo usando memoria dinamica */
void cargarDesdeDisco(GestorRiesgos *g) {
    FILE *f = fopen(g->archivoActual, "r");
    if (f == NULL) {
        printf("\nNo se pudo abrir '%s'. Verifique que exista (opcion 6 o 7).\n", g->archivoActual);
        return;
    }

    char linea[1024];
    /* Descarta la linea de encabezado */
    if (fgets(linea, sizeof(linea), f) == NULL) { fclose(f); printf("Archivo vacio.\n"); return; }

    /* Reinicia el gestor con memoria dinamica limpia */
    liberarMemoria(g);
    inicializarGestor(g);

    int cargados = 0;
    while (fgets(linea, sizeof(linea), f) != NULL) {
        asegurarCapacidad(g);
        Riesgo *r = &g->riesgos[g->cantidad];

        char *token;
        token = strtok(linea, ",");  if (token) r->id = atoi(token);
        token = strtok(NULL, ",");   if (token) strncpy(r->bien, token, MAX_NOMBRE);
        token = strtok(NULL, ",");   if (token) strncpy(r->riesgo, token, MAX_STR);
        token = strtok(NULL, ",");   if (token) strncpy(r->dano, token, MAX_STR);
        token = strtok(NULL, ",");   if (token) r->F = atoi(token);
        token = strtok(NULL, ",");   if (token) r->S = atoi(token);
        token = strtok(NULL, ",");   if (token) r->P = atoi(token);
        token = strtok(NULL, ",");   if (token) r->E = atoi(token);
        token = strtok(NULL, ",");   if (token) r->A = atoi(token);
        token = strtok(NULL, ",");   if (token) r->V = atoi(token);
        /* I, D, C, Pb, ER, clase se recalculan para garantizar consistencia */
        calcularRiesgo(r);

        g->cantidad++;
        cargados++;
    }
    fclose(f);
    printf("\nSe cargaron %d riesgo(s) desde '%s' usando memoria dinamica.\n",
           cargados, g->archivoActual);
}

/* ---------------------------------- Menu ------------------------------------ */

void mostrarMenu(const GestorRiesgos *g) {
    printf("\n================= METODO MOSLER - GESTION DE RIESGOS =================\n");
    printf(" Archivo activo: %s | Riesgos en memoria: %d\n", g->archivoActual, g->cantidad);
    printf("------------------------------------------------------------------------\n");
    printf(" 1. Crear riesgo\n");
    printf(" 2. Modificar parametros del riesgo\n");
    printf(" 3. Calcular riesgos\n");
    printf(" 4. Imprimir los riesgos\n");
    printf(" 5. Borrar riesgo\n");
    printf(" 6. Seleccionar un archivo especifico\n");
    printf(" 7. Crear un archivo especifico\n");
    printf(" 8. Guardar en archivo (txt/csv/json/xml)\n");
    printf(" 9. Cargar riesgos desde disco (memoria dinamica)\n");
    printf(" 10. Analisis con IA (recomendacion por riesgo)\n");
    printf(" 0. Salir\n");
    printf("========================================================================\n");
}

int main(void) {
    GestorRiesgos gestor;
    inicializarGestor(&gestor);

    int opcion;
    do {
        mostrarMenu(&gestor);
        opcion = leerEntero("Seleccione una opcion: ", 0, 10);

        switch (opcion) {
            case 1: crearRiesgo(&gestor); break;
            case 2: modificarRiesgo(&gestor); break;
            case 3: calcularTodosLosRiesgos(&gestor); break;
            case 4: imprimirRiesgos(&gestor); break;
            case 5: borrarRiesgo(&gestor); break;
            case 6: seleccionarArchivo(&gestor); break;
            case 7: crearArchivo(&gestor); break;
            case 8: guardarArchivo(&gestor); break;
            case 9: cargarDesdeDisco(&gestor); break;
            case 10: {
                if (gestor.cantidad == 0) { printf("\nNo hay riesgos registrados.\n"); break; }
                int id = leerEntero("ID del riesgo a analizar: ", 1, 999999);
                Riesgo *r = buscarPorId(&gestor, id);
                if (r == NULL) printf("No se encontro un riesgo con ese ID.\n");
                else moduloIA(r);
                break;
            }
            case 0: printf("\nGuarde sus cambios antes de salir si aun no lo ha hecho.\n"
                            "Saliendo del programa...\n"); break;
        }
    } while (opcion != 0);

    liberarMemoria(&gestor);
    return 0;
}
