#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sqlite3.h"
#include "dataBase.h"
#include "menu.h"
#include <string.h>
#include <time.h>

void agregarNuevoLibro(sqlite3* db) {
    // Solicitar detalles del nuevo libro al usuario
    printf("Ingrese el ISBN del libro: ");
    char isbn[15];
    scanf("%14s", isbn);

    printf("Ingrese el titulo del libro: ");
    char titulo[100];
    scanf("%s", titulo);

    printf("Ingrese el autor del libro: ");
    char autor[100];
    scanf("%s", autor);

    printf("Ingrese el genero del libro: ");
    char genero[100];
    scanf("%s", genero);

    printf("Ingrese el numero de ejemplares del libro: ");
    int nEjemplares;
    scanf("%i", &nEjemplares);
    // Asegúrate de manejar correctamente la entrada del usuario y validarla según sea necesario

    // Crear una instancia de Libro con los detalles ingresados por el usuario
    Libro nuevoLibro;
    strcpy(nuevoLibro.isbn, isbn);
    strcpy(nuevoLibro.titulo, titulo);
    strcpy(nuevoLibro.autor, autor);
    strcpy(nuevoLibro.genero, genero);
    nuevoLibro.nEjemplares = nEjemplares;    

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
    buscarLibroBD(db, termino);
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
                leerLibros(db);
                break;
            case 3:
                buscarLibro(db);
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

// GESTION DE PRESTAMOS

bool usuarioExiste(sqlite3* db, const char* idUsuario) {
    // Consulta SQL para buscar el usuario por su ID
    const char* sql = "SELECT COUNT(*) FROM Usuario WHERE ID_Usuario = ?";

    // Preparar la consulta
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return false;
    }

    // Bind the user ID parameter
    sqlite3_bind_text(stmt, 1, idUsuario, -1, SQLITE_STATIC);

    // Ejecutar la consulta
    int result = sqlite3_step(stmt);

    // Verificar si se encontró un usuario
    bool existe = false;
    if (result == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        existe = count > 0;
    } else if (result != SQLITE_DONE) {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
    }

    // Finalizar la consulta
    sqlite3_finalize(stmt);

    return existe;
}

void agregarNuevoPrestamo(sqlite3* db) {
    // Solicitar detalles del nuevo préstamo al usuario
    printf("Ingrese el ID del libro: ");
    char idLibro[15];
    scanf("%14s", idLibro);

    printf("Ingrese el ID del usuario: ");
    char idUsuario[50];
    scanf("%49s", idUsuario);

    // Verificar si el usuario existe en la base de datos
    if (!usuarioExiste(db, idUsuario)) {
        printf("El usuario con el ID '%s' no existe en la base de datos.\n", idUsuario);
        return;
    }

    printf("Ingrese la fecha de vencimiento (YYYY-MM-DD): ");
    char fechaVencimiento[11];
    scanf("%10s", fechaVencimiento);

    // Insertar el nuevo préstamo en la base de datos
    char* sql = "INSERT INTO Prestamo (ID_Libro, ID_Usuario, Fecha_Vencimiento) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, idLibro, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, idUsuario, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, fechaVencimiento, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
    } else {
        printf("El nuevo prestamo ha sido agregado correctamente.\n");
    }

    sqlite3_finalize(stmt);
}

// Función para obtener la nueva fecha de vencimiento extendida por 15 días
char* calcularNuevaFechaVencimiento(const char* fechaVencimientoActual) {
    struct tm *tm_fecha;
    time_t tiempo;
    
    // Analiza la fecha de vencimiento actual desde la cadena
    int year, month, day;
    sscanf(fechaVencimientoActual, "%d-%d-%d", &year, &month, &day);
    
    // Obtiene el tiempo actual
    tiempo = time(NULL);
    tm_fecha = gmtime(&tiempo);
    tm_fecha->tm_year = year - 1900; // Año desde 1900
    tm_fecha->tm_mon = month - 1; // Mes (0-11)
    tm_fecha->tm_mday = day; // Día del mes
    
    // Añade 15 días a la fecha de vencimiento actual
    tiempo = mktime(tm_fecha) + (15 * 24 * 60 * 60); // 15 días en segundos
    
    // Convierte el tiempo en estructura tm
    tm_fecha = gmtime(&tiempo);
    
    // Formatea la nueva fecha de vencimiento en una cadena
    static char nuevaFecha[11];
    snprintf(nuevaFecha, sizeof(nuevaFecha), "%04d-%02d-%02d", tm_fecha->tm_year + 1900, tm_fecha->tm_mon + 1, tm_fecha->tm_mday);

    return nuevaFecha;
}

void renovarPrestamo(sqlite3* db) {
    // Solicitar el ID del libro
    printf("Ingrese el ID del libro cuyo prestamo desea renovar: ");
    int idLibro;
    scanf("%d", &idLibro);

    // Consultar la fecha de vencimiento actual del préstamo asociado al libro
    char selectSql[200];
    sprintf(selectSql, "SELECT Fecha_Vencimiento FROM Prestamo WHERE ID_Libro = %d", idLibro);
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, selectSql, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Si existe un préstamo asociado al libro, actualizar la fecha de vencimiento
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* fechaVencimientoActual = (const char*)sqlite3_column_text(stmt, 0);

        // Calcular la nueva fecha de vencimiento extendida por 15 días
        char* nuevaFechaVencimiento = calcularNuevaFechaVencimiento(fechaVencimientoActual);

        // Actualizar la fecha de vencimiento en la base de datos
        char updateSql[200];
        sprintf(updateSql, "UPDATE Prestamo SET Fecha_Vencimiento = '%s' WHERE ID_Libro = %d", nuevaFechaVencimiento, idLibro);
        int updateResult = sqlite3_exec(db, updateSql, NULL, 0, NULL);
        if (updateResult != SQLITE_OK) {
            printf("Error al actualizar la fecha de vencimiento: %s\n", sqlite3_errmsg(db));
            return;
        }

        printf("El prestamo del libro con ID %d ha sido renovado con exito.\n", idLibro);
    } else {
        printf("No se encontro un prestamo asociado al libro con ID %d.\n", idLibro);
    }

    // Liberar la consulta preparada
    sqlite3_finalize(stmt);
}

void registrarDevolucion(sqlite3* db) {
    // Solicitar al usuario que ingrese el ID del libro devuelto
    int idLibro;
    printf("Ingrese el ID del libro a devolver: ");
    scanf("%d", &idLibro);

    // Consultar la tabla Prestamo para verificar si hay préstamos activos para este libro
    char selectSql[100];
    sprintf(selectSql, "SELECT ID FROM Prestamo WHERE ID_Libro = %d AND Estado = 0", idLibro);
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, selectSql, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Ejecutar la consulta y verificar si hay resultados
    bool prestamosActivos = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        prestamosActivos = true;
        int idPrestamo = sqlite3_column_int(stmt, 0);

        // Actualizar el estado del préstamo a devuelto (Estado = 1)
        char updateSql[100];
        sprintf(updateSql, "UPDATE Prestamo SET Estado = 1 WHERE ID = %d", idPrestamo);
        int updateResult = sqlite3_exec(db, updateSql, NULL, 0, NULL);
        if (updateResult != SQLITE_OK) {
            printf("Error al actualizar el estado del prestamo: %s\n", sqlite3_errmsg(db));
            return;
        }
    }

    // Liberar la consulta preparada
    sqlite3_finalize(stmt);

    // Verificar si se encontraron préstamos activos para este libro
    if (prestamosActivos) {
        printf("El libro ha sido devuelto con exito.\n");
    } else {
        printf("No hay prestamos pendientes para este libro.\n");
    }
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
                //gestionarProblemasPrestamo(db);
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