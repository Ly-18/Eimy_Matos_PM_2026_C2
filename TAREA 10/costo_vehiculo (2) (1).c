/* ============================================================
   COSTO REAL DE VEHICULOS - Republica Dominicana
   ------------------------------------------------------------
   Calcula el costo real por km de un vehiculo, combinando:
     - Amortizacion (costo de compra repartido en su vida util)
     - Mantenimiento
     - Seguro
     - Neumaticos
     - Combustible (diferenciado entre ciudad y autopista)

   Todos los montos en RD$ (pesos dominicanos).
   El combustible se maneja en GALONES.

   Los vehiculos se guardan en un archivo binario (vehiculos.dat)
   como una lista de structs. Cada vehiculo tiene un ID unico.
   "Eliminar" marca el registro como inactivo (borrado logico),
   asi el archivo nunca queda con huecos ni se corrompen los
   demas registros.

   NOTA IMPORTANTE: toda la entrada numerica pasa por
   leerEntero()/leerLong()/leerDouble(), que validan lo que el
   usuario escribe. Si mete texto donde se espera un numero, el
   programa avisa y vuelve a preguntar en vez de quedarse
   pegado o guardar datos corruptos.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARCHIVO "vehiculos.dat"
#define MAX_NOMBRE 100

typedef struct {
    int id;
    char nombre[MAX_NOMBRE];       // ej: "Toyota Corolla 2020"
    double costoAdquisicion;       // RD$ - precio de compra
    int vidaUtilAnios;             // anios de vida util estimados
    long vidaUtilKm;               // km totales de vida util estimados
    double kmAnualCiudad;          // km/anio que se manejan en ciudad
    double kmAnualAutopista;       // km/anio que se manejan en autopista
    double rendCiudad;             // km por galon en ciudad
    double rendAutopista;          // km por galon en autopista
    double seguroAnual;            // RD$ por anio
    double mantenimientoAnual;     // RD$ por anio (promedio)
    double costoJuegoNeumaticos;   // RD$ por juego de neumaticos
    long vidaUtilNeumaticosKm;     // km que dura un juego de neumaticos
    double precioCombustible;      // RD$ por galon (se puede actualizar)
    int activo;                    // 1 = activo, 0 = eliminado
} Vehiculo;

/* -------------------- PROTOTIPOS -------------------- */
int generarNuevoId(void);
void crearVehiculo(void);
void listarVehiculos(void);
int buscarVehiculo(int id, Vehiculo *resultado, long *posicion);
void eliminarVehiculo(void);
void modificarVehiculo(void);
void modificarPrecioCombustible(void);
void mostrarCostosDetallados(Vehiculo v);
void calcularCostosVehiculo(void);
void calcularViaje(void);
void limpiarBufer(void);
void leerTexto(const char *prompt, char *destino, int tam);
int leerEntero(const char *prompt);
long leerLong(const char *prompt);
double leerDouble(const char *prompt);

/* -------------------- ENTRADA SEGURA -------------------- */

/* Descarta lo que quede pendiente en el buffer de entrada hasta
   el proximo salto de linea (o EOF). */
void limpiarBufer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* Lee una linea de texto de forma segura (para nombres). */
void leerTexto(const char *prompt, char *destino, int tam) {
    printf("%s", prompt);
    if (fgets(destino, tam, stdin) != NULL) {
        destino[strcspn(destino, "\n")] = 0;
    } else {
        destino[0] = 0;
    }
}

/* Lee un entero validado. Si el usuario mete algo que no es un
   numero, se avisa y se vuelve a pedir - nunca se queda en un
   bucle silencioso ni avanza con basura. */
int leerEntero(const char *prompt) {
    int valor;
    char sobrante;
    while (1) {
        printf("%s", prompt);
        int leidos = scanf("%d%c", &valor, &sobrante);
        if (leidos == 2 && sobrante == '\n') {
            return valor;
        }
        if (leidos == EOF) {
            printf("\nSe termino la entrada de datos. Cerrando el programa.\n");
            exit(0);
        }
        printf("Entrada invalida, debe ser un numero entero. Intente de nuevo.\n");
        limpiarBufer();
    }
}

/* Igual que leerEntero pero para valores tipo long (km, etc.) */
long leerLong(const char *prompt) {
    long valor;
    char sobrante;
    while (1) {
        printf("%s", prompt);
        int leidos = scanf("%ld%c", &valor, &sobrante);
        if (leidos == 2 && sobrante == '\n') {
            return valor;
        }
        if (leidos == EOF) {
            printf("\nSe termino la entrada de datos. Cerrando el programa.\n");
            exit(0);
        }
        printf("Entrada invalida, debe ser un numero entero. Intente de nuevo.\n");
        limpiarBufer();
    }
}

/* Igual que leerEntero pero para valores decimales (RD$, km, etc.) */
double leerDouble(const char *prompt) {
    double valor;
    char sobrante;
    while (1) {
        printf("%s", prompt);
        int leidos = scanf("%lf%c", &valor, &sobrante);
        if (leidos == 2 && sobrante == '\n') {
            return valor;
        }
        if (leidos == EOF) {
            printf("\nSe termino la entrada de datos. Cerrando el programa.\n");
            exit(0);
        }
        printf("Entrada invalida, debe ser un numero (puede usar decimales). Intente de nuevo.\n");
        limpiarBufer();
    }
}

/* -------------------- ID AUTOMATICO -------------------- */
int generarNuevoId(void) {
    FILE *f = fopen(ARCHIVO, "rb");
    int maxId = 0;
    if (f) {
        Vehiculo v;
        while (fread(&v, sizeof(Vehiculo), 1, f) == 1) {
            if (v.id > maxId) maxId = v.id;
        }
        fclose(f);
    }
    return maxId + 1;
}

/* -------------------- 1. CREAR VEHICULO -------------------- */
void crearVehiculo(void) {
    Vehiculo v;
    memset(&v, 0, sizeof(v));
    v.id = generarNuevoId();

    printf("\n=== CREAR NUEVO VEHICULO ===\n");
    leerTexto("Nombre/Modelo del vehiculo: ", v.nombre, MAX_NOMBRE);

    v.costoAdquisicion      = leerDouble("Costo de adquisicion (RD$): ");
    v.vidaUtilAnios         = leerEntero("Vida util (anios): ");
    v.vidaUtilKm            = leerLong("Vida util estimada (km totales): ");
    v.kmAnualCiudad         = leerDouble("Km promedio al anio EN CIUDAD: ");
    v.kmAnualAutopista      = leerDouble("Km promedio al anio EN AUTOPISTA: ");
    v.rendCiudad            = leerDouble("Rendimiento en ciudad (km por galon): ");
    v.rendAutopista         = leerDouble("Rendimiento en autopista (km por galon): ");
    v.seguroAnual           = leerDouble("Seguro ANUAL (RD$): ");
    v.mantenimientoAnual    = leerDouble("Mantenimiento ANUAL promedio (RD$): ");
    v.costoJuegoNeumaticos  = leerDouble("Costo de un juego de neumaticos (RD$): ");
    v.vidaUtilNeumaticosKm  = leerLong("Duracion estimada de un juego de neumaticos (km): ");
    v.precioCombustible     = leerDouble("Precio actual del combustible (RD$ por galon): ");
    v.activo = 1;

    FILE *f = fopen(ARCHIVO, "ab");
    if (!f) { printf("Error al abrir el archivo.\n"); return; }
    fwrite(&v, sizeof(Vehiculo), 1, f);
    fclose(f);

    printf("\n[OK] Vehiculo guardado con ID %d\n", v.id);
}

/* -------------------- LISTAR -------------------- */
void listarVehiculos(void) {
    FILE *f = fopen(ARCHIVO, "rb");
    if (!f) { printf("No hay vehiculos registrados todavia.\n"); return; }

    Vehiculo v;
    int encontrados = 0;
    printf("\n%-4s %-30s %-15s\n", "ID", "Nombre", "Costo (RD$)");
    printf("-------------------------------------------------------\n");
    while (fread(&v, sizeof(Vehiculo), 1, f) == 1) {
        if (v.activo) {
            printf("%-4d %-30s %-15.2f\n", v.id, v.nombre, v.costoAdquisicion);
            encontrados++;
        }
    }
    fclose(f);
    if (!encontrados) printf("No hay vehiculos activos.\n");
}

/* Busca un vehiculo activo por ID. Si lo encuentra, llena
   'resultado' y guarda su posicion en bytes dentro del archivo
   (para poder reescribirlo luego con fseek). Retorna 1 si lo
   encontro, 0 si no. */
int buscarVehiculo(int id, Vehiculo *resultado, long *posicion) {
    FILE *f = fopen(ARCHIVO, "rb");
    if (!f) return 0;

    Vehiculo v;
    long pos = 0;
    while (fread(&v, sizeof(Vehiculo), 1, f) == 1) {
        if (v.id == id && v.activo) {
            *resultado = v;
            *posicion = pos;
            fclose(f);
            return 1;
        }
        pos += sizeof(Vehiculo);
    }
    fclose(f);
    return 0;
}

/* -------------------- 2. BORRAR VEHICULO -------------------- */
void eliminarVehiculo(void) {
    int id = leerEntero("ID del vehiculo a eliminar: ");
    Vehiculo v;
    long pos;

    if (!buscarVehiculo(id, &v, &pos)) {
        printf("No se encontro un vehiculo activo con ese ID.\n");
        return;
    }

    v.activo = 0;
    FILE *f = fopen(ARCHIVO, "rb+");
    fseek(f, pos, SEEK_SET);
    fwrite(&v, sizeof(Vehiculo), 1, f);
    fclose(f);

    printf("[OK] Vehiculo '%s' eliminado.\n", v.nombre);
}

/* -------------------- 3. MODIFICAR VEHICULO -------------------- */
void modificarVehiculo(void) {
    int id = leerEntero("ID del vehiculo a modificar: ");
    Vehiculo v;
    long pos;

    if (!buscarVehiculo(id, &v, &pos)) {
        printf("No se encontro un vehiculo activo con ese ID.\n");
        return;
    }

    int opcion;
    do {
        printf("\n--- Modificando: %s ---\n", v.nombre);
        printf(" 1. Nombre                         (%s)\n", v.nombre);
        printf(" 2. Costo de adquisicion            (RD$%.2f)\n", v.costoAdquisicion);
        printf(" 3. Vida util en anios               (%d)\n", v.vidaUtilAnios);
        printf(" 4. Vida util en km                 (%ld)\n", v.vidaUtilKm);
        printf(" 5. Km anuales ciudad               (%.2f)\n", v.kmAnualCiudad);
        printf(" 6. Km anuales autopista            (%.2f)\n", v.kmAnualAutopista);
        printf(" 7. Rendimiento ciudad (km/gal)     (%.2f)\n", v.rendCiudad);
        printf(" 8. Rendimiento autopista (km/gal)  (%.2f)\n", v.rendAutopista);
        printf(" 9. Seguro anual                    (RD$%.2f)\n", v.seguroAnual);
        printf("10. Mantenimiento anual             (RD$%.2f)\n", v.mantenimientoAnual);
        printf("11. Costo juego de neumaticos       (RD$%.2f)\n", v.costoJuegoNeumaticos);
        printf("12. Vida util neumaticos (km)       (%ld)\n", v.vidaUtilNeumaticosKm);
        printf(" 0. Terminar y guardar\n");
        opcion = leerEntero("Seleccione el campo a modificar: ");

        switch (opcion) {
            case 1: leerTexto("Nuevo nombre: ", v.nombre, MAX_NOMBRE); break;
            case 2: v.costoAdquisicion = leerDouble("Nuevo costo: "); break;
            case 3: v.vidaUtilAnios = leerEntero("Nueva vida util (anios): "); break;
            case 4: v.vidaUtilKm = leerLong("Nueva vida util (km): "); break;
            case 5: v.kmAnualCiudad = leerDouble("Nuevos km ciudad: "); break;
            case 6: v.kmAnualAutopista = leerDouble("Nuevos km autopista: "); break;
            case 7: v.rendCiudad = leerDouble("Nuevo rendimiento ciudad: "); break;
            case 8: v.rendAutopista = leerDouble("Nuevo rendimiento autopista: "); break;
            case 9: v.seguroAnual = leerDouble("Nuevo seguro anual: "); break;
            case 10: v.mantenimientoAnual = leerDouble("Nuevo mantenimiento anual: "); break;
            case 11: v.costoJuegoNeumaticos = leerDouble("Nuevo costo juego neumaticos: "); break;
            case 12: v.vidaUtilNeumaticosKm = leerLong("Nueva vida util neumaticos (km): "); break;
            case 0: break;
            default: printf("Opcion invalida.\n");
        }
    } while (opcion != 0);

    FILE *f = fopen(ARCHIVO, "rb+");
    fseek(f, pos, SEEK_SET);
    fwrite(&v, sizeof(Vehiculo), 1, f);
    fclose(f);

    printf("[OK] Vehiculo actualizado.\n");
}

/* -------------------- 4. PRECIO DE COMBUSTIBLE -------------------- */
void modificarPrecioCombustible(void) {
    int id = leerEntero("ID del vehiculo: ");
    Vehiculo v;
    long pos;

    if (!buscarVehiculo(id, &v, &pos)) {
        printf("No se encontro un vehiculo activo con ese ID.\n");
        return;
    }

    printf("Precio actual: RD$%.2f por galon\n", v.precioCombustible);
    v.precioCombustible = leerDouble("Nuevo precio por galon (RD$): ");

    FILE *f = fopen(ARCHIVO, "rb+");
    fseek(f, pos, SEEK_SET);
    fwrite(&v, sizeof(Vehiculo), 1, f);
    fclose(f);

    printf("[OK] Precio de combustible actualizado.\n");
}

/* -------------------- CALCULO DE COSTOS -------------------- */
void mostrarCostosDetallados(Vehiculo v) {
    double kmAnualTotal = v.kmAnualCiudad + v.kmAnualAutopista;
    if (kmAnualTotal <= 0 || v.vidaUtilKm <= 0 || v.vidaUtilNeumaticosKm <= 0
        || v.rendCiudad <= 0 || v.rendAutopista <= 0) {
        printf("Este vehiculo tiene datos incompletos o en cero. Revisa sus parametros.\n");
        return;
    }

    double amortizacionKm   = v.costoAdquisicion / v.vidaUtilKm;
    double mantenimientoKm  = v.mantenimientoAnual / kmAnualTotal;
    double seguroKm         = v.seguroAnual / kmAnualTotal;
    double neumaticosKm     = v.costoJuegoNeumaticos / v.vidaUtilNeumaticosKm;
    double fijoKm           = amortizacionKm + mantenimientoKm + seguroKm + neumaticosKm;

    double combustibleCiudadKm    = v.precioCombustible / v.rendCiudad;
    double combustibleAutopistaKm = v.precioCombustible / v.rendAutopista;

    double totalCiudadKm    = fijoKm + combustibleCiudadKm;
    double totalAutopistaKm = fijoKm + combustibleAutopistaKm;

    printf("\n=== COSTOS DE '%s' ===\n", v.nombre);
    printf("--- Costos fijos por km ---\n");
    printf("Amortizacion:   RD$%.2f/km\n", amortizacionKm);
    printf("Mantenimiento:  RD$%.2f/km\n", mantenimientoKm);
    printf("Seguro:         RD$%.2f/km\n", seguroKm);
    printf("Neumaticos:     RD$%.2f/km\n", neumaticosKm);
    printf("Subtotal fijo:  RD$%.2f/km\n", fijoKm);
    printf("--- Combustible (RD$%.2f/galon) ---\n", v.precioCombustible);
    printf("Ciudad:         RD$%.2f/km\n", combustibleCiudadKm);
    printf("Autopista:      RD$%.2f/km\n", combustibleAutopistaKm);
    printf("--- COSTO TOTAL REAL POR KM ---\n");
    printf("Ciudad:         RD$%.2f/km\n", totalCiudadKm);
    printf("Autopista:      RD$%.2f/km\n", totalAutopistaKm);
}

/* -------------------- 5. COSTOS DETALLADOS DE UN VEHICULO -------------------- */
void calcularCostosVehiculo(void) {
    int id = leerEntero("ID del vehiculo: ");
    Vehiculo v;
    long pos;

    if (!buscarVehiculo(id, &v, &pos)) {
        printf("No se encontro un vehiculo activo con ese ID.\n");
        return;
    }
    mostrarCostosDetallados(v);
}

/* -------------------- 6. CALCULAR UN VIAJE -------------------- */
void calcularViaje(void) {
    int id = leerEntero("ID del vehiculo: ");
    Vehiculo v;
    long pos;

    if (!buscarVehiculo(id, &v, &pos)) {
        printf("No se encontro un vehiculo activo con ese ID.\n");
        return;
    }

    double kmAnualTotal = v.kmAnualCiudad + v.kmAnualAutopista;
    if (kmAnualTotal <= 0 || v.vidaUtilKm <= 0 || v.vidaUtilNeumaticosKm <= 0
        || v.rendCiudad <= 0 || v.rendAutopista <= 0) {
        printf("Este vehiculo tiene datos incompletos o en cero. Revisa sus parametros.\n");
        return;
    }

    double kmCiudad    = leerDouble("Km del viaje EN CIUDAD: ");
    double kmAutopista = leerDouble("Km del viaje EN AUTOPISTA: ");

    double amortizacionKm  = v.costoAdquisicion / v.vidaUtilKm;
    double mantenimientoKm = v.mantenimientoAnual / kmAnualTotal;
    double seguroKm        = v.seguroAnual / kmAnualTotal;
    double neumaticosKm    = v.costoJuegoNeumaticos / v.vidaUtilNeumaticosKm;

    double combustibleCiudadKm    = v.precioCombustible / v.rendCiudad;
    double combustibleAutopistaKm = v.precioCombustible / v.rendAutopista;

    double galonesCiudad    = kmCiudad / v.rendCiudad;
    double galonesAutopista = kmAutopista / v.rendAutopista;

    /* Costo del viaje desglosado componente por componente, no solo
       el total prorrateado. Cada rubro fijo se prorratea por los km
       del viaje usando su propia tarifa por km (amortizacionKm,
       mantenimientoKm, seguroKm, neumaticosKm), y el combustible se
       separa entre ciudad y autopista porque el rendimiento (y por
       lo tanto el gasto) es distinto en cada uno. */
    double costoAmortizacionViaje   = amortizacionKm  * (kmCiudad + kmAutopista);
    double costoMantenimientoViaje  = mantenimientoKm * (kmCiudad + kmAutopista);
    double costoSeguroViaje         = seguroKm        * (kmCiudad + kmAutopista);
    double costoNeumaticosViaje     = neumaticosKm    * (kmCiudad + kmAutopista);
    double costoCombustibleCiudad   = kmCiudad    * combustibleCiudadKm;
    double costoCombustibleAutopista= kmAutopista * combustibleAutopistaKm;

    double costoCombustible = costoCombustibleCiudad + costoCombustibleAutopista;
    double costoFijo        = costoAmortizacionViaje + costoMantenimientoViaje
                             + costoSeguroViaje + costoNeumaticosViaje;
    double costoTotal       = costoCombustible + costoFijo;
    double kmTotal          = kmCiudad + kmAutopista;

    /* Evita division por cero si por alguna razon el costo total da 0 */
    double pctAmortizacion  = costoTotal > 0 ? costoAmortizacionViaje  / costoTotal * 100.0 : 0.0;
    double pctMantenimiento = costoTotal > 0 ? costoMantenimientoViaje / costoTotal * 100.0 : 0.0;
    double pctSeguro        = costoTotal > 0 ? costoSeguroViaje        / costoTotal * 100.0 : 0.0;
    double pctNeumaticos    = costoTotal > 0 ? costoNeumaticosViaje    / costoTotal * 100.0 : 0.0;
    double pctCombustible   = costoTotal > 0 ? costoCombustible        / costoTotal * 100.0 : 0.0;

    printf("\n=== RESUMEN DEL VIAJE - %s ===\n", v.nombre);
    printf("Distancia ciudad:      %.2f km\n", kmCiudad);
    printf("Distancia autopista:   %.2f km\n", kmAutopista);
    printf("Distancia total:       %.2f km\n", kmTotal);
    printf("-------------------------------------\n");
    printf("Costos fijos:\n");
    printf("  Amortizacion    RD$%9.2f  (%4.1f%%)\n", costoAmortizacionViaje, pctAmortizacion);
    printf("  Mantenimiento   RD$%9.2f  (%4.1f%%)\n", costoMantenimientoViaje, pctMantenimiento);
    printf("  Seguro          RD$%9.2f  (%4.1f%%)\n", costoSeguroViaje, pctSeguro);
    printf("  Neumaticos      RD$%9.2f  (%4.1f%%)\n", costoNeumaticosViaje, pctNeumaticos);
    printf("  Subtotal        RD$%9.2f\n", costoFijo);
    printf("-------------------------------------\n");
    printf("Combustible (RD$%.2f/galon):\n", v.precioCombustible);
    printf("  Ciudad     %5.2f gal   RD$%9.2f\n", galonesCiudad, costoCombustibleCiudad);
    printf("  Autopista  %5.2f gal   RD$%9.2f\n", galonesAutopista, costoCombustibleAutopista);
    printf("  Subtotal   %5.2f gal   RD$%9.2f  (%4.1f%%)\n",
           galonesCiudad + galonesAutopista, costoCombustible, pctCombustible);
    printf("-------------------------------------\n");
    printf("COSTO TOTAL DEL VIAJE: RD$%.2f\n", costoTotal);
    if (kmTotal > 0) printf("Costo promedio por km: RD$%.2f\n", costoTotal / kmTotal);
}

/* -------------------- MENU PRINCIPAL -------------------- */
int main(void) {
    int opcion;
    do {
        printf("\n===========================================\n");
        printf("   COSTO REAL DE VEHICULOS (RD)\n");
        printf("===========================================\n");
        printf("1. Crear vehiculo\n");
        printf("2. Listar vehiculos\n");
        printf("3. Modificar vehiculo\n");
        printf("4. Eliminar vehiculo\n");
        printf("5. Modificar precio de combustible\n");
        printf("6. Ver costo real por km (ciudad/autopista)\n");
        printf("7. Calcular costo de un viaje\n");
        printf("0. Salir\n");
        opcion = leerEntero("Seleccione una opcion: ");

        switch (opcion) {
            case 1: crearVehiculo(); break;
            case 2: listarVehiculos(); break;
            case 3: modificarVehiculo(); break;
            case 4: eliminarVehiculo(); break;
            case 5: modificarPrecioCombustible(); break;
            case 6: calcularCostosVehiculo(); break;
            case 7: calcularViaje(); break;
            case 0: printf("Hasta luego!\n"); break;
            default: printf("Opcion invalida.\n");
        }
    } while (opcion != 0);

    return 0;
}
