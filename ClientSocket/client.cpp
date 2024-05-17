//para ejecutar: g++ client.cpp -o client.exe -lws2_32  
#include <iostream>
#include <string>
#include <winsock2.h>
#include <iomanip>

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
                    std::cout << "\n=== Menu de Gestion de Libros ===\n1. Agregar Nuevo Libro\n2. Mostrar Todos los Libros\n3. Buscar Libro\n4. Volver al Menu Principal\n";
                    std::cout << "Seleccione una opcion: ";
                    std::cin >> opcionLibros; // Captura la opción seleccionada por el usuario

                    std::string isbn, titulo, genero, autor, apellido;
                    int nEjemplares, aPubl, cod_E;
                    if(opcionLibros == '1') 
                    {
                        std::cin.ignore(); 
                        std::cout << "ISBN (13 numeros): ";
                        std::getline(std::cin, isbn);
                        if (isbn.length() != 13) {
                            std::cerr << "El ISBN debe tener 13 numeros.\n";
                            continue;
                        }
                        std::cout << "Titulo (max 100 caracteres): ";
                        std::getline(std::cin, titulo);
                        if (titulo.length() > 100) {
                            std::cerr << "El titulo no debe exceder 100 caracteres.\n";
                            continue;
                        }
                        std::cout << "Genero (max 100 caracteres): ";
                        std::getline(std::cin, genero);
                        if (genero.length() > 100) {
                            std::cerr << "El genero no debe exceder 100 caracteres.\n";
                            continue;
                        }
                        std::cout << "Nombre del Autor: ";
                        std::getline(std::cin, autor);
                        if (autor.length() > 100) {
                            std::cerr << "El nombre del autor no debe exceder 100 caracteres.\n";
                            continue;
                        }
                        std::cout << "Apellido del Autor: ";
                        std::getline(std::cin, apellido);
                        if (apellido.length() > 100) {
                            std::cerr << "El apellido del autor no debe exceder 100 caracteres.\n";
                            continue;
                        }
                        std::cout << "Numero de Ejemplares: ";
                        std::cin >> nEjemplares;
                        std::cout << "Anyo de Publicacion: ";
                        std::cin >> aPubl;
                        std::cout << "Codigo Editorial: ";
                        std::cin >> cod_E;

                        std::string bookDetails = "AgregarLibro|" + isbn + "|" + titulo + "|" + genero + "|" + autor + "|" + apellido + "|" + std::to_string(nEjemplares) + "|" + std::to_string(aPubl) + "|" + std::to_string(cod_E);

                        send(s, bookDetails.c_str(), bookDetails.length() + 1, 0);  
                        std::cout << "\nDetalles del libro enviados al servidor.\n";

                        int bytesReceived = recv(s, recvBuff, sizeof(recvBuff) - 1, 0);
                        if (bytesReceived > 0) {
                            recvBuff[bytesReceived] = '\0';
                            std::cout << "\nRespuesta del servidor: " << recvBuff << "\n";
                        }                   
                    }
                    if(opcionLibros == '2')
                    {
                        //Codigo para leer libros
                        strcpy(sendBuff, "LeerLibros");
                        send(s, sendBuff, sizeof(sendBuff), 0); 

                        // Recibir y mostrar los datos de los libros del servidor
                        int bytesReceived = recv(s, recvBuff, sizeof(recvBuff) - 1, 0);
                        if (bytesReceived > 0) {
                            recvBuff[bytesReceived] = '\0';
                            std::cout << "\nDatos de libros recibidos del servidor:\n" << recvBuff << std::endl;
                        } else {
                            std::cerr << "\nError al recibir datos de libros del servidor.\n";
                        }
                    } 
                    else if(opcionLibros == '3')
                    {
                        //Codigo para buscar un libro
                        std::string terminoBusqueda;
                        std::cout << "Ingrese el termino de busqueda (titulo, genero o autor): ";
                        std::getline(std::cin, terminoBusqueda);

                        // Enviar el término de búsqueda al servidor
                        send(s, terminoBusqueda.c_str(), terminoBusqueda.length() + 1, 0);

                        // Recibir y mostrar los resultados de la búsqueda del servidor
                        int bytesReceived = recv(s, recvBuff, sizeof(recvBuff), 0);
                        if (bytesReceived > 0) {
                            recvBuff[bytesReceived] = '\0';
                            std::cout << "\nResultados de la busqueda:\n" << recvBuff << std::endl;
                        } else {
                            std::cerr << "\nError al recibir resultados de busqueda del servidor.\n";
                        }
                    } 
                    else if(opcionLibros == '4')
                    {
                        //Codigo para volver al menu principal
                        break;
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
            std::cout << "\n===Menu de Gestion de Usuarios ===\n1. Agregar Nuevo Usuario\n2. Mostrar todos los Usuarios\n3. Buscar Usuario\n4. Editar Usuario\n5. Borrar Usuario\n6. Volver al Menu Principal\n";
            std::cout << "Seleccione una opcion: ";
            std::cin >> opcionUsuarios;

            std::string ID_Usuario, nombreU, apellidoU, correo, contrasenya;
            if (opcionUsuarios == '1')
            {
                // codigo para agregar nuevo usuario
                std::cin.ignore();
                std::cout << "ID: ";
                std::getline(std::cin, ID_Usuario);
                std::cout << "Nombre: ";
                std::getline(std::cin, nombreU);
                std::cout << "Apellido: ";
                std::getline(std::cin, apellidoU);
                std::cout << "Correo electronico: ";
                std::getline(std::cin, correo);
                std::cout << "Contraseña: ";
                std::getline(std::cin, contrasenya);

                std::string mensaje = "AgregarUsuario|" + ID_Usuario + "|" + nombreU + "|" + apellidoU + "|" + correo + "|" + contrasenya;

                // enviar mensaje de "AgregarUsuario" al servidor
                send(s, mensaje.c_str(), mensaje.length() +1, 0);
                std::cout << "\nDetalles del usuario enviados al servidor\n";

                // esperar la respuesta del servidor
                int bytesReceived = recv(s, recvBuff, sizeof(recvBuff)-1, 0);
                if(bytesReceived > 0) {
                    recvBuff[bytesReceived] = '\0';
                    std::cout << "Respuesta del servidor: " << recvBuff << "\n";
                }
            }
            else if(opcionUsuarios == '2')
            {
                // codigo para mostrar todos los usuarios
                // enviar mensaje de "MostrarUsuarios" al servidor
                printf("\n");
                std::cout << "\n======================== Usuarios en la Biblioteca ===============================" << std::endl;
                std::cout << std::left << std::setw(10) << "ID" << std::setw(10) << "Nombre" << std::setw(20) << "Apellido" << std::setw(25) << "Correo" << std::setw(15) << "Contrasenya" << std::endl;               
                std::cout << "==================================================================================" << std::endl;
                strcpy(sendBuff, "MostrarUsuarios");
                send(s, sendBuff, sizeof(sendBuff), 0);

                int bytesReceived = recv(s, recvBuff, sizeof(recvBuff)-1, 0);
                if(bytesReceived > 0) {
                    recvBuff[bytesReceived] = '\0';
                    std::cout << recvBuff << "\nDatos de usuarios recividos del servidor\n"  << std::endl;
                } else {
                    std::cerr << "\nError al recibir datos de libros del servidor.\n";
                }
            }
            else if(opcionUsuarios == '3')
            {
                std::string termino;
                // codigo para buscar usuario
                std::cout << "Ingrese el ID o nombre del usuario: ";
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
                    std::cout << "Ingrese el ID del libro asociado al prestamo que desea renovar: ";
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
