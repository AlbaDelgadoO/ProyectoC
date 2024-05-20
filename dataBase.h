#ifndef DATABASE_H
#define DATABASE_H
#include "EstructuraDatos.h"
#include "sqlite3.h"
#include <stdbool.h>

sqlite3* inicializarBaseDatos(const char* nombreArchivo);
void crearTablaLibro(sqlite3* db);
void insertarLibro(sqlite3* db, Libro libro);
void comprobarAutor(sqlite3* db, Libro libro);
void leerLibros(sqlite3* db);
char* obtenerDatosLibros(sqlite3* db);
void buscarLibroBD(sqlite3* db, const char* termino);
char* obtenerLibro(sqlite3* db, const char* termino);
void borrarLibroConISBN(sqlite3* db, const char* isbn);


void crearTablaUsuario(sqlite3* db);
void insertarUsuario(sqlite3* db, Usuario usuario);
int insertarUsuarioServer(sqlite3* db, Usuario usuario);
void leerUsuarios(sqlite3* db);
char* obtenerUsuarios(sqlite3* db);
void buscarUsuariosDB(sqlite3* db, char* termino);
char* buscarUsuariosServer(sqlite3* db, char* termino);
int editarUsuarioServidor(sqlite3* db, const char* id, const char* nombre, const char* apellido, const char* correo, const char* contrasenya);
int borrarUsuarioServidor(sqlite3* db, const char* id);

void modificarContra(sqlite3* db, char* contraNueva, char* id);
void modificarCorreo(sqlite3* db, char* correoNuevo, char* id);
void modificarApellido(sqlite3* db, char* apellidoNuevo, char* id);
void modificarNombre(sqlite3* db, char* nombreNuevo, char* id);
void modificarID(sqlite3* db, char* idNueva, char* idAntigua);
void mostrarMenuEdicion();
void ejecutarMenuEdicion(sqlite3* db, char* id);
void editarUsuarioDB(sqlite3* db, char* id);
void borrarUsuarioDB(sqlite3 *db, char* id);
void crearTablaPrestamo(sqlite3* db);
bool existeUsuario(sqlite3* db, const char* idUsuario);
void insertarPrestamo(sqlite3* db, Prestamo p);
void renovarPrestamoDB(sqlite3* db, char* idLibro);
bool registrarDevolucionLibro(sqlite3* db, char* idLibro);
void obtenerPrestamosPendientes(sqlite3* db, const char* idUsuario);
const char* obtenerResultadoBuffer();

//AUTOR
void insertarAutor(sqlite3*db, Libro libro);
void leerAutores(sqlite3* db);
void eliminarAutores(sqlite3* db, const char* nombreAutor);
void buscarAutor(sqlite3* db, const char* termino);
void crearTablaAutor(sqlite3* db);

void borrarUsuarioTabla(sqlite3* db);
void borrarAutor(sqlite3* db);
void borrarPrestamo(sqlite3* db);
void borrarLibro(sqlite3* db);

#endif /* DATABASE_H */