// IMPORTANT: Winsock Library ("ws2_32") should be linked

#include <stdio.h>
#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000
#define MAX_LINE 10


void clearIfNeeded(char *str, int max_line)
{
	// Limpia los caracteres de más introducidos
	if ((strlen(str) == max_line-1) && (str[max_line-2] != '\n'))
		while (getchar() != '\n');
}

char mostrarMenu()
{
	printf("1. CargarUsuarios\n");
	printf("2. NuevoUsuario\n");
	printf("3. EliminarUsuario\n");
	printf("Pulsar 'q' para salir\n");
	printf("\n");
	printf("Opcion: ");
	fflush(stdout);
	char linea[MAX_LINE];
	fgets(linea, MAX_LINE, stdin);
	clearIfNeeded(linea, MAX_LINE);
	return *linea;
}

int main(int argc, char *argv[]) {

	WSADATA wsaData;
	SOCKET s;
	struct sockaddr_in server;
	char sendBuff[512], recvBuff[512];

	printf("\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("Failed. Error Code : %d", WSAGetLastError());
		return -1;
	}

	printf("Initialised.\n");

	//SOCKET creation
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	printf("Socket created.\n");

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	//CONNECT to remote server
	if (connect(s, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
		printf("Connection error: %d", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		return -1;
	}

	printf("Connection stablished with: %s (%d)\n", inet_ntoa(server.sin_addr),
			ntohs(server.sin_port));


	char opcion;
	do{
		opcion = mostrarMenu();
		switch (opcion){
			case '1':
				printf("Mostrar Usuarios \n");
				strcpy(sendBuff, "PedirUsuarios");
				send(s, sendBuff, sizeof(sendBuff), 0);
				recv(s, recvBuff, sizeof(recvBuff), 0);
				printf("Usuarios: %s \n", recvBuff);

				break;
			case '2':
				break;
			case '3':
				printf("Eliminar Usuarios \n");
				strcpy(sendBuff, "EliminarUsuario");
				send(s, sendBuff, sizeof(sendBuff), 0);
				strcpy(sendBuff, "01");
				send(s, sendBuff, sizeof(sendBuff), 0);
				break;
		}
	}while(opcion != 'q');

	printf("Cerrando socket... \n");
	strcpy(sendBuff, "Bye");
	send(s, sendBuff, sizeof(sendBuff), 0);

	return 0;

	// CLOSING the socket and cleaning Winsock...
	closesocket(s);
	WSACleanup();

	return 0;
}