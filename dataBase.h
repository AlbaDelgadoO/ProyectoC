#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"

typedef struct {
    char isbn[14];
    char titulo[100];
    char autor[100];
    char genero[100];
    int nEjemplares;
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
void leerUsuarios(sqlite3* db);
void buscarUsuariosDB(sqlite3* db, char* termino);

void modificarContra(sqlite3* db, char* contraNueva, char* id);
void modificarCorreo(sqlite3* db, char* correoNuevo, char* id);
void modificarApellido(sqlite3* db, char* apellidoNuevo, char* id);
void modificarNombre(sqlite3* db, char* nombreNuevo, char* id);
void modificarID(sqlite3* db, char* idNueva, char* idAntigua);
void mostrarMenuEdicion();
void ejecutarMenuEdicion(sqlite3* db, char* id);
void editarUsuarioDB(sqlite3* db, char* id);


#endif /* DATABASE_H */