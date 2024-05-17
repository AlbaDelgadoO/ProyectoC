//para ejecutar: g++ client.cpp -o client.exe -lws2_32  
#include <iostream>
#include <string>
#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000
#define MAX_LINE 100

void clearIfNeeded(std::string &str, int max_line)
{
    // Limpia los caracteres de más introducidos
    if ((str.length() == max_line - 1) && (str[max_line - 2] != '\n'))
        while (std::cin.get() != '\n');
}

char mostrarMenu()
{
    std::cout << "1. Gestion de Libros\n";
    std::cout << "2. Gestion de Usuarios\n";
    std::cout << "3. Gestion de Prestamos\n";
    std::cout << "4. Configuracion del Sistema\n";
    std::cout << "5. Generacion de Informes\n";
    std::cout << "Pulsar 'q' para salir\n";
    std::cout << "\n";
    std::cout << "Opcion: ";
    std::cout.flush();
    char linea[MAX_LINE];
    std::cin.getline(linea, MAX_LINE);
    std::string opcion_str(linea); // Convertir char[] a std::string
    clearIfNeeded(opcion_str, MAX_LINE); // Pasar std::string a clearIfNeeded
    return opcion_str[0];
}

int main(int argc, char *argv[]) {

    WSADATA wsaData;
    SOCKET s;
    struct sockaddr_in server;
    char sendBuff[512], recvBuff[512];

    std::cout << "\nInitialising Winsock...\n";
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cout << "Failed. Error Code : " << WSAGetLastError();
        return -1;
    }

    std::cout << "Initialised.\n";

    //SOCKET creation
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        std::cout << "Could not create socket : " << WSAGetLastError();
        WSACleanup();
        return -1;
    }

    std::cout << "Socket created.\n";

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    //CONNECT to remote server
    if (connect(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        std::cout << "Connection error: " << WSAGetLastError();
        closesocket(s);
        WSACleanup();
        return -1;
    }

    std::cout << "Connection stablished with: " << inet_ntoa(server.sin_addr) << " (" << ntohs(server.sin_port) << ")\n";

    char opcion;
    do {
        opcion = mostrarMenu();
        switch (opcion)
        {
        case '1':
                char opcionLibros;
                do{            
                    std::cout << "Gestion de Libros \n";
                    std::cout << "\n=== Menu de Gestion de Libros ===\n1. Agregar Nuevo Libro\n2. Mostrar Todos los Libros\n3. Buscar Libro\n4. Volver al Menu Principal\n";
                    std::cout << "Seleccione una opcion: ";
                    std::cin >> opcionLibros; // Captura la opción seleccionada por el usuario

                    std::string isbn, titulo, genero, autor, apellido;
                    int nEjemplares, aPubl, cod_E;
                    if(opcionLibros == '1') 
                    {
                        //Codigo para agregar un libro
                        std::cout << "Ingrese los detalles del libro:\n";
                        std::cout << "ISBN: ";
                        std::getline(std::cin >> std::ws, isbn);
                        clearIfNeeded(isbn, MAX_LINE);
                        std::cout << "Titulo: ";
                        std::getline(std::cin >> std::ws, titulo);
                        clearIfNeeded(titulo, MAX_LINE);
                        std::cout << "Genero: ";
                        std::getline(std::cin >> std::ws, genero);
                        clearIfNeeded(genero, MAX_LINE);
                        std::cout << "Nombre del autor: ";
                        std::getline(std::cin >> std::ws, autor);
                        clearIfNeeded(autor, MAX_LINE);
                        std::cout << "Apellido del autor: ";
                        std::getline(std::cin >> std::ws, apellido);
                        clearIfNeeded(apellido, MAX_LINE);
                        std::cout << "Numero de ejemplares: ";
                        std::cin >> nEjemplares;
                        std::cout << "Anyo de publicacion: ";
                        std::cin >> aPubl;
                        std::cout << "Codigo de editorial: ";
                        std::cin >> cod_E;

                        // Enviar el mensaje "AgregarLibro" al servidor
                        strcpy(sendBuff, "AgregarLibro");
                        send(s, sendBuff, sizeof(sendBuff), 0);

                        // Enviar los detalles del libro al servidor
                        send(s, isbn.c_str(), strlen(isbn.c_str()), 0);
                        send(s, titulo.c_str(), strlen(titulo.c_str()), 0);
                        send(s, genero.c_str(), strlen(genero.c_str()), 0);
                        send(s, autor.c_str(), strlen(autor.c_str()), 0);
                        send(s, apellido.c_str(), strlen(apellido.c_str()), 0);
                        send(s, std::to_string(nEjemplares).c_str(), std::to_string(nEjemplares).length(), 0);
                        send(s, std::to_string(aPubl).c_str(), std::to_string(aPubl).length(), 0);
                        send(s, std::to_string(cod_E).c_str(), std::to_string(cod_E).length(), 0);

                        // Esperar la respuesta del servidor
                        recv(s, recvBuff, sizeof(recvBuff), 0);
                        std::cout << "Respuesta del servidor: " << recvBuff << "\n";
                    }
                    else if(opcionLibros == '2')
                    {
                        //Codigo para leer libros
                    } 
                    else if(opcionLibros == '3')
                    {
                        //Codigo para buscar un libro
                    } 
                    else if(opcionLibros == '4')
                    {
                        //Codigo para volver al menu principal
                    } 
                    else
                    {
                        std::cout << "Opcion invalida\n";
                    }
                }while(opcionLibros != '4');
                break;
        case '2':
            char opcionUsuarios;
            do {
            std::cout << "Gestion de Usuarios \n";
            std::cout << "\n===Menu de Gestion de Usuarios ===\n1. Agregar Nuevo Usuario\n2. Mostrar todos los Usuarios\n3. Buscar Usuario\n4. Editar Usuario\n5. Borrar Usuario\n6. Volver al Menu Principal";
            std::cout << "Seleccione una opcion: ";
            std::cin >> opcionUsuarios;

            std::string ID_Usuario, nombreU, apellidoU, correo, contrasenya;
            if (opcionUsuarios == '1')
            {
                // codigo para agregar nuevo usuario
                std::cout << "Ingrese los detalles del usuario:\n";
                std::cout << "ID: ";
                std::getline(std::cin >> std::ws, ID_Usuario);
                clearIfNeeded(ID_Usuario, MAX_LINE);
                std::cout << "Nombre: ";
                std::getline(std::cin >> std::ws, nombreU);
                clearIfNeeded(nombreU, MAX_LINE);
                std::cout << "Apellido: ";
                std::getline(std::cin >> std::ws, apellidoU);
                clearIfNeeded(apellidoU, MAX_LINE);
                std::cout << "Correo electronico: ";
                std::getline(std::cin >> std::ws, correo);
                clearIfNeeded(correo, MAX_LINE);
                std::cout << "Contraseña: ";
                std::getline(std::cin >> std::ws, contrasenya);
                clearIfNeeded(contrasenya, MAX_LINE);

                // enviar mensaje de "AgregarUsuario" al servidor
                strcpy(sendBuff, "AgregarUsuario");
                send(s, sendBuff, sizeof(sendBuff), 0);

                // enviar detalles del usuario al servidor
                send(s, ID_Usuario.c_str(), strlen(ID_Usuario.c_str()),0);
                send(s, nombreU.c_str(), strlen(ID_Usuario.c_str()),0);
                send(s, apellidoU.c_str(), strlen(ID_Usuario.c_str()),0);
                send(s, correo.c_str(), strlen(ID_Usuario.c_str()),0);
                send(s, contrasenya.c_str(), strlen(ID_Usuario.c_str()),0);

                // esperar la respuesta del servidor
                recv(s, recvBuff, sizeof(recvBuff), 0);
                std::cout << "Respuesta del servidor: " << recvBuff << "\n";
            }
            else if(opcionUsuarios == '2')
            {
                // codigo para mostrar todos los usuarios
                // enviar mensaje de "MostrarUsuarios" al servidor
                strcpy(sendBuff, "MostrarUsuarios");
                send(s, sendBuff, sizeof(sendBuff), 0);

                // esperar la respuesta del servidor
                recv(s, recvBuff, sizeof(recvBuff), 0);
                std::cout << "Respuesta del servidor: " << recvBuff << "\n";
            }
            else if(opcionUsuarios == '3')
            {
                std::string termino;
                // codigo para buscar usuario
                std:cout << "Ingrese el ID o nombre del usuario: ";
                std::getline(std::cin >> std::ws, termino);
                clearIfNeeded(termino, MAX_LINE);

                // enviar mensaje de "BuscarUsuario" al servidor
                strcpy(sendBuff, "BuscarUsuario");
                send(s, sendBuff, sizeof(sendBuff), 0);

                // enviar detalles del usuario al servidor
                send(s, termino.c_str(), strlen(termino.c_str()),0);

                // esperar la respuesta del servidor
                recv(s, recvBuff, sizeof(recvBuff), 0);
                std::cout << "Respuesta del servidor: " << recvBuff << "\n";
            }
            else if(opcionUsuarios == '4')
            {
                // codigo para editar usuario
            }
            else if(opcionUsuarios == '5')
            {
                // codigo para borrar usuario
            }
            else if(opcionUsuarios == '6')
            {
                // codigo para volver al menu principal
            }
            
            } while(opcionUsuarios != '6');
            break;
        case '3':
            std::cout << "Gestion de Prestamos \n";
            char opcionPrestamos;
            do {            
                std::cout << "\n=== Menu de Gestion de Prestamos ===\n";
                std::cout << "1. Agregar Nuevo Prestamo\n";
                std::cout << "2. Renovar Prestamo Existente\n";
                std::cout << "3. Registrar Devolucion de Libro\n";
                std::cout << "4. Gestionar Problemas de Prestamo\n";
                std::cout << "5. Volver al Menu Principal\n";
                std::cout << "Seleccione una opcion: ";
                std::cin >> opcionPrestamos; // Captura la opción seleccionada por el usuario

                if (opcionPrestamos == '1') {
                    // Código para agregar un nuevo préstamo
                    std::string idLibro, idUsuario, fechaVencimiento;

                    // Solicitar detalles del nuevo préstamo al usuario
                    std::cout << "Ingrese el ID del libro: ";
                    std::cin >> idLibro;
                    std::cout << "Ingrese el ID del usuario: ";
                    std::cin >> idUsuario;
                    std::cout << "Ingrese la fecha de vencimiento (AAAA-MM-DD): ";
                    std::cin >> fechaVencimiento;

                    // Enviar el mensaje "AgregarPrestamo" al servidor
                    strcpy(sendBuff, "AgregarPrestamo");
                    send(s, sendBuff, sizeof(sendBuff), 0);

                    // Enviar los detalles del préstamo al servidor
                    send(s, idLibro.c_str(), strlen(idLibro.c_str()), 0);
                    send(s, idUsuario.c_str(), strlen(idUsuario.c_str()), 0);
                    send(s, fechaVencimiento.c_str(), strlen(fechaVencimiento.c_str()), 0);

                    // Esperar la respuesta del servidor
                    recv(s, recvBuff, sizeof(recvBuff), 0);
                    std::cout << "Respuesta del servidor: " << recvBuff << "\n";
                } else if (opcionPrestamos == '2') {
                    // Código para renovar un préstamo existente
                    std::string idLibro;

                    // Solicitar al usuario el ID del libro asociado al préstamo que desea renovar
                    std::cout << "Ingrese el ID del libro asociado al préstamo que desea renovar: ";
                    std::cin >> idLibro;

                    // Enviar el mensaje "RenovarPrestamo" al servidor
                    strcpy(sendBuff, "RenovarPrestamo");
                    send(s, sendBuff, sizeof(sendBuff), 0);

                    // Enviar el ID del libro al servidor
                    send(s, idLibro.c_str(), strlen(idLibro.c_str()), 0);

                    // Esperar la respuesta del servidor
                    recv(s, recvBuff, sizeof(recvBuff), 0);
                    std::cout << "Respuesta del servidor: " << recvBuff << "\n";
                } else if (opcionPrestamos == '3') {
                    // Código para registrar la devolución de un libro
                    std::string idLibro;

                    // Solicitar al usuario el ID del libro que se está devolviendo
                    std::cout << "Ingrese el ID del libro que desea devolver: ";
                    std::cin >> idLibro;

                    // Enviar el mensaje "DevolverLibro" al servidor
                    strcpy(sendBuff, "DevolverLibro");
                    send(s, sendBuff, sizeof(sendBuff), 0);

                    // Enviar el ID del libro al servidor
                    send(s, idLibro.c_str(), strlen(idLibro.c_str()), 0);

                    // Esperar la respuesta del servidor
                    recv(s, recvBuff, sizeof(recvBuff), 0);
                    std::cout << "Respuesta del servidor: " << recvBuff << "\n";
                } else if (opcionPrestamos == '4') {
                    // Código para gestionar problemas de préstamo
                } else if (opcionPrestamos == '5') {
                    std::cout << "Volviendo al Menu Principal...\n";
                } else {
                    std::cout << "Opcion no valida. Por favor, seleccione una opcion valida.\n";
                }
            } while (opcionPrestamos != '5');
            break;

        case '4':
            std::cout << "Configuracion del Sistema \n";
            strcpy(sendBuff, "EliminarUsuario");
            send(s, sendBuff, sizeof(sendBuff), 0);
            strcpy(sendBuff, "01");
            send(s, sendBuff, sizeof(sendBuff), 0);
            break;

        case '5':
            char opcionInformes;
            char opcionMenuInforme;
            char continuar;
            do {            
                std::cout << "Generacion de Informes \n";
                std::cout << "\n=== Menu de Informes ===\n1. Informe de Usuarios\n2. Informe de Prestamos\n3. Informe de Libros\n4. Volver al Menu Principal\n";
                std::cout << "Seleccione una opcion: ";
                std::cin >> opcionInformes; // Captura la opción seleccionada por el usuario

                if(opcionInformes == '1') { 
                    // Enviar el mensaje "informeUsuario" al servidor
                    strcpy(sendBuff, "informeUsuario");
                    send(s, sendBuff, sizeof(sendBuff), 0);

                    // Esperar la respuesta del servidor
                    recv(s, recvBuff, sizeof(recvBuff), 0);
                    // Imprimir el informe en la consola del cliente
                    std::cout << "Informe de usuarios:\n" << recvBuff << "\n";    
                    
                } else if(opcionInformes == '2') {
                    // Enviar el mensaje "informePrestamos" al servidor
                    strcpy(sendBuff, "informePrestamos");
                    send(s, sendBuff, sizeof(sendBuff), 0);

                    // Esperar la respuesta del servidor
                    recv(s, recvBuff, sizeof(recvBuff), 0);
                    std::cout << "Informe de prestamos:\n" << recvBuff << "\n";

                } else if(opcionInformes == '3') {
                    // Enviar el mensaje "informeLibros" al servidor
                    strcpy(sendBuff, "informeLibros");
                    send(s, sendBuff, sizeof(sendBuff), 0);

                    // Esperar la respuesta del servidor
                    recv(s, recvBuff, sizeof(recvBuff), 0);
                    std::cout << "Informe de libros:\n" << recvBuff << "\n";

                } else if(opcionInformes == '4') {
                    // Código para volver al menú principal
                    std::cout << "Volviendo al Menu Principal...\n";

                } else {
                    std::cout << "Opcion invalida\n";
/*                    do {
                        std::cout << "1. Volver al Menu de Informes\n";
                        std::cout << "2. Volver al Menu Principal\n";
                        std::cout << "Seleccione una opcion: ";
                        std::cin >> opcionMenuInforme;

                        switch (opcionMenuInforme) {
                            case 1:
                                std::cout << "\nVolviendo al Menu de Informes...\n\n";
                                break;
                            case 2:
                                std::cout << "\nVolviendo al Menu Principal...\n\n";
                                break;
                            default:
                                std::cout << "\nOpcion no valida. Por favor, seleccione una opcion valida.\n\n";
                                break;
                        }
                    } while (opcionMenuInforme != 1 && opcionMenuInforme != 2);
*/                } 
            } while(opcionInformes != '4');
            break;
    
    }
    std::cout << "Cerrando socket... \n";
    strcpy(sendBuff, "Bye");
    send(s, sendBuff, sizeof(sendBuff), 0);

    // CLOSING the socket and cleaning Winsock...
    closesocket(s);
    WSACleanup();
    }while(opcion != 'q');

    return 0;
}
