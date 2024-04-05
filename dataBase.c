#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "dataBase.h"
#include <string.h>

// Implementación de las funciones para interactuar con la base de datos
sqlite3* inicializarBaseDatos(const char* nombreArchivo) {
    sqlite3* db;
    int resultado = sqlite3_open(nombreArchivo, &db);
    if (resultado != SQLITE_OK) {
        fprintf(stderr, "Error al abrir o crear la base de datos: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    return db;
}

void crearTablaLibro(sqlite3* db){
    char* sql = "CREATE TABLE IF NOT EXISTS Libro (ISBN TEXT, Titulo TEXT, Autor TEXT)";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return;
    }
    
    sqlite3_finalize(stmt);
}

void insertarLibro(sqlite3* db, Libro libro) {
    // Preparar la consulta SQL para insertar un nuevo libro
    char* sql = "INSERT INTO Libro (ISBN, Titulo, Autor) VALUES (?, ?, ?)";

    // Crear una declaración SQL
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Bindear los parámetros a la declaración SQL
    sqlite3_bind_text(stmt, 1, libro.isbn, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, libro.titulo, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, libro.autor, -1, SQLITE_STATIC);

    // Ejecutar la consulta
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return;
    }

    // Finalizar la declaración
    sqlite3_finalize(stmt);
}

void leerLibros(sqlite3* db) {
    char* sql = "SELECT * FROM Libro";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("=== Libros en la Biblioteca ===\n");
    printf("%-15s %-30s %-20s\n", "ISBN", "Título", "Autor");
    printf("==============================================\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* isbn = sqlite3_column_text(stmt, 0);
        const unsigned char* titulo = sqlite3_column_text(stmt, 1);
        const unsigned char* autor = sqlite3_column_text(stmt, 2);
        printf("%-15s %-30s %-20s\n", isbn, titulo, autor);
    }

    sqlite3_finalize(stmt);
}

void buscarLibroBD(sqlite3* db, const char* termino) {
    char* sql = "SELECT * FROM Libro WHERE Titulo LIKE ? OR Autor LIKE ?";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Bindear los parámetros a la consulta
    sqlite3_bind_text(stmt, 1, termino, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, termino, -1, SQLITE_STATIC);

    printf("=== Resultados de la Búsqueda ===\n");
    printf("%-15s %-30s %-20s\n", "ISBN", "Título", "Autor");
    printf("==============================================\n");

    int resultado;
    while ((resultado = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char* isbn = sqlite3_column_text(stmt, 0);
        const unsigned char* titulo = sqlite3_column_text(stmt, 1);
        const unsigned char* autor = sqlite3_column_text(stmt, 2);
        printf("%-15s %-30s %-20s\n", isbn, titulo, autor);
    }

    if (resultado != SQLITE_DONE) {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}

// GESTIÓN DE USUARIOS
void crearTablaUsuario(sqlite3* db) {
    char* sql = "CREATE TABLE IF NOT EXISTS Usuario (ID_Usuario TEXT, Nombre TEXT, Apellido TEXT, Correo TEXT, Contrasenya TEXT)";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return;
    }
    
    sqlite3_finalize(stmt);
}

void insertarUsuario(sqlite3* db, Usuario usuario) {
    char* sql = "INSERT INTO Usuario (ID_Usuario, Nombre, Apellido, Correo, Contrasenya) VALUES (?, ?, ?, ?, ?)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, usuario.ID_Usuario, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, usuario.nombreU, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, usuario.apellidoU, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, usuario.correo, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, usuario.contrasenya, -1, SQLITE_STATIC);


    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);
}

void leerUsuarios(sqlite3* db) {
    char* sql = "SELECT * FROM Usuario";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }
    printf("\n");
    printf("=== Usuarios en la Biblioteca ===\n");
    printf("%-10s %-10s %-10s %-25s %-15s\n", "ID", "Nombre", "Apellido", "Correo", "Contrasenya");
    printf("========================================================================\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* id = sqlite3_column_text(stmt, 0);
        const unsigned char* nombre = sqlite3_column_text(stmt, 1);
        const unsigned char* apellido = sqlite3_column_text(stmt, 2);
        const unsigned char* correo = sqlite3_column_text(stmt, 3);
        const unsigned char* contr = sqlite3_column_text(stmt, 4);

        printf("%-10s %-10s %-10s %-25s %-15s\n", id, nombre, apellido, correo, contr);
    }
    printf("\n");

    sqlite3_finalize(stmt);
}

void buscarUsuariosDB(sqlite3* db, char* termino) {
    char* sql = "SELECT * FROM Usuario WHERE ID_Usuario LIKE ? OR Nombre LIKE ?";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Bindear los parámetros a la consulta
    sqlite3_bind_text(stmt, 1, termino, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, termino, -1, SQLITE_STATIC);
    
    printf("\n");
    printf("=================== Coindidencias encontradas =========================\n");
    printf("%-10s %-10s %-10s %-25s %-15s\n", "ID", "Nombre", "Apellido", "Correo", "Contrasenya");
    printf("=======================================================================\n");

    int resultado;
    while ((resultado = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char* id = sqlite3_column_text(stmt, 0);
        const unsigned char* nombre = sqlite3_column_text(stmt, 1);
        const unsigned char* apellido = sqlite3_column_text(stmt, 2);
        const unsigned char* correo = sqlite3_column_text(stmt, 3);
        const unsigned char* contr = sqlite3_column_text(stmt, 4);

        printf("%-10s %-10s %-10s %-25s %-15s\n", id, nombre, apellido, correo, contr);
    }

    if (resultado != SQLITE_DONE) {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
    }

    printf("\n");

    sqlite3_finalize(stmt);
}


void modificarContra(sqlite3* db, char* contraNueva, char* id) {
    char* sql = "UPDATE Usuario SET Contrasenya = ? WHERE ID_Usuario = ?";
    sqlite3_stmt* stmt;

     if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, contraNueva, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, id, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
    } 
    sqlite3_finalize(stmt);
    printf("Usuario modificado con exito. Pulse 6 para salir.");
}

void modificarCorreo(sqlite3* db, char* correoNuevo, char* id) {
    char* sql = "UPDATE Usuario SET Correo = ? WHERE ID_Usuario = ?";
    sqlite3_stmt* stmt;

     if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, correoNuevo, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, id, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
    } 
    sqlite3_finalize(stmt);
    printf("Usuario modificado con exito. Pulse 6 para salir.");
}

void modificarApellido(sqlite3* db, char* apellidoNuevo, char* id) {
    char* sql = "UPDATE Usuario SET Apellido = ? WHERE ID_Usuario = ?";
    sqlite3_stmt* stmt;

     if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, apellidoNuevo, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, id, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
    } 
    sqlite3_finalize(stmt);
    printf("Usuario modificado con exito. Pulse 6 para salir.");
}


void modificarNombre(sqlite3* db, char* nombreNuevo, char* id) {
    char* sql = "UPDATE Usuario SET Nombre = ? WHERE ID_Usuario = ?";
    sqlite3_stmt* stmt;

     if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, nombreNuevo, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, id, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
    } 
    sqlite3_finalize(stmt);
    printf("Usuario modificado con exito. Pulse 6 para salir.");
}



void modificarID(sqlite3* db, char* idNueva, char* idAntigua) {
    char* sql = "UPDATE Usuario SET ID_Usuario = ? WHERE ID_Usuario = ?";
    sqlite3_stmt* stmt;

     if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Bindear parametros
    sqlite3_bind_text(stmt, 1, idNueva, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, idAntigua, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
    } 

    sqlite3_finalize(stmt);            
    printf("Usuario modificado con exito. Pulse 6 para salir.");

}

void mostrarMenuEdicion() {
    printf("\n");
    printf("=== Selecciona el parametro que deseas modificar ===\n");
    printf("1. ID\n");
    printf("2. Nombre\n");
    printf("3. Apellido\n");
    printf("4. Correo\n");
    printf("5. Contrasenya\n");
    printf("6. Volver\n");
    printf("Seleccion: ");
}


void ejecutarMenuEdicion(sqlite3* db, char* id) {
    mostrarMenuEdicion();
    char idUsuario[50];
    strcpy(idUsuario, (const char *)id);
    int opcion;
    do {
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                printf("Introduce el nuevo id: ");
                char idNuevo[50];
                scanf("%s", idNuevo);                   
                modificarID(db, idNuevo, idUsuario);
                break;
            case 2:
                printf("Introduce el nuevo nombre: ");
                char nombreNuevo[50];
                scanf("%s", nombreNuevo);                   
                modificarNombre(db, nombreNuevo, idUsuario);
                break;
            case 3:
                printf("Introduce el nuevo apellido: ");
                char apellidoNuevo[50];
                scanf("%s", apellidoNuevo);                   
                modificarApellido(db, apellidoNuevo, idUsuario);
                break;
            case 4:
                printf("Introduce el nuevo correo: ");
                char correo[50];
                scanf("%s", correo);                   
                modificarCorreo(db, correo, idUsuario);
                break;
            case 5:
                printf("Introduce la nueva contrasenya: ");
                char contra[50];
                scanf("%s", contra);                   
                modificarContra(db, contra, idUsuario);
                break;
            case 6:
                printf("Volviendo al Menu de Usuarios...\n");
                break;
            default:
                printf("Opcion no valida. Por favor, seleccione una opcion valida.\n");
                break;
        }
    } while (opcion != 6);
}


void editarUsuarioDB(sqlite3* db, char* id) {
    char* sql = "SELECT * FROM Usuario WHERE ID_Usuario LIKE ?";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }
    // Bindear parametros
    sqlite3_bind_text(stmt, 1, id, -1, SQLITE_STATIC);

    // Ejecutar consulta
    int result;
    result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
        // Usuario encontrado, imprimir sus datos
        printf("Usuario encontrado:\n");
        printf("%-10s %-10s %-10s %-25s %-15s\n", "ID", "Nombre", "Apellido", "Correo", "Contrasenya");
        const unsigned char* id = sqlite3_column_text(stmt, 0);
        const unsigned char* nombre = sqlite3_column_text(stmt, 1);
        const unsigned char* apellido = sqlite3_column_text(stmt, 2);
        const unsigned char* correo = sqlite3_column_text(stmt, 3);
        const unsigned char* contr = sqlite3_column_text(stmt, 4);

        printf("%-10s %-10s %-10s %-25s %-15s\n", id, nombre, apellido, correo, contr);

        ejecutarMenuEdicion(db, (char *)id);

    } else if (result == SQLITE_DONE) {
        printf("El usuario con ID '%s' no existe.\n", id);
    } else {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
    }

}






