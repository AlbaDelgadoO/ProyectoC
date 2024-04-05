#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "dataBase.h"

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
// Preparar la consulta SQL para insertar un nuevo usuario
    char* sql = "INSERT INTO Usuario (ID_Usuario, Nombre, Apellido, Correo, Contrasenya) VALUES (?, ?, ?, ?, ?)";

    // Crear una declaración SQL
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Bindear los parámetros a la declaración SQL
    sqlite3_bind_text(stmt, 1, usuario.ID_Usuario, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, usuario.nombreU, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, usuario.apellidoU, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, usuario.correo, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, usuario.contrasenya, -1, SQLITE_STATIC);



    // Ejecutar la consulta
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return;
    }

    // Finalizar la declaración
    sqlite3_finalize(stmt);
}
