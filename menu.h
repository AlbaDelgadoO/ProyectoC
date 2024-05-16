#ifndef MENU_H
#define MENU_H
#include <stdbool.h>
#include "sqlite3.h"

void mostrarMenuPrincipal();
void ejecutarMenuPrincipal(sqlite3* db);
void mostrarMenuLibros();
void ejecutarMenuLibros(sqlite3* db);
void buscarLibro(sqlite3* db);

// USUARIOS
void agregarNuevoUsuario(sqlite3* db);
void buscarUsuario(sqlite3* db);
void editarUsuario(sqlite3* db);
void borrarUsuario(sqlite3* db);
void mostrarMenuUsuarios();
void ejecutarMenuUsuarios(sqlite3* db);

// GESTION DE PRESTAMOS
bool usuarioExiste(sqlite3* db, const char* idUsuario);
void agregarNuevoPrestamo(sqlite3* db);
char* calcularNuevaFechaVencimiento(const char* fechaVencimientoActual);
void renovarPrestamo(sqlite3* db);
void registrarDevolucion(sqlite3* db);
void buscarPrestamosPendientes(sqlite3* db);
void mostrarMenuPrestamos();
void ejecutarMenuPrestamos(sqlite3* db);

// CONFIGURACION DEL SISTEMA
void crearFicheroLog();
void leerFicheroConfiguracion();
void mostrarMenuConfiguracionFicheros();
void actualizarParametros(const char *clave, const char *nuevoValor);
void mostrarMenuConfiguracionParametros();
void ejecutarMenuConfiguracionParametros(sqlite3* db);
void mostrarMenuConfiguracion();
void ejecutarMenuConfiguracion(sqlite3* db);


// Generación de informes
void mostrarMenuInformes();
char* generarInformeUsuarios(sqlite3* db);
char* generarInformePrestamos(sqlite3* db);
char* generarInformeLibros(sqlite3* db);

#endif /* MENU_H */


/*

GENERACION DE INFORMES

USUARIOS:

Usuario que no haya realizado ningun prestamo
Usuario con mas prestamo
Numero de usuarios registrado

PRESTAMOS:

Numero de veces que se ha prestado un libro
Numero de prestamos activos
Libro mas prestado
Libro menos prestado

LIBROS:

Numero de libros que no sa han prestado
Numero de libros prestados
Numero de libros devueltos
Numero de libros
 
*/