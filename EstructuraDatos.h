#ifndef ESTRUCTURA_DATOS_H
#define ESTRUCTURA_DATOS_H

// Estructura para representar un libro
typedef struct {
    char ISBN[13];
    char titulo[100];
    char genero[50];
    char nom_autor[50];
    char apellido_autor[50];
    int numEjemplares;
    int anyoPublicacion;
    int cod_Editorial;
} Libro;

// Estructura para representar un autor
typedef struct {
    int ID_Autor;
    char nombreA[50];
    char apellidoA[50];
} Autor;

// Estructura para representar un usuario
typedef struct {
    char ID_Usuario[50];
    char nombreU[50];
    char apellidoU[50];
    char correo[100];
    char contrasenya[50];
} Usuario;

// Estructura para representar un préstamo
typedef struct {
    int ID_Prestamo;
    char fechaPrestado[20];
    char fechaDevolucion[20];
    char Estado[20];
    char ISBN[13];
    int ID_Usuario;
} Prestamo;


#endif /* ESTRUCTURA_DATOS_H */