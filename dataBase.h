#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"

typedef struct {
    char isbn[20];
    char titulo[100];
    char autor[100];
} Libro;

sqlite3* inicializarBaseDatos(const char* nombreArchivo);
void crearTablaLibro(sqlite3* db);
void insertarLibro(sqlite3* db, Libro libro);
void leerLibros(sqlite3* db);
void buscarLibroBD(sqlite3* db, const char* termino);

#endif /* DATABASE_H */