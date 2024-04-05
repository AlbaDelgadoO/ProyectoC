#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sqlite3.h"
#include "database.h"
#include "menu.h"
#include "prestamos.h" 
#include <string.h>

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

// GESTION DE USUARIOS

void agregarNuevoUsuario(sqlite3* db) {
// Solicitar detalles del nuevo usuario
    printf("Ingrese el ID del usuario: ");
    char id[50];
    scanf("%s", id);

    printf("Ingrese el nombre del usuario: ");
    char nombre[50];
    scanf("%s", nombre);
    printf("\n");

    printf("Ingrese el apellido del usuario: ");
    char apellido[50];
    scanf("%s", apellido);
    printf("\n");

    printf("Ingrese el correo del usuario: ");
    char correo[100];
    scanf("%s", correo);
    printf("\n");

    printf("Ingrese la contraseña del usuario: ");
    char contr[50];
    scanf("%s", contr);
    printf("\n");

    // Crear una instancia de Usuario
    Usuario nuevoUsuario;

    // Asignar valores al usuario
    strcpy(nuevoUsuario.ID_Usuario, id);
    strcpy(nuevoUsuario.nombreU, nombre);
    strcpy(nuevoUsuario.apellidoU, apellido);
    strcpy(nuevoUsuario.correo, correo);
    strcpy(nuevoUsuario.contrasenya, contr);


    insertarUsuario(db, nuevoUsuario);
    printf("El nuevo usuario ha sido agregado correctamente.\n");
}

void buscarUsuario(sqlite3* db) {
    printf("Ingrese el ID o nombre del usuario: ");
    char termino[50];
    scanf("%s", termino);

    buscarUsuariosDB(db, termino);
}

void editarUsuario(sqlite3* db){
    printf("Introduce el ID del usuario que deseas editar: ");
    char id[50];
    scanf("%s", id);

    editarUsuarioDB(db, id);
}


void mostrarMenuUsuarios() {
    printf("=== Menu de Gestion de Usuarios ===\n");
    printf("1. Agregar Nuevo Usuario\n");
    printf("2. Mostrar Todos los Usuarios\n");
    printf("3. Buscar Usuario\n");
    printf("4. Editar Usuario\n");
    printf("5. Borrar Usuario\n");
    printf("6. Volver al Menu Principal\n");
    printf("Seleccione una opcion: ");
}

void ejecutarMenuUsuarios(sqlite3* db) {
    int opcion;
    do {
        mostrarMenuUsuarios();
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                agregarNuevoUsuario(db);
                break;
            case 2:
                leerUsuarios(db);
                break;
            case 3:
                buscarUsuario(db);
                break;
            case 4:
                editarUsuario(db);
                break;
            case 5:
                // Lógica para eliminar un usuario
                break;
            case 6:
                printf("Volviendo al Menu Principal...\n");
                break;
            default:
                printf("Opcion no valida. Por favor, seleccione una opcion valida.\n");
                break;
        }
    } while (opcion != 6);
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
                ejecutarMenuUsuarios(db);
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