#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sqlite3.h"
#include "dataBase.h"
#include "menu.h"
#include <string.h>
#include <time.h>

#define TAMANIO_MAXIMO_INFORME 1000

void agregarNuevoLibro(sqlite3* db) {
    // Solicitar detalles del nuevo libro al usuario
    printf("\n");
    printf("Ingrese el ISBN del libro: ");
    char isbn[15];
    scanf(" %[^\n]", isbn);

    printf("Ingrese el titulo del libro: ");
    char titulo[100];
    scanf(" %[^\n]", titulo);

    printf("Ingrese el genero del libro: ");
    char genero[100];
    scanf(" %[^\n]", genero);

    printf("Ingrese el nombre del autor: ");
    char autor[100];
    scanf(" %[^\n]", autor);

    printf("Ingrese el apellido del autor: ");
    char apellido[100];
    scanf(" %[^\n]", apellido);

    printf("Ingrese el numero de ejemplares del libro: ");
    int nEjemplares;
    scanf("%i", &nEjemplares);

    printf("Ingrese el año de publicacion del libro: ");
    int aPubl;
    scanf("%i", &aPubl);

    printf("Ingrese el codigo de editorial del libro: ");
    int cod_E;
    scanf("%i", &cod_E);

    
    // Crear una instancia de Libro con los detalles ingresados por el usuario
    Libro nuevoLibro;
    strcpy(nuevoLibro.ISBN, isbn);
    strcpy(nuevoLibro.titulo, titulo);
    strcpy(nuevoLibro.genero, genero);
    strcpy(nuevoLibro.nom_autor, autor);
    strcpy(nuevoLibro.apellido_autor, apellido);
    nuevoLibro.numEjemplares = nEjemplares;
    nuevoLibro.anyoPublicacion = aPubl;
    nuevoLibro.cod_Editorial = cod_E;

    // Insertar el nuevo libro en la base de datos
    insertarLibro(db, nuevoLibro);
}

void buscarLibro(sqlite3* db) {
    printf("\n");
    char termino[100];
    printf("Ingrese el termino de busqueda (titulo, autor, etc.): ");
    scanf("%s", termino);

    // Realizar la búsqueda en la base de datos
    printf("Buscando libros que coincidan con '%s'...\n", termino);
    // Llamar a la función para buscar libros en la base de datos
    buscarLibroBD(db, termino);
    // Manejar el caso en que no se encuentren resultados
}

void mostrarMenuLibros() {
    printf("\n=== Menu de Gestion de Libros ===\n");
    printf("1. Agregar Nuevo Libro\n");
    printf("2. Mostrar Todos los Libros\n");
    printf("3. Buscar Libro\n");
    printf("4. Volver al Menu Principal\n");
    printf("Seleccione una opcion: ");
}

void ejecutarMenuLibros(sqlite3* db) {
    int opcion;
    do {
        mostrarMenuLibros();
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                agregarNuevoLibro(db);
                break;
            case 2:
                leerLibros(db);
                break;
            case 3:
                buscarLibro(db);
                break;
            case 4:
                printf("\n");
                printf("Volviendo al Menu Principal...\n\n");
                break;
            default:
                printf("Opcion no valida. Por favor, seleccione una opcion valida.\n");
                break;
        }
    } while (opcion != 4);
}

// GESTION DE USUARIOS

void agregarNuevoUsuario(sqlite3* db) {
// Solicitar detalles del nuevo usuario
    printf("\n");
    printf("Ingrese el ID del usuario: ");
    char id[50];
    scanf("%s", id);

    printf("Ingrese el nombre del usuario: ");
    char nombre[50];
    scanf("%s", nombre);

    printf("Ingrese el apellido del usuario: ");
    char apellido[50];
    scanf("%s", apellido);

    printf("Ingrese el correo del usuario: ");
    char correo[100];
    scanf("%s", correo);

    printf("Ingrese la contraseña del usuario: ");
    char contr[50];
    scanf("%s", contr);

    // Crear una instancia de Usuario
    Usuario nuevoUsuario;

    // Asignar valores al usuario
    strcpy(nuevoUsuario.ID_Usuario, id);
    strcpy(nuevoUsuario.nombreU, nombre);
    strcpy(nuevoUsuario.apellidoU, apellido);
    strcpy(nuevoUsuario.correo, correo);
    strcpy(nuevoUsuario.contrasenya, contr);


    insertarUsuario(db, nuevoUsuario);
}

void buscarUsuario(sqlite3* db) {
    printf("\n");
    printf("Ingrese el ID o nombre del usuario: ");
    char termino[50];
    scanf("%s", termino);

    buscarUsuariosDB(db, termino);
}

void editarUsuario(sqlite3* db){
    printf("\n");
    printf("Introduce el ID del usuario que deseas editar: ");
    char id[50];
    scanf("%s", id);

    editarUsuarioDB(db, id);
}

void borrarUsuario(sqlite3* db) {
    printf("\n");
    printf("Introduce el ID del usuario que deseas borrar: ");
    char id[50];
    scanf("%s", id);

    borrarUsuarioDB(db, id);
}


void mostrarMenuUsuarios() {
    printf("\n=== Menu de Gestion de Usuarios ===\n");
    printf("1. Agregar Nuevo Usuario\n");
    printf("2. Mostrar Todos los Usuarios\n");
    printf("3. Buscar Usuario\n");
    printf("4. Editar Usuario\n");
    printf("5. Borrar Usuario\n");
    printf("6. Volver al Menu Principal\n");
    printf("Seleccione una opcion: ");
}

void ejecutarMenuUsuarios(sqlite3* db) {
    int opcion;
    do {
        mostrarMenuUsuarios();
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                agregarNuevoUsuario(db);
                break;
            case 2:
                leerUsuarios(db);
                break;
            case 3:
                buscarUsuario(db);
                break;
            case 4:
                editarUsuario(db);
                break;
            case 5:
                borrarUsuario(db);
                break;
            case 6:
                printf("\n");
                printf("Volviendo al Menu Principal...\n\n");                
                break;
            default:
                printf("Opcion no valida. Por favor, seleccione una opcion valida.\n");
                break;
        }
    } while (opcion != 6);
}

// GESTION DE PRESTAMOS

bool usuarioExiste(sqlite3* db, const char* idUsuario) {
    // Consulta SQL para buscar el usuario por su ID
    const char* sql = "SELECT COUNT(*) FROM Usuario WHERE ID_Usuario = ?";

    // Preparar la consulta
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return false;
    }

    // Bind the user ID parameter
    sqlite3_bind_text(stmt, 1, idUsuario, -1, SQLITE_STATIC);

    // Ejecutar la consulta
    int result = sqlite3_step(stmt);

    // Verificar si se encontró un usuario
    bool existe = false;
    if (result == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        existe = count > 0;
    } else if (result != SQLITE_DONE) {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
    }

    // Finalizar la consulta
    sqlite3_finalize(stmt);

    return existe;
}

void agregarNuevoPrestamo(sqlite3* db) {
    // Solicitar detalles del nuevo préstamo al usuario
    printf("\n");
    printf("Ingrese el ISBN del libro: ");
    char idLibro[15];
    scanf("%14s", idLibro);

    printf("Ingrese el ID del usuario: ");
    char idUsuario[50];
    scanf("%49s", idUsuario);

    // Verificar si el usuario existe en la base de datos
    if (!usuarioExiste(db, idUsuario)) {
        printf("El usuario con el ID '%s' no existe en la base de datos.\n", idUsuario);
        return;
    }

    printf("Ingrese la fecha de vencimiento (YYYY-MM-DD): ");
    char fechaVencimiento[11];
    scanf("%10s", fechaVencimiento);

    // Crear una instancia de Prestamo
    Prestamo nuevoPrestamo;

    // Asignar valores al prestamo
    strcpy(nuevoPrestamo.ISBN, idLibro);
    strcpy(nuevoPrestamo.ID_Usuario, idUsuario);
    strcpy(nuevoPrestamo.fechaDevolucion, fechaVencimiento);

    insertarPrestamo(db, nuevoPrestamo);

}

// Función para obtener la nueva fecha de vencimiento extendida por 15 días
char* calcularNuevaFechaVencimiento(const char* fechaVencimientoActual) {
    struct tm *tm_fecha;
    time_t tiempo;
    
    // Analiza la fecha de vencimiento actual desde la cadena
    int year, month, day;
    sscanf(fechaVencimientoActual, "%d-%d-%d", &year, &month, &day);
    
    // Obtiene el tiempo actual
    tiempo = time(NULL);
    tm_fecha = gmtime(&tiempo);
    tm_fecha->tm_year = year - 1900; // Año desde 1900
    tm_fecha->tm_mon = month - 1; // Mes (0-11)
    tm_fecha->tm_mday = day; // Día del mes
    
    // Añade 15 días a la fecha de vencimiento actual
    tiempo = mktime(tm_fecha) + (15 * 24 * 60 * 60); // 15 días en segundos
    
    // Convierte el tiempo en estructura tm
    tm_fecha = gmtime(&tiempo);
    
    // Formatea la nueva fecha de vencimiento en una cadena
    static char nuevaFecha[11];
    snprintf(nuevaFecha, sizeof(nuevaFecha), "%04d-%02d-%02d", tm_fecha->tm_year + 1900, tm_fecha->tm_mon + 1, tm_fecha->tm_mday);

    return nuevaFecha;
}

void renovarPrestamo(sqlite3* db) {
    printf("\n");
    printf("Ingrese el ISBN del libro cuyo prestamo desea renovar: ");
    char idLibro[15];
    scanf("%14s", idLibro);
    
    // Llamar a la función en dataBase.c para renovar el préstamo
    renovarPrestamoDB(db, idLibro);
}

void registrarDevolucion(sqlite3* db) {
    // Solicitar al usuario que ingrese el ID del libro devuelto
    char idLibro[15];
    printf("\nIngrese el ISBN del libro a devolver: ");
    scanf("%14s", idLibro);

    // Llamar a la función en dataBase.c para registrar la devolución del libro
    bool devolucionExitosa = registrarDevolucionLibro(db, idLibro);

    // Mostrar mensaje de acuerdo a la devolución
    if (devolucionExitosa) {
        printf("El libro ha sido devuelto con exito.\n\n");
    } else {
        printf("No hay prestamos pendientes para este libro.\n\n");
    }
}

void buscarPrestamosPendientes(sqlite3* db) {
    // Solicitar al usuario que ingrese el ID del usuario para buscar sus préstamos pendientes
    char idUsuario[50];
    printf("\nIngrese el ID del usuario para buscar sus prestamos pendientes: ");
    scanf("%s", idUsuario);

    // Consultar la tabla Prestamo para obtener los préstamos pendientes del usuario especificado
    char selectSql[200];
    sprintf(selectSql, "SELECT ISBN_Libro, Fecha_Vencimiento FROM Prestamo WHERE ID_Usuario = '%s' AND Estado = 0", idUsuario);
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, selectSql, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Mostrar los préstamos pendientes del usuario
    printf("Prestamos pendientes del usuario %s:\n", idUsuario);
    printf("ISBN_Libro\tFecha_Vencimiento\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* idLibro = (const char*)sqlite3_column_text(stmt, 0);
        const char* fechaVencimiento = (const char*)sqlite3_column_text(stmt, 1);
        printf("%s\t\t%s\n", idLibro, fechaVencimiento);
    }

    printf("\n");

    // Liberar la consulta preparada
    sqlite3_finalize(stmt);
}

void mostrarMenuPrestamos() {
    printf("\n=== Menu de Gestion de Prestamos ===\n");
    printf("1. Agregar Nuevo Prestamo\n");
    printf("2. Renovar Prestamo Existente\n");
    printf("3. Registrar Devolucion de Libro\n");
    printf("4. Buscar Prestamos Pendientes\n");
    printf("5. Volver al Menu Principal\n");
    printf("Seleccione una opcion: ");
}

void ejecutarMenuPrestamos(sqlite3* db) {
    int opcion;
    do {
        mostrarMenuPrestamos();
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                agregarNuevoPrestamo(db);
                break;
            case 2:
                renovarPrestamo(db);
                break;
            case 3:
                registrarDevolucion(db);
                break;
            case 4:
                buscarPrestamosPendientes(db);
                break;
            case 5:
                printf("\n");
                printf("Volviendo al Menu Principal...\n\n");
                break;
            default:
                printf("Opcion no valida. Por favor, seleccione una opcion valida.\n");
                break;
        }
    } while (opcion != 5);
}

//CONFIGURACION DEL SISTEMA 

void crearFicheroLog(){
    FILE* archivoLog = fopen("registro.log", "w");
    if(archivoLog == NULL){
        printf("Error al crear el archivo de resgistro\n");
        return;
    }
    fprintf(archivoLog, "Inicio del registro...\n");
    fclose(archivoLog);
    printf("Archivo de resgistro creado correctamente\n");
    
}


void leerFicheroConfiguracion(){
    FILE*archivo;
    char linea[100];

    archivo= fopen("configuracion.txt", "r");
    if(archivo== NULL){
        printf("No se puedo encontrar el archivo\n");
        return;
    }

    printf("\n=== FICHERO DE CONFIGURACION ===\n");
    while(fgets(linea, sizeof(linea), archivo) !=NULL){
        printf("%s", linea);
    };
    fclose(archivo);
}

void mostrarMenuConfiguracionFicheros(){
    printf("\n=== Fichero de configuracion ===\n");
    printf("1. Leer Fichero de configuracion con las rutas del los archivos del programa \n");
    printf("2. Volver al Menu de Configuracion del Sistema\n");
    printf("Seleccione una opcion: ");
}

void ejecutarMenuConfiguracionFicheros(sqlite3*db){
    int opcion;
    do{
        mostrarMenuConfiguracionFicheros();
        scanf("%d", &opcion);
        switch(opcion){
            case 1:
                leerFicheroConfiguracion();
                break;
            case 2:
                printf("Volviendo al menu de configuracion del sistema...");
                break;
            default:
                printf("Opcion no valida. Por favor, seleccione una opcion valida.\n");
                break;
            
        }
    }while (opcion !=2);
}

void actualizarParametros(const char *clave, const char *nuevoValor){
    FILE *archivo = fopen("politicas.txt", "r");
    if(!archivo){
        printf("No se pudo encontrar el archivo");
        return;
    }
   
   FILE *temp = fopen("politicas_temp.txt", "w");
   if(!temp){
    printf("No se pudo abrir el archivo temporal para la escritura de los cambios");
    fclose(archivo);
    return;
   }

   char linea[100];
   int claveActualizada= 0;

   while (fgets(linea, sizeof(linea), archivo) != NULL)
   { 
    char copiaLinea[100];
    strcpy(copiaLinea, linea);
    char *token = strtok(copiaLinea, "=\n");

    if( token != NULL && strcmp(token, clave)== 0){
        fprintf(temp, "%s= %s\n", clave, nuevoValor);
        claveActualizada=1;
    }else{
        fprintf(temp, "%s", linea);
    }
   }
   
    fclose(archivo);
    fclose(temp);

   remove("politicas.txt");
   rename("politicas_temp.txt", "politicas.txt");
   

   if(claveActualizada==1){
    printf("Configuracion '%s' actualizada con exito \n\n", clave);
   }else{
    printf("Clave '%s' no encontrada\n", clave );
   }
   
}
void mostrarMenuConfiguracionParametros(){
    printf("\n=== Configuracion de Politicas y Parametros del Sistema ===\n");
    printf("1. Configurar duracion maxima de prestamo \n");
    printf("2. Configurar cantidad maxima de libros prestados por usuario al dia\n");
    printf("3. Configurar multa por devolucion tardia\n");
    printf("4. Volver al Menu de Configuracion del Sistema \n");
    printf("Seleccione una opcion: ");

}

void ejecutarMenuConfiguracionParametros(sqlite3* db){
    int opcion;
    char nuevoValor[50];
     do{
       mostrarMenuConfiguracionParametros();
        scanf("%d", &opcion);
        switch (opcion){
        case 1:
            printf("\nIngrese la nueva duración maxima de prestamo (en dias): ");
            scanf("%49s", nuevoValor);
            actualizarParametros("DuracionMaximaPrestamo", nuevoValor);
            
            break;
        case 2: 
            printf("\nIngrese la nueva cantidad maxima de libros prestados por usuario: ");
            scanf("%49s", nuevoValor);
            actualizarParametros("CantidadMaximaLibros", nuevoValor);
            
            break;
        case 3:
            printf("\nIngrese el nuevo numero de dias de sanción en caso de devolución tardia por dia: ");
            scanf("%49s", nuevoValor);
            actualizarParametros("MultaDevolucionTardia", nuevoValor);
            
            break;
        case 4: 
            printf("\nVolviendo al Menu de Configuracioon del Sistema...\n\n");
            break;
        default:
            printf("\nOpcion no valida. Por favor, seleccione una opcion valida.\n\n");
            break;
        }
    } while (opcion != 4);
    
}

void mostrarMenuConfiguracion(){
    printf("\n=== Configuracion Del Sistema ===\n");
    printf("1. Fichero de Configuracion\n");
    printf("2. Politicas y Parametros del Sistema\n");
    printf("3. Volver al Menu Principal\n");
    printf("Selccione una opcion: ");

}


// GENERACION DE INFORMES

// Función para ejecutar el menú de informes
void ejecutarMenuInformes(sqlite3* db) {
    int opcion;
    do {
        mostrarMenuInformes();
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                generarInformeUsuarios(db);
                break;
            case 2:
                generarInformePrestamos(db);
                break;
            case 3:
                generarInformeLibros(db);
                break;
            case 4:
                printf("\n");
                printf("Volviendo al Menu Principal...\n\n");
                break;
            default:
                printf("Opcion no valida. Por favor, seleccione una opción valida.\n");
                break;
        }
    } while (opcion != 4);
}

void mostrarMenuInformes() {
    printf("\n=== Menu de Informes ===\n");
    printf("1. Informe de Usuarios\n");
    printf("2. Informe de Prestamos\n");
    printf("3. Informe de Libros\n");
    printf("4. Volver al Menu Principal\n");
    printf("Seleccione una opcion: ");
}


// USUARIOS
// Función para contar el número de préstamos por usuario
int contarPrestamosPorUsuario(sqlite3* db, char* userID) {
    char sql[100];
    sprintf(sql, "SELECT COUNT(*) FROM Prestamo WHERE ID_Usuario = '%s'", userID);
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return count;
}

// Función para obtener el ID del usuario con más préstamos
char* obtenerUsuarioConMasPrestamos(sqlite3* db) {
    char sql[] = "SELECT ID_Usuario FROM Prestamo GROUP BY ID_Usuario ORDER BY COUNT(*) DESC LIMIT 1";
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    char* userID;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* id_text = sqlite3_column_text(stmt, 0);
        if (id_text) {
            userID = strdup((const char*)id_text); 
        }    
    }
    sqlite3_finalize(stmt);
    return (char*)userID;
}

// Función para contar el número de usuarios registrados
int contarUsuariosRegistrados(sqlite3* db) {
    char sql[] = "SELECT COUNT(*) FROM Usuario";
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return count;
}

// Función para generar el informe de usuarios
char * generarInformeUsuarios(sqlite3* db) {
    // Crear un buffer para almacenar el informe
    char* informe = malloc(TAMANIO_MAXIMO_INFORME * sizeof(char));
    if (informe == NULL) {
        // Manejar el error de asignación de memoria
        printf("Error: No se pudo asignar memoria para el informe.\n");
        return NULL;
    }
    
    // Inicializar el buffer del informe
    informe[0] = '\0';

    // Concatenar cada parte del informe al buffer
    strcat(informe, "\n=== Informe de Usuarios ===\n");

    // Usuario que no haya realizado ningún préstamo
    strcat(informe, "Usuarios sin prestamos:\n");


    //printf("\n=== Informe de Usuarios ===\n\n");
    // Usuario que no haya realizado ningún préstamo
    //printf("Usuarios sin prestamos:\n");
    char sqlUsuariosSinPrestamos[] = "SELECT * FROM Usuario WHERE ID_Usuario NOT IN (SELECT DISTINCT ID_Usuario FROM Prestamo)";
    sqlite3_stmt* stmtUsuariosSinPrestamos;
    int resultUsuariosSinPrestamos = sqlite3_prepare_v2(db, sqlUsuariosSinPrestamos, -1, &stmtUsuariosSinPrestamos, NULL);
    if (resultUsuariosSinPrestamos != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
    } else {
        while (sqlite3_step(stmtUsuariosSinPrestamos) == SQLITE_ROW) {
            char* userID = (char*)sqlite3_column_text(stmtUsuariosSinPrestamos, 0);
            char* userName = (char*)sqlite3_column_text(stmtUsuariosSinPrestamos, 1);
            char usuarioInfo[100];
            sprintf(usuarioInfo, "ID: %s \n, Nombre: %s\n", userID, userName);
            strcat(informe, usuarioInfo);
        }
    }
    sqlite3_finalize(stmtUsuariosSinPrestamos);

    // Usuario con más préstamos
    strcat(informe, "Usuario con mas prestamos:\n");
    //printf("\nUsuario con mas prestamos:\n");
    char* userIDConMasPrestamos = obtenerUsuarioConMasPrestamos(db);
    if (userIDConMasPrestamos != NULL) {
        int numPrestamos = contarPrestamosPorUsuario(db, userIDConMasPrestamos);
        char usuarioMasPrestamosInfo[50];
        sprintf(usuarioMasPrestamosInfo, "\tID: %s\n, \tNumero de prestamos: %d\n", (char*)userIDConMasPrestamos, numPrestamos);
        strcat(informe, usuarioMasPrestamosInfo);
    } else {
        strcat(informe, "No hay usuarios registrados.\n");
        //printf("No hay usuarios registrados.\n");
    }

    // Número de usuarios registrados
    char usuariosRegistradosInfo[50];
    sprintf(usuariosRegistradosInfo, "Numero total de usuarios registrados: %d\n", contarUsuariosRegistrados(db));
    strcat(informe, usuariosRegistradosInfo);
    //printf("\nNumero total de usuarios registrados: %d\n", contarUsuariosRegistrados(db));

    // Concatenar las opciones del menú al informe
    strcat(informe, "\n1. Volver al Menu de Informes\n");
    strcat(informe, "2. Volver al Menu Principal\n");
    strcat(informe, "Seleccione una opcion: \n");

    // Liberar memoria utilizada por variables temporales
    free(userIDConMasPrestamos);

    // Devolver la cadena de caracteres con el informe generado
    return informe;
/*
    int opcion;
    do {
        printf("1. Volver al Menu de Informes\n");
        printf("2. Volver al Menu Principal\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                printf("\nVolviendo al Menu de Informes...\n\n");
                return; // Sale de la función, volviendo al menú de informes
            case 2:
                printf("\n");
                printf("Volviendo al Menu Principal...\n\n");
                return; // Sale de la función, volviendo al menú principal
            default:
                printf("\nOpcion no valida. Por favor, seleccione una opcion valida.\n\n");
                break;
        }

    } while (opcion != 1 && opcion != 2);
*/
}



// PRESTAMOS

// Función para obtener el número total de préstamos activos
int obtenerNumeroPrestamosActivos(sqlite3* db) {
    char sql[] = "SELECT COUNT(*) FROM Prestamo WHERE Estado = 'activo'";
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return count;
}

// Función para obtener el ID del libro más prestado
int obtenerLibroMasPrestado(sqlite3* db) {
    char sql[] = "SELECT ISBN_Libro FROM Prestamo GROUP BY ISBN_Libro ORDER BY COUNT(*) DESC LIMIT 1";
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    int bookID = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        bookID = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return bookID;
}

// Función para obtener el ID del libro menos prestado
int obtenerLibroMenosPrestado(sqlite3* db) {
    char sql[] = "SELECT ISBN_Libro FROM Prestamo GROUP BY ISBN_Libro ORDER BY COUNT(*) ASC LIMIT 1";
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    int bookID = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        bookID = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return bookID;
}

// Función para generar el informe de préstamos
char* generarInformePrestamos(sqlite3* db) {
    // Crear un buffer para almacenar el informe
    char* informe = (char*)malloc(TAMANIO_MAXIMO_INFORME * sizeof(char));
    if (informe == NULL) {
        // Manejar el error de asignación de memoria
        printf("Error: No se pudo asignar memoria para el informe.\n");
        return NULL;
    }
    
    // Inicializar el buffer del informe
    informe[0] = '\0';

    // Concatenar cada parte del informe al buffer
    strcat(informe, "\n=== Informe de Prestamos ===\n");
    
    //printf("\n=== Informe de Prestamos ===\n");

    // Número total de préstamos activos
    int numPrestamosActivos = obtenerNumeroPrestamosActivos(db);
    char prestamosActivosInfo[50];
    sprintf(prestamosActivosInfo, "Numero total de prestamos activos: %d\n", numPrestamosActivos);
    strcat(informe, prestamosActivosInfo);
    //printf("Numero total de prestamos activos: %d\n", numPrestamosActivos);

    // Libro más prestado
    int libroMasPrestadoID = obtenerLibroMasPrestado(db);
    if (libroMasPrestadoID != -1) {
        char libroMasPrestadoInfo[50];
        sprintf(libroMasPrestadoInfo, "ISBN del libro más prestado: %d\n", libroMasPrestadoID);
        strcat(informe, libroMasPrestadoInfo);
        //printf("isbn del libro más prestado: %d\n", libroMasPrestadoID);
    } else {
        strcat(informe, "No hay libros prestados.\n");
        //printf("No hay libros prestados.\n");
    }

    // Libro menos prestado
    int libroMenosPrestadoID = obtenerLibroMenosPrestado(db);
    if (libroMenosPrestadoID != -1) {
        char libroMenosPrestadoInfo[50];
        sprintf(libroMenosPrestadoInfo, "ISBN del libro menos prestado: %d\n", libroMenosPrestadoID);
        strcat(informe, libroMenosPrestadoInfo);
        //printf("ISBN del libro menos prestado: %d\n", libroMenosPrestadoID);
    } else {
        strcat(informe, "No hay libros prestados.\n");
        //printf("No hay libros prestados.\n");
    }

    // Concatenar las opciones del menú al informe
    strcat(informe, "\n1. Volver al Menu de Informes\n");
    strcat(informe, "2. Volver al Menu Principal\n");
    strcat(informe, "Seleccione una opcion: \n");

    // Devolver la cadena de caracteres con el informe generado
    return informe;

    /*
    int opcion;
    do {
        printf("1. Volver al Menu de Informes\n");
        printf("2. Volver al Menu Principal\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                printf("\nVolviendo al Menu de Informes...\n\n");
                return; // Sale de la función, volviendo al menú de informes
            case 2:
                printf("\n");
                printf("Volviendo al Menu Principal...\n\n");
                return; // Sale de la función, volviendo al menú principal
            default:
                printf("\nOpcion no valida. Por favor, seleccione una opcion valida.\n\n");
                break;
        }
    } while (opcion != 1 && opcion != 2);
    */
}


// LIBROS

// Función para obtener el número de libros que no se han prestado
int obtenerNumLibrosNoPrestados(sqlite3* db) {
    char sql[] = "SELECT COUNT(*) FROM Libro WHERE numEjemplares > 0";
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return count;
}

// Función para obtener el número de libros prestados
int obtenerNumLibrosPrestados(sqlite3* db) {
    char sql[] = "SELECT COUNT(*) FROM Libro WHERE numEjemplares = 0";
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return count;
}

// Función para obtener el número de libros devueltos
int obtenerNumLibrosDevueltos(sqlite3* db) {
    char sql[] = "SELECT COUNT(*) FROM Prestamo WHERE Estado = 'devuelto'";
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return count;
}

// Función para generar el informe de libros
char* generarInformeLibros(sqlite3* db) {
    // Crear un buffer para almacenar el informe
    char* informe = (char*)malloc(TAMANIO_MAXIMO_INFORME * sizeof(char));
    if (informe == NULL) {
        // Manejar el error de asignación de memoria
        printf("Error: No se pudo asignar memoria para el informe.\n");
        return NULL;
    }
    
    // Inicializar el buffer del informe
    informe[0] = '\0';

    // Concatenar cada parte del informe al buffer
    strcat(informe, "\n=== Informe de Libros ===\n");

    //printf("\n=== Informe de Libros ===\n");

    // Número de libros que no se han prestado
    int numLibrosNoPrestados = obtenerNumLibrosNoPrestados(db);
    char librosNoPrestadosInfo[50];
    sprintf(librosNoPrestadosInfo, "Numero de libros que no se han prestado: %d\n", numLibrosNoPrestados);
    strcat(informe, librosNoPrestadosInfo);
    //printf("Numero de libros que no se han prestado: %d\n", numLibrosNoPrestados);

    // Número de libros prestados
    int numLibrosPrestados = obtenerNumLibrosPrestados(db);
    char librosPrestadosInfo[50];
    sprintf(librosPrestadosInfo, "Numero de libros prestados: %d\n", numLibrosPrestados);
    strcat(informe, librosPrestadosInfo);
    //printf("Numero de libros prestados: %d\n", numLibrosPrestados);

    // Número de libros devueltos
    int numLibrosDevueltos = obtenerNumLibrosDevueltos(db);
    char librosDevueltosInfo[50];
    sprintf(librosDevueltosInfo, "Numero de libros devueltos: %d\n", numLibrosDevueltos);
    strcat(informe, librosDevueltosInfo);
    //printf("Numero de libros devueltos: %d\n", numLibrosDevueltos);

    // Número total de libros
    int numTotalLibros = numLibrosNoPrestados + numLibrosPrestados + numLibrosDevueltos;
    char totalLibrosInfo[50];
    sprintf(totalLibrosInfo, "Numero total de libros: %d\n", numTotalLibros);
    strcat(informe, totalLibrosInfo);
    //printf("Numero total de libros: %d\n", numTotalLibros);

    // Concatenar las opciones del menú al informe
    strcat(informe, "\n1. Volver al Menu de Informes\n");
    strcat(informe, "2. Volver al Menu Principal\n");
    strcat(informe, "Seleccione una opcion: \n");

    // Devolver la cadena de caracteres con el informe generado
    return informe;
/*
    int opcion;
    do {
        printf("1. Volver al Menu de Informes\n");
        printf("2. Volver al Menu Principal\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                printf("\nVolviendo al Menu de Informes...\n\n");
                return; // Sale de la función, volviendo al menú de informes
            case 2:
                printf("\n");
                printf("Volviendo al Menu Principal...\n\n");
                return; // Sale de la función, volviendo al menú principal
            default:
                printf("\nOpcion no válida. Por favor, seleccione una opcion valida.\n\n");
                break;
        }
    } while (opcion != 1 && opcion != 2);
*/
}




// MENU PRINCIPAL

void ejecutarMenuConfiguracion(sqlite3* db){
    int opcion; 
    do{
        mostrarMenuConfiguracion();
        scanf("%d", &opcion);
        switch (opcion){
        case 1:
            ejecutarMenuConfiguracionFicheros(db);
            break;
        case 2: 
            ejecutarMenuConfiguracionParametros(db);
            break;
        case 3:
            printf("\n");
            printf("Volviendo al Menu Principal...\n\n");
            break;
        default:
            printf("Opcion no valida. Por favor, seleccione una opcion valida.\n");
            break;
        }
    } while (opcion != 3);
}

void mostrarMenuPrincipal() {
    printf("=== Menu Principal ===\n");
    printf("1. Gestion de Libros\n");
    printf("2. Gestion de Usuarios\n");
    printf("3. Gestion de Prestamos\n");
    printf("4. Configuracion del Sistema\n");
    printf("5. Generacion de Informes\n");
    printf("6. Salir\n");
    printf("Seleccione una opcion: ");
}

void ejecutarMenuPrincipal(sqlite3* db) {
    int opcion;
    do {
        mostrarMenuPrincipal();
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                ejecutarMenuLibros(db);
                break;
            case 2:
                ejecutarMenuUsuarios(db);
                break;
            case 3:
                ejecutarMenuPrestamos(db);
                break;
            case 4:
                ejecutarMenuConfiguracion(db);
                break;
            case 5:
                ejecutarMenuInformes(db);
                break;
            case 6:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opcion no valida. Por favor, seleccione una opcion valida.\n");
                break;
        }
    } while (opcion != 6);
}


 