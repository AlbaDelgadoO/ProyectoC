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
			if (strncmp(recvBuff, "AgregarLibro", 12) == 0) {
				char *token;
				token = strtok(recvBuff, "|");
				token = strtok(NULL, "|"); // Skip "AgregarLibro"

				char isbn[14], titulo[101], genero[101], autor[101], apellido[101];
				int nEjemplares, aPubl, cod_E;

				strncpy(isbn, token, 13); isbn[13] = '\0'; token = strtok(NULL, "|");
				strncpy(titulo, token, 100); titulo[100] = '\0'; token = strtok(NULL, "|");
				strncpy(genero, token, 100); genero[100] = '\0'; token = strtok(NULL, "|");
				strncpy(autor, token, 100); autor[100] = '\0'; token = strtok(NULL, "|");
				strncpy(apellido, token, 100); apellido[100] = '\0'; token = strtok(NULL, "|");
				nEjemplares = atoi(token); token = strtok(NULL, "|");
				aPubl = atoi(token); token = strtok(NULL, "|");
				cod_E = atoi(token);

				// Crear una instancia de Libro con los detalles ingresados por el usuario
				Libro nuevoLibro;
				strncpy(nuevoLibro.ISBN, isbn, 14);
				strncpy(nuevoLibro.titulo, titulo, 101);
				strncpy(nuevoLibro.genero, genero, 101);
				strncpy(nuevoLibro.nom_autor, autor, 101);
				strncpy(nuevoLibro.apellido_autor, apellido, 101);
				nuevoLibro.numEjemplares = nEjemplares;
				nuevoLibro.anyoPublicacion = aPubl;
				nuevoLibro.cod_Editorial = cod_E;

				// Insertar el nuevo libro en la base de datos
				insertarLibro(db, nuevoLibro);

				// Enviar confirmación al cliente de que el libro ha sido insertado
				const char* confirmacion = "Libro insertado correctamente";
				send(comm_socket, confirmacion, strlen(confirmacion) + 1, 0);  // Add +1 to include the null terminator
			}
			else if (strcmp(recvBuff, "LeerLibros") == 0) 
			{
				char* librosData = obtenerDatosLibros(db); 
				send(comm_socket, librosData, strlen(librosData) + 1, 0); 
				free(librosData); // Liberar la memoria utilizada
			}
			else if (strcmp(recvBuff, "BuscarLibro") == 0) 
			{
				// Recibir el término de búsqueda del cliente
				char terminoBusqueda[100];
				recv(comm_socket, terminoBusqueda, sizeof(terminoBusqueda), 0);

				// Buscar el libro en la base de datos y enviar los detalles al cliente
				char* librosData = obtenerLibro(db, terminoBusqueda);
				send(comm_socket, librosData, strlen(librosData) + 1, 0); // Enviar los detalles de los libros al cliente
				free(librosData); // Liberar la memoria utilizada
			}

			// GESTIÓN DE USUARIOS
			else if(strncmp(recvBuff, "AgregarUsuario", 14) == 0) {
				// Código para agregar un nuevo usuario
				char* token;
				token = strtok(recvBuff, "|");
				token = strtok(NULL, "|");
				char ID_Usuario[50], nombreU[50], apellidoU[50], correo[100], contrasenya[50];

				strncpy(ID_Usuario, token,49); ID_Usuario[49] = '\0'; token = strtok(NULL, "|");
				strncpy(nombreU, token,49); nombreU[49] = '\0'; token = strtok(NULL, "|");
				strncpy(apellidoU, token,49); apellidoU[49] = '\0'; token = strtok(NULL, "|");
				strncpy(correo, token,99); correo[99] = '\0'; token = strtok(NULL, "|");
				strncpy(contrasenya, token,49); contrasenya[49] = '\0'; token = strtok(NULL, "|");



				// Crear un nuevo usuario
				Usuario nuevoUsuario;
				strncpy(nuevoUsuario.ID_Usuario, ID_Usuario,50);
				strncpy(nuevoUsuario.nombreU, nombreU,50);
				strncpy(nuevoUsuario.apellidoU, apellidoU, 50);
				strncpy(nuevoUsuario.correo, correo,100);
				strncpy(nuevoUsuario.contrasenya, contrasenya, 50);

				// Insertar usuario en la BD
				insertarUsuario(db, nuevoUsuario);

				// Enviar confirmación al cliente de que el préstamo ha sido insertado
				const char* confirmacion = "Usuario insertado correctamente";
				send(comm_socket, confirmacion, strlen(confirmacion) + 1, 0);
			}
			else if(strcmp(recvBuff, "MostrarUsuarios") == 0) {
				char* usuarios = obtenerUsuarios(db);
				send(comm_socket, usuarios, strlen(usuarios)+1, 0);
				free(usuarios);
			}
			else if(strcmp(recvBuff, "BuscarUsuario") == 0) {
				// Código para agregar un nuevo usuario
				char termino[50];

				// Recibir los detalles del usuario
				recv(comm_socket, termino, sizeof(termino), 0);

				// Buscar usuario en la BD
				buscarUsuariosDB(db, termino);

				// Enviar confirmación al cliente de que el préstamo ha sido insertado
				//const char* confirmacion = "Resultados mostrados correctamente";
				//send(comm_socket, confirmacion, strlen(confirmacion), 0);
			}


			// GESTIÓN DE PRESTAMOS
			else if (strcmp(recvBuff, "AgregarPrestamo") == 0) {
				// Código para agregar un nuevo préstamo
				char idLibro[13], idUsuario[13], fechaVencimiento[20];

				// Recibir los detalles del préstamo del cliente
				recv(comm_socket, idLibro, sizeof(idLibro), 0);
				recv(comm_socket, idUsuario, sizeof(idUsuario), 0);
				recv(comm_socket, fechaVencimiento, sizeof(fechaVencimiento), 0);


				// Crear una instancia de Prestamo con los detalles ingresados por el usuario
				Prestamo nuevoPrestamo;
				strcpy(nuevoPrestamo.ISBN, idLibro);
				strcpy(nuevoPrestamo.ID_Usuario, idUsuario);
				strcpy(nuevoPrestamo.fechaDevolucion, fechaVencimiento);

				// Insertar el nuevo prestamo en la base de datos
				insertarPrestamo(db, nuevoPrestamo);

				// Enviar confirmación al cliente de que el préstamo ha sido insertado
				const char* confirmacion = "Prestamo insertado correctamente";
				send(comm_socket, confirmacion, strlen(confirmacion), 0);
			}
			else if (strcmp(recvBuff, "RenovarPrestamo") == 0) {
				// Código para renovar un préstamo existente
				char idLibro[20];

				// Recibir el ID del libro del cliente
				recv(comm_socket, idLibro, sizeof(idLibro), 0);

				// Renovar el préstamo asociado al libro en la base de datos
				renovarPrestamoDB(db, idLibro);

				// Enviar confirmación al cliente de que el préstamo ha sido renovado
				const char* confirmacion = "Prestamo renovado correctamente";
				send(comm_socket, confirmacion, strlen(confirmacion), 0);
			}
			else if (strcmp(recvBuff, "DevolverLibro") == 0) {
				// Código para registrar la devolución de un libro
				char idLibro[15];

				// Recibir el ID del libro del cliente
				recv(comm_socket, idLibro, sizeof(idLibro), 0);

				// Llamar a la función en dataBase.c para registrar la devolución del libro
				registrarDevolucionLibro(db, idLibro);

				// Enviar confirmación al cliente de que la devolución ha sido registrada
				const char* confirmacion = "Devolucion registrada correctamente";
				send(comm_socket, confirmacion, strlen(confirmacion), 0);
			}


			//GENERACION DE INFORMES
			// Esperar a que llegue un mensaje del cliente
			//recv(comm_socket, recvBuff, sizeof(recvBuff), 0);

			// Procesar la solicitud del cliente
			else if (strcmp(recvBuff, "informeUsuario") == 0) {
				// Lógica para generar el informe de usuarios
				//generarInformeUsuarios(db);
				// Enviar confirmación al cliente de que el libro ha sido insertado
				//const char* confirmacion = "Informe de usuario generado correctamente";
				//send(comm_socket, confirmacion, strlen(confirmacion), 0);

				printf("Cargando");
				// Lógica para generar el informe de usuarios
				char* informeUsuarios = generarInformeUsuarios(db);
				// Enviar informe generado al cliente
				send(comm_socket, informeUsuarios, strlen(informeUsuarios), 0);
				free(informeUsuarios); // Liberar memoria del informe generado
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

