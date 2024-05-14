// IMPORTANT: Winsock Library ("ws2_32") should be linked
//para ejecutar: gcc server.c ../*.c -o server.exe -lws2_32
#include <stdio.h>
#include <winsock2.h>
#include "../menu.h"
#include "../dataBase.h"
#include "../sqlite3.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

int main(int argc, char *argv[]) {

	// Inicializar la base de datos
	sqlite3* db;
    db = inicializarBaseDatos("biblioteca.db");
	// Crear tabla de libros si no existe
    crearTablaLibro(db);
    // Crear tabla usuario
    crearTablaUsuario(db);
    //Crear tabla prestamo
    crearTablaPrestamo(db);
    // Crear tabla Autor
    crearTablaAutor(db);

	WSADATA wsaData;
	SOCKET conn_socket;
	SOCKET comm_socket;
	struct sockaddr_in server;
	struct sockaddr_in client;
	char sendBuff[512], recvBuff[512];

	printf("\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("Failed. Error Code : %d", WSAGetLastError());
		return -1;
	}

	printf("Initialised.\n");

	//SOCKET creation
	if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	printf("Socket created.\n");

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	//BIND (the IP/port with socket)
	if (bind(conn_socket, (struct sockaddr*) &server,
			sizeof(server)) == SOCKET_ERROR) {
		printf("Bind failed with error code: %d", WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	printf("Bind done.\n");

	//LISTEN to incoming connections (socket server moves to listening mode)
	if (listen(conn_socket, 1) == SOCKET_ERROR) {
		printf("Listen failed with error code: %d", WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	//ACCEPT incoming connections (server keeps waiting for them)
	printf("Waiting for incoming connections...\n");
	int stsize = sizeof(struct sockaddr);
	comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize);
	// Using comm_socket is able to send/receive data to/from connected client
	if (comm_socket == INVALID_SOCKET) {
		printf("accept failed with error code : %d", WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}
	printf("Incomming connection from: %s (%d)\n", inet_ntoa(client.sin_addr),
			ntohs(client.sin_port));

	// Closing the listening sockets (is not going to be used anymore)
	closesocket(conn_socket);

	//SEND and RECEIVE data
	printf("Waiting for incoming messages from client... \n");
	do {
		int bytes = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
		if (bytes > 0) {
			//GESTION DE LIBROS
			if(strcmp(recvBuff, "AgregarLibro") == 0)
			{
				//Codigo para agregar un libro
				// Recibir los detalles del libro del cliente
				char isbn[15], titulo[100], genero[100], autor[100], apellido[100], nEjemplaresStr[20], aPublStr[20], cod_EStr[20];
				int nEjemplares, aPubl, cod_E;

				// Recibir los detalles del libro del cliente
				recv(comm_socket, isbn, sizeof(isbn), 0);
				recv(comm_socket, titulo, sizeof(titulo), 0);
				recv(comm_socket, genero, sizeof(genero), 0);
				recv(comm_socket, autor, sizeof(autor), 0);
				recv(comm_socket, apellido, sizeof(apellido), 0);
				recv(comm_socket, nEjemplaresStr, sizeof(nEjemplaresStr), 0);
				recv(comm_socket, aPublStr, sizeof(aPublStr), 0);
				recv(comm_socket, cod_EStr, sizeof(cod_EStr), 0);

				// Convertir las cadenas a enteros
				nEjemplares = atoi(nEjemplaresStr);
				aPubl = atoi(aPublStr);
				cod_E = atoi(cod_EStr);

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
				// Enviar confirmación al cliente de que el libro ha sido insertado
				const char* confirmacion = "Libro insertado correctamente";
				send(comm_socket, confirmacion, strlen(confirmacion), 0);
			}
			// GESTIÓN DE PRESTAMOS
			if (strcmp(recvBuff, "AgregarPrestamo") == 0) {
				// Código para agregar un nuevo préstamo
				char idLibro[20], idUsuario[20], fechaVencimiento[20];

				// Recibir los detalles del préstamo del cliente
				recv(comm_socket, idLibro, sizeof(idLibro), 0);
				recv(comm_socket, idUsuario, sizeof(idUsuario), 0);
				recv(comm_socket, fechaVencimiento, sizeof(fechaVencimiento), 0);

				// Insertar el nuevo préstamo en la base de datos
				// Aquí debes implementar la lógica para insertar el préstamo en la base de datos
				// Puedes utilizar las funciones de tu API de base de datos (SQLite en este caso)
				// No tengo acceso a tu implementación específica, así que no puedo proporcionar el código exacto
				// Pero deberías realizar una inserción en la tabla Prestamo con los valores recibidos del cliente

				// Enviar confirmación al cliente de que el préstamo ha sido insertado
				const char* confirmacion = "Prestamo insertado correctamente";
				send(comm_socket, confirmacion, strlen(confirmacion), 0);
			}

			if(strcmp(recvBuff, "Bye") == 0)
			{
				//Llamada a bbdd
				//devolver usuarios
			}

			//GENERACION DE INFORMES
			// Esperar a que llegue un mensaje del cliente
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);

			// Procesar la solicitud del cliente
			else if (strcmp(recvBuff, "informeUsuario") == 0) {
				// Lógica para generar el informe de usuarios
				generarInformeUsuarios(db);
				// Enviar confirmación al cliente de que el libro ha sido insertado
				const char* confirmacion = "Informe de usuario generado correctamente";
				send(comm_socket, confirmacion, strlen(confirmacion), 0);
			} else if (strcmp(recvBuff, "informePrestamos") == 0) {
				// Lógica para generar el informe de préstamos
				generarInformePrestamos(db);
				const char* confirmacion = "Informe de prestamos generado correctamente";
				send(comm_socket, confirmacion, strlen(confirmacion), 0);
			} else if (strcmp(recvBuff, "informeLibros") == 0) {
				// Lógica para generar el informe de libros
				generarInformeLibros(db);
				const char* confirmacion = "Informe de usuario libros correctamente";
				send(comm_socket, confirmacion, strlen(confirmacion), 0);
			} else {
				// Mensaje de solicitud no reconocida
				const char* mensajeError = "Solicitud no reconocida";
				send(comm_socket, mensajeError, strlen(mensajeError), 0);
			}
		
			if (strcmp(recvBuff, "Bye") == 0)
				break;
		}
	} while (1);

	// CLOSING the sockets and cleaning Winsock...
	closesocket(comm_socket);
	WSACleanup();

	return 0;
}

