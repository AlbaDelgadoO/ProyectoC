#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "dataBase.h"
#include <string.h>
#include "menu.h"

#define USUARIO_EDITADO_CORRECTAMENTE 0
#define USUARIO_NO_EXISTE 1
#define ERROR_EDICION_USUARIO 2
#define ERROR_BORRANDO_USUARIO 3
#define USUARIO_BORRADO_CORRECTAMENTE 4

// Implementación de las funciones para interactuar con la base de datos
sqlite3* inicializarBaseDatos(const char* nombreArchivo) {
    sqlite3* db;
    int resultado = sqlite3_open(nombreArchivo, &db);
    if (resultado != SQLITE_OK) {
        fprintf(stderr, "Error al abrir o crear la base de datos: %s\n\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    return db;
}

void crearTablaLibro(sqlite3* db){
    char* sql = "CREATE TABLE IF NOT EXISTS Libro (ISBN TEXT PRIMARY KEY, Titulo TEXT, Genero TEXT, Nom_Autor TEXT REFERENCES Autor(Nombre), Ap_Autor TEXT REFERENCES Autor(Apellido), Año_Publicacion INTEGER, NumEjemplares INTEGER, Cod_Editorial INTEGER)";
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
    char* sql = "INSERT INTO Libro (ISBN, Titulo, Genero, Nom_Autor, Ap_Autor, Año_Publicacion, NumEjemplares, Cod_Editorial) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";

    // Crear una declaración SQL
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    char isbn[14];
    strncpy(isbn, libro.ISBN, 13);
    isbn[13] = '\0';

    // Bindear los parámetros a la declaración SQL
    sqlite3_bind_text(stmt, 1, isbn, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, libro.titulo, -1, SQLITE_STATIC); 
    sqlite3_bind_text(stmt, 3, libro.genero, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, libro.nom_autor, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, libro.apellido_autor, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, libro.anyoPublicacion);
    sqlite3_bind_int(stmt, 7, libro.numEjemplares);
    sqlite3_bind_int(stmt, 8, libro.cod_Editorial);



    // Ejecutar la consulta
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
        printf("Revise los datos introducidos\n\n");
        sqlite3_finalize(stmt);
        return;
    } else {
        printf("Libro agregado correctamente\n");
        printf("\n");
        comprobarAutor(db, libro);
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
    printf("\n");
    printf("================================================================= Libros en la Biblioteca =====================================================================================\n");
    printf("%-15s %-30s %-20s %-20s %-30s %-20s %-20s %-20s\n", "ISBN", "Titulo", "Genero", "Nombre_A", "Apellido_A", "AñoPublicacion", "nEjemplares", "cod_Editorial");
    printf("===============================================================================================================================================================================\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* isbn = sqlite3_column_text(stmt, 0);
        const unsigned char* titulo = sqlite3_column_text(stmt, 1);
        const unsigned char* genero = sqlite3_column_text(stmt, 2);
        const unsigned char* n_autor = sqlite3_column_text(stmt, 3);
        const unsigned char* a_autor = sqlite3_column_text(stmt, 4);
        int anyo = sqlite3_column_int(stmt, 5);
        int nEjemplares = sqlite3_column_int(stmt, 6);
        int cod_e = sqlite3_column_int(stmt, 7);


        printf("%-15s %-30s %-20s %-20s %-30s %-20d %-20d %-20d\n\n", isbn, titulo, genero, n_autor, a_autor, anyo, nEjemplares, cod_e);
    }

    sqlite3_finalize(stmt);
}
char* obtenerDatosLibros(sqlite3* db) {
    char* librosData = malloc(4096 * sizeof(char)); // Reservar memoria para almacenar los datos de los libros
    librosData[0] = '\0'; // Inicializar la cadena vacía
    sqlite3_stmt* stmt;
    const char* sql = "SELECT * FROM Libro";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        strcpy(librosData, "Error al obtener los datos de los libros.");
        return librosData;
    }

    // Construir la cadena con los datos de los libros
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* isbn = sqlite3_column_text(stmt, 0);
        const unsigned char* titulo = sqlite3_column_text(stmt, 1);
        const unsigned char* genero = sqlite3_column_text(stmt, 2);
        const unsigned char* n_autor = sqlite3_column_text(stmt, 3);
        const unsigned char* a_autor = sqlite3_column_text(stmt, 4);
        int anyo = sqlite3_column_int(stmt, 5);
        int nEjemplares = sqlite3_column_int(stmt, 6);
        int cod_e = sqlite3_column_int(stmt, 7);

        // Concatenar los datos del libro a la cadena
        sprintf(librosData + strlen(librosData), "%s | %s | %s | %s | %s | %d | %d | %d\n",
                isbn, titulo, genero, n_autor, a_autor, anyo, nEjemplares, cod_e);
    }

    sqlite3_finalize(stmt); // Liberar el statement
    return librosData;
}
void buscarLibroBD(sqlite3* db, const char* termino) {
    char* sql = "SELECT * FROM Libro WHERE Titulo LIKE '%' || ? || '%' OR Nom_Autor LIKE ? OR Genero LIKE ?";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Bindear los parámetros a la consulta
    sqlite3_bind_text(stmt, 1, termino, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, termino, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, termino, -1, SQLITE_STATIC);

    printf("================================================================= Resultados de la búsqueda ===================================================================================\n");
    printf("%-15s %-30s %-20s %-20s %-30s %-20s %-20s %-20s\n", "ISBN", "Titulo", "Genero", "Nombre_A", "Apellido_A", "AñoPublicacion", "nEjemplares", "cod_Editorial");
    printf("===============================================================================================================================================================================\n");

    int resultado;
    while ((resultado = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char* isbn = sqlite3_column_text(stmt, 0);
        const unsigned char* titulo = sqlite3_column_text(stmt, 1);
        const unsigned char* genero = sqlite3_column_text(stmt, 2);
        const unsigned char* n_autor = sqlite3_column_text(stmt, 3);
        const unsigned char* a_autor = sqlite3_column_text(stmt, 4);
        int nEjemplares = sqlite3_column_int(stmt, 6);
        int anyo = sqlite3_column_int(stmt, 5);
        int cod_e = sqlite3_column_int(stmt, 7);


        printf("%-15s %-30s %-20s %-20s %-30s %-20d %-20d %-20d\n\n", isbn, titulo, genero, n_autor, a_autor, anyo, nEjemplares, cod_e);
    }
    printf("\n");

    if (resultado != SQLITE_DONE) {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}
char* obtenerLibro(sqlite3* db, const char* termino) {
    char* librosData = malloc(4096 * sizeof(char)); // Reservar memoria para almacenar los datos de los libros
    librosData[0] = '\0'; // Inicializar la cadena vacía
    sqlite3_stmt* stmt;
    char* sql = "SELECT * FROM Libro WHERE Titulo LIKE '%' || ? || '%' OR Nom_Autor LIKE ? OR Genero LIKE ?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        strcpy(librosData, "Error al buscar el libro en la base de datos.");
        return librosData;
    }

    // Bindear los parámetros a la consulta
    sqlite3_bind_text(stmt, 1, termino, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, termino, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, termino, -1, SQLITE_STATIC);

    // Construir la cadena con los detalles de los libros
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* isbn = sqlite3_column_text(stmt, 0);
        const unsigned char* titulo = sqlite3_column_text(stmt, 1);
        const unsigned char* genero = sqlite3_column_text(stmt, 2);
        const unsigned char* n_autor = sqlite3_column_text(stmt, 3);
        const unsigned char* a_autor = sqlite3_column_text(stmt, 4);
        int anyo = sqlite3_column_int(stmt, 5);
        int nEjemplares = sqlite3_column_int(stmt, 6);
        int cod_e = sqlite3_column_int(stmt, 7);

        // Concatenar los detalles del libro a la cadena
        sprintf(librosData + strlen(librosData), "%s|%s|%s|%s|%s|%d|%d|%d\n",
                isbn, titulo, genero, n_autor, a_autor, anyo, nEjemplares, cod_e);
    }

    sqlite3_finalize(stmt); // Liberar el statement
    return librosData;
}

void comprobarAutor(sqlite3* db, Libro libro) {
    char* sql = "SELECT * FROM Autor WHERE Nombre LIKE ? AND Apellido LIKE ?";
    sqlite3_stmt* stmt;
        
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Bindear los parámetros a la consulta
    sqlite3_bind_text(stmt, 1, libro.nom_autor, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, libro.apellido_autor, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        insertarAutor(db, libro);
    }
    
     sqlite3_finalize(stmt);
}



// GESTIÓN DE USUARIOS
void crearTablaUsuario(sqlite3* db) {
    char* sql = "CREATE TABLE IF NOT EXISTS Usuario (ID_Usuario TEXT PRIMARY KEY, Nombre TEXT, Apellido TEXT, Correo TEXT, Contrasenya TEXT)";
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
        printf("Revise los datos introducidos\n");
        sqlite3_finalize(stmt);
        return;
    } else {
        printf("Usuario agregado correctamente\n");
    }
    printf("\n");

    sqlite3_finalize(stmt);
}

int insertarUsuarioServer(sqlite3* db, Usuario usuario) {
    char* sql = "INSERT INTO Usuario (ID_Usuario, Nombre, Apellido, Correo, Contrasenya) VALUES (?, ?, ?, ?, ?)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, usuario.ID_Usuario, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, usuario.nombreU, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, usuario.apellidoU, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, usuario.correo, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, usuario.contrasenya, -1, SQLITE_STATIC);


    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return 0;
    } else {
        return 1;
    }
    printf("\n");

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
    printf("======================== Usuarios en la Biblioteca ===============================\n");
    printf("%-10s %-10s %-20s %-25s %-15s\n", "ID", "Nombre", "Apellido", "Correo", "Contrasenya");
    printf("==================================================================================\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* id = sqlite3_column_text(stmt, 0);
        const unsigned char* nombre = sqlite3_column_text(stmt, 1);
        const unsigned char* apellido = sqlite3_column_text(stmt, 2);
        const unsigned char* correo = sqlite3_column_text(stmt, 3);
        const unsigned char* contr = sqlite3_column_text(stmt, 4);

        printf("%-10s %-10s %-20s %-25s %-15s\n", id, nombre, apellido, correo, contr);
    }
    printf("\n");

    sqlite3_finalize(stmt);
}

// funcion adaptada para cliente-servidor
char* obtenerUsuarios(sqlite3* db) {
    char* usuarios = malloc((4096*5)* sizeof(char));
    usuarios[0] = '\0';
    sqlite3_stmt* stmt;
    const char* sql = "SELECT * FROM Usuario";
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        strcpy(usuarios, "Error al obtener los datos de los usuarios");
        return usuarios;
    }

    while(sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* id = sqlite3_column_text(stmt, 0);
        const unsigned char* nombre = sqlite3_column_text(stmt, 1);
        const unsigned char* apellido = sqlite3_column_text(stmt, 2);
        const unsigned char* correo = sqlite3_column_text(stmt, 3);
        const unsigned char* contr = sqlite3_column_text(stmt, 4);

        sprintf(usuarios + strlen(usuarios), "%-10s %-10s %-20s %-25s %-15s\n",
                id, nombre, apellido, correo, contr);
    }
    sqlite3_finalize(stmt);
    return usuarios;
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
    printf("======================== Coincidencias encontradas ===============================\n");
    printf("%-10s %-10s %-20s %-25s %-15s\n", "ID", "Nombre", "Apellido", "Correo", "Contraseña");
    printf("==================================================================================\n");

    int resultado;
    while ((resultado = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char* id = sqlite3_column_text(stmt, 0);
        const unsigned char* nombre = sqlite3_column_text(stmt, 1);
        const unsigned char* apellido = sqlite3_column_text(stmt, 2);
        const unsigned char* correo = sqlite3_column_text(stmt, 3);
        const unsigned char* contr = sqlite3_column_text(stmt, 4);

        printf("%-10s %-10s %-20s %-25s %-15s\n", id, nombre, apellido, correo, contr);
    }

    if (resultado != SQLITE_DONE) {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
    }

    printf("\n");

    sqlite3_finalize(stmt);
}

char* buscarUsuariosServer(sqlite3* db, char* termino) {
    char* result = malloc(4096 * sizeof(char));
    result[0] = '\0';

    // Preparar término para LIKE
    char likeTermino[52];  // 50 + 2 para '%' y '\0'
    snprintf(likeTermino, sizeof(likeTermino), "%%%s%%", termino);

    const char* sql = "SELECT * FROM Usuario WHERE ID_Usuario LIKE ? OR Nombre LIKE ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        strcpy(result, "Error al buscar usuario en la base de datos");
        return result;
    }

    // Bindear los parámetros a la consulta
    sqlite3_bind_text(stmt, 1, likeTermino, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, likeTermino, -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* id = sqlite3_column_text(stmt, 0);
        const unsigned char* nombre = sqlite3_column_text(stmt, 1);
        const unsigned char* apellido = sqlite3_column_text(stmt, 2);
        const unsigned char* correo = sqlite3_column_text(stmt, 3);
        const unsigned char* contr = sqlite3_column_text(stmt, 4);

        sprintf(result + strlen(result), "%-10s %-10s %-20s %-25s %-15s\n", id, nombre, apellido, correo, contr);
    }

    sqlite3_finalize(stmt);
    return result;
}

int editarUsuarioServidor(sqlite3* db, const char* id, const char* nombre, const char* apellido, const char* correo, const char* contrasenya) {
    char* sql = "UPDATE Usuario SET Nombre = ?, Apellido = ?, Correo = ?, Contrasenya = ? WHERE ID_Usuario = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return ERROR_EDICION_USUARIO;
    }

    sqlite3_bind_text(stmt, 1, nombre, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, apellido, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, correo, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, contrasenya, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, id, -1, SQLITE_STATIC);

    int result = sqlite3_step(stmt);
    if (result == SQLITE_DONE) {
        if (sqlite3_changes(db) > 0) {
            sqlite3_finalize(stmt);
            return USUARIO_EDITADO_CORRECTAMENTE;
        } else {
            sqlite3_finalize(stmt);
            return USUARIO_NO_EXISTE;
        }
    } else {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return ERROR_EDICION_USUARIO;
    }
}

int borrarUsuarioServidor(sqlite3* db, const char* id) {
    char* sql = "DELETE FROM Usuario WHERE ID_Usuario = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return ERROR_BORRANDO_USUARIO;
    }

    sqlite3_bind_text(stmt, 1, id, -1, SQLITE_STATIC);
    
    int result = sqlite3_step(stmt);
    if (result == SQLITE_DONE) {
        if (sqlite3_changes(db) > 0) {
            sqlite3_finalize(stmt);
            return USUARIO_BORRADO_CORRECTAMENTE;
        } else {
            sqlite3_finalize(stmt);
            return USUARIO_NO_EXISTE;
        }
    } else {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return ERROR_BORRANDO_USUARIO;
    }

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
    printf("Usuario modificado con exito, pulse 6 para salir\n");
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
    printf("Usuario modificado con exito, pulse 6 para salir\n");
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
    printf("Usuario modificado con exito, pulse 6 para salir\n");
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
    printf("Usuario modificado con exito, pulse 6 para salir\n");
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
    printf("Usuario modificado con exito, pulse 6 para salir\n");
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
        printf("%-10s %-10s %-20s %-25s %-15s\n", "ID", "Nombre", "Apellido", "Correo", "Contrasenya");
        const unsigned char* id = sqlite3_column_text(stmt, 0);
        const unsigned char* nombre = sqlite3_column_text(stmt, 1);
        const unsigned char* apellido = sqlite3_column_text(stmt, 2);
        const unsigned char* correo = sqlite3_column_text(stmt, 3);
        const unsigned char* contr = sqlite3_column_text(stmt, 4);

        printf("%-10s %-10s %-20s %-25s %-15s\n", id, nombre, apellido, correo, contr);

        ejecutarMenuEdicion(db, (char *)id);

    } else if (result == SQLITE_DONE) {
        printf("El usuario con ID '%s' no existe.\n", id);
    } else {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
    }
    printf("\n");

}

void borrarUsuarioDB(sqlite3 *db, char* id) {
    char* sql = "DELETE FROM Usuario WHERE ID_Usuario = ?";
    sqlite3_stmt* stmt;

     if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Bindear parametros
    sqlite3_bind_text(stmt, 1, id, -1, SQLITE_STATIC);
    

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
    } 

    sqlite3_finalize(stmt);            
    printf("Usuario eliminado con exito.\n");
    printf("\n");
}

//GESTION DE PRESTAMOS
void crearTablaPrestamo(sqlite3* db) {
    // Consulta SQL para crear la tabla Prestamo
    const char* sql = "CREATE TABLE IF NOT EXISTS Prestamo ("
                      "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "ISBN_Libro TEXT REFERENCES Libro(ISBN),"
                      "ID_Usuario TEXT REFERENCES Usuario(ID_Usuario),"
                      "Fecha_Vencimiento TEXT,"
                      "Estado INTEGER DEFAULT 0" //0 indica que el préstamo está pendiente y 1 indica que el préstamo ha sido devuelto
                      ")";

    // Ejecutar la consulta
    char* errMsg = NULL;
    sqlite3_exec(db, sql, NULL, 0, &errMsg);
    if (errMsg != NULL) {
        sqlite3_free(errMsg);
    }
}

void insertarPrestamo(sqlite3* db, Prestamo p)
{
    // Insertar el nuevo préstamo en la base de datos
    char* sql = "INSERT INTO Prestamo (ISBN_Libro, ID_Usuario, Fecha_Vencimiento) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, p.ISBN, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, p.ID_Usuario, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, p.fechaDevolucion, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
        printf("Por favor, revise que el ISBN introducido es válido\n\n");

    } else {
        printf("El nuevo prestamo ha sido agregado correctamente.\n\n");
    }


    sqlite3_finalize(stmt);
}

void renovarPrestamoDB(sqlite3* db, char* idLibro) {
    // Consultar la fecha de vencimiento actual del préstamo asociado al libro
    char selectSql[200];
    sprintf(selectSql, "SELECT Fecha_Vencimiento FROM Prestamo WHERE ISBN_Libro = '%s'", idLibro);
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, selectSql, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Si existe un préstamo asociado al libro, actualizar la fecha de vencimiento
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* fechaVencimientoActual = (const char*)sqlite3_column_text(stmt, 0);

        // Calcular la nueva fecha de vencimiento extendida por 15 días
        char* nuevaFechaVencimiento = calcularNuevaFechaVencimiento(fechaVencimientoActual);

        // Actualizar la fecha de vencimiento en la base de datos
        char updateSql[200];
        sprintf(updateSql, "UPDATE Prestamo SET Fecha_Vencimiento = '%s' WHERE ISBN_Libro = '%s'", nuevaFechaVencimiento, idLibro);
        int updateResult = sqlite3_exec(db, updateSql, NULL, 0, NULL);
        if (updateResult != SQLITE_OK) {
            printf("Error al actualizar la fecha de vencimiento: %s\n", sqlite3_errmsg(db));
            return;
        }

        printf("El prestamo del libro con ID %s ha sido renovado con exito.\n\n", idLibro);
    } else {
        printf("No se encontro un prestamo asociado al libro con ID %s.\n", idLibro);
    }

    // Liberar la consulta preparada
    sqlite3_finalize(stmt);
}

bool registrarDevolucionLibro(sqlite3* db, char* idLibro) {
    // Consultar la tabla Prestamo para verificar si hay préstamos activos para este libro
    char selectSql[100];
    sprintf(selectSql, "SELECT ID FROM Prestamo WHERE ISBN_Libro = '%s' AND Estado = 0", idLibro);
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, selectSql, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return false;
    }

    // Ejecutar la consulta y verificar si hay resultados
    bool prestamosActivos = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        prestamosActivos = true;
        int idPrestamo = sqlite3_column_int(stmt, 0);

        // Actualizar el estado del préstamo a devuelto (Estado = 1)
        char updateSql[100];
        sprintf(updateSql, "UPDATE Prestamo SET Estado = 1 WHERE ID = %d", idPrestamo);
        int updateResult = sqlite3_exec(db, updateSql, NULL, 0, NULL);
        if (updateResult != SQLITE_OK) {
            printf("Error al actualizar el estado del prestamo: %s\n", sqlite3_errmsg(db));
            return false;
        }
    }

    // Liberar la consulta preparada
    sqlite3_finalize(stmt);

    return prestamosActivos;
}

//TABLA AUTOR
void crearTablaAutor(sqlite3* db){
    char* sql = "CREATE TABLE IF NOT EXISTS Autor(ID INTEGER PRIMARY KEY AUTOINCREMENT, Nombre TEXT, Apellido TEXT)";
    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL)!= SQLITE_OK){
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    if(sqlite3_step(stmt) !=SQLITE_DONE){
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return;
    }
}

void insertarAutor(sqlite3*db, Libro libro){
    char*sql= "INSERT INTO Autor(Nombre,Apellido)VALUES(?,?)";
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK){
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, libro.nom_autor, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, libro.apellido_autor, -1, SQLITE_STATIC);

    if(sqlite3_step(stmt) !=SQLITE_DONE){
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);
}

void leerAutores(sqlite3* db){
    char* sql = "SELECT * FROM Autor";
    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK){
        fprintf(stderr, "Error al preparar las consultas: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("=== LISTA DE AUTORES === \n");
    printf("%-20s %-20s %20s\n", "ID", "Nombre", "Apellido");
    printf("=====================================================================\n");
    while(sqlite3_step(stmt) == SQLITE_ROW){
        int id_A = sqlite3_column_int(stmt, 0);
        const unsigned char* nombre =sqlite3_column_text(stmt, 1);
        const unsigned char* apellido = sqlite3_column_text(stmt, 2);
        printf("ID: %d, Nombre: %s, Apelliddo: %s\n", id_A, nombre, apellido);
    }
    printf("\n");

    sqlite3_finalize(stmt);
}

void eliminarAutores(sqlite3* db, const char* nombreAutor){
    char* sql = "DELETE FROM Autor WHERE Nombre = ?";
    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) !=SQLITE_OK){
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, nombreAutor, -1, SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_DONE){
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
    }else{
        printf("Autor eliminado correctamente \n");
    }

     sqlite3_finalize(stmt);
    
}

void buscarAutor(sqlite3* db, const char* termino){
    char* sql= "SELECT * FROM Autor WHERE Nombre LIKE ? OR APELLIDO LIKE ?";
    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) !=SQLITE_OK){
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, termino, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, termino, -1, SQLITE_STATIC);

    printf("=== Resultados de la búsqueda ===\n");
    while(sqlite3_step(stmt) == SQLITE_ROW){
        int id_A = sqlite3_column_int(stmt, 0);
        const unsigned char* nombre =sqlite3_column_text(stmt, 1);
        const unsigned char* apellido = sqlite3_column_text(stmt, 2);
        printf("ID: %d, Nombre: %s, Apelliddo: %s\n", id_A, nombre, apellido);
    }

    sqlite3_finalize(stmt);
    

}


void borrarUsuarioTabla



(sqlite3* db) {
    char* sql = "DROP TABLE Usuario";
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

void borrarAutor(sqlite3* db) {
    char* sql = "DROP TABLE Autor";
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

void borrarLibro(sqlite3* db) {
    char* sql = "DROP TABLE Libro";
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

void borrarPrestamo(sqlite3* db) {
    char* sql = "DROP TABLE Prestamo";
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