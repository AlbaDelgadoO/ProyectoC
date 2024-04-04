#ifndef PRESTAMOS_H
#define PRESTAMOS_H

#include "sqlite3.h"

void agregarNuevoPrestamo(sqlite3* db);
void buscarPrestamosPendientes(sqlite3* db);
void mostrarMenuPrestamos();
void ejecutarMenuPrestamos(sqlite3* db);

#endif /* PRESTAMOS_H */
