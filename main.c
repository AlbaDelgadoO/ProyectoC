#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "menu.h"
#include "dataBase.h"

/*
int main() {
    sqlite3* db;

    // Inicializar la base de datos
    db = inicializarBaseDatos("biblioteca.db");


    // Crear tabla de libros si no existe
    crearTablaLibro(db);

    // Crear tabla usuario
    crearTablaUsuario(db);

    //Crear tabla prestamo
    crearTablaPrestamo(db);

    // Crear tabla Autor
    crearTablaAutor(db);

    // Ejecutar el menú principal
    ejecutarMenuPrincipal(db);

    //CREAR  EL FICHERO LOG 
    crearFicheroLog();

    // Cerrar la base de datos
    sqlite3_close(db);

    return 0;
}*/