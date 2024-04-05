#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"

typedef struct {
    char isbn[20];
    char titulo[100];
    char autor[100];
} Libro;

typedef struct {
    char ID_Usuario[50];
    char nombreU[50];
    char apellidoU[50];
    char correo[100];
    char contrasenya[50];
} Usuario;

sqlite3* inicializarBaseDatos(const char* nombreArchivo);
void crearTablaLibro(sqlite3* db);
void insertarLibro(sqlite3* db, Libro libro);
void leerLibros(sqlite3* db);
void buscarLibroBD(sqlite3* db, const char* termino);

void crearTablaUsuario(sqlite3* db);
void insertarUsuario(sqlite3* db, Usuario usuario);

#endif /* DATABASE_H */