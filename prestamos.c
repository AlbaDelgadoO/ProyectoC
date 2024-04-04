#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sqlite3.h"
#include "dataBase.h"
#include "prestamos.h"
#include "EstructuraDatos.h"

void agregarNuevoPrestamo(sqlite3* db) {
    // Código para agregar un nuevo préstamo
}

void buscarPrestamosPendientes(sqlite3* db) {
    // Código para buscar préstamos pendientes
}

void mostrarMenuPrestamos() {
    printf("=== Menu de Gestion de Prestamos ===\n");
    printf("1. Agregar Nuevo Prestamo\n");
    printf("2. Renovar Prestamo Existente\n");
    printf("3. Registrar Devolucion de Libro\n");
    printf("4. Gestionar Problemas de Prestamo\n");
    printf("5. Volver al Menu Principal\n");
    printf("Seleccione una opcion: ");
}

void ejecutarMenuPrestamos(sqlite3* db) {
    int opcion;
    do {
        mostrarMenuPrestamos();
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                agregarNuevoPrestamo(db);
                break;
            case 2:
                renovarPrestamo(db);
                break;
            case 3:
                registrarDevolucion(db);
                break;
            case 4:
                gestionarProblemasPrestamo(db);
                break;
            case 5:
                printf("Volviendo al Menu Principal...\n");
                break;
            default:
                printf("Opcion no valida. Por favor, seleccione una opcion valida.\n");
                break;
        }
    } while (opcion != 5);
}
