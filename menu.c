#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sqlite3.h"
#include "database.h"
#include "menu.h"
#include "prestamos.h" 

void agregarNuevoLibro(sqlite3* db) {
    // Solicitar detalles del nuevo libro al usuario
    printf("Ingrese el ISBN del libro: ");
    char isbn[50];
    scanf("%s", isbn);
    // Solicitar otros detalles como título, autor, etc.
    // Asegúrate de manejar correctamente la entrada del usuario y validarla según sea necesario

    // Crear una instancia de Libro con los detalles ingresados por el usuario
    Libro nuevoLibro;
    // Asigna los detalles a la estructura nuevoLibro

    // Insertar el nuevo libro en la base de datos
    insertarLibro(db, nuevoLibro);
    printf("El nuevo libro ha sido agregado correctamente.\n");
}

void buscarLibro(sqlite3* db) {
    char termino[100];
    printf("Ingrese el termino de busqueda (titulo, autor, etc.): ");
    scanf("%s", termino);

    // Realizar la búsqueda en la base de datos
    printf("Buscando libros que coincidan con '%s'...\n", termino);
    // Llamar a la función para buscar libros en la base de datos
    // Mostrar los resultados de la búsqueda
    // Manejar el caso en que no se encuentren resultados
}

void mostrarMenuLibros() {
    printf("=== Menu de Gestion de Libros ===\n");
    printf("1. Agregar Nuevo Libro\n");
    printf("2. Mostrar Todos los Libros\n");
    printf("3. Buscar Libro\n");
    printf("4. Volver al Menu Principal\n");
    printf("Seleccione una opcion: ");
}

void ejecutarMenuLibros(sqlite3* db) {
    int opcion;
    do {
        mostrarMenuLibros();
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                agregarNuevoLibro(db);
                break;
            case 2:
                // Lógica para mostrar todos los libros
                break;
            case 3:
                // Lógica para buscar un libro
                break;
            case 4:
                printf("Volviendo al Menu Principal...\n");
                break;
            default:
                printf("Opcion no valida. Por favor, seleccione una opcion valida.\n");
                break;
        }
    } while (opcion != 4);
}

void mostrarMenuPrincipal() {
    printf("=== Menu Principal ===\n");
    printf("1. Gestion de Libros\n");
    printf("2. Gestion de Usuarios\n");
    printf("3. Gestion de Prestamos\n");
    printf("4. Configuracion del Sistema\n");
    printf("5. Generacion de Informes\n");
    printf("6. Salir\n");
    printf("Seleccione una opcion: ");
}

void ejecutarMenuPrincipal(sqlite3* db) {
    int opcion;
    do {
        mostrarMenuPrincipal();
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                ejecutarMenuLibros(db);
                break;
            case 2:
                // Lógica para ejecutar el menú de gestión de usuarios
                break;
            case 3:
                ejecutarMenuPrestamos(db);
                break;
            case 4:
                // Lógica para ejecutar el menú de configuración del sistema
                break;
            case 5:
                // Lógica para ejecutar el menú de generación de informes
                break;
            case 6:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opcion no valida. Por favor, seleccione una opcion valida.\n");
                break;
        }
    } while (opcion != 6);
}