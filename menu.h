#ifndef MENU_H
#define MENU_H

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
void mostrarMenuUsuarios();
void ejecutarMenuUsuarios(sqlite3* db);

// CONFIGURACIÓN DEL SISTEMA
void mostraMenuConfiguracionPoliticas();
void ejecutarMenuConfiguracionPoliticas(sqlite3* db);
void mostrarMenuConfiguracion();
void ejecutarMenuConfiguracion(sqlite3* db);


#endif /* MENU_H */