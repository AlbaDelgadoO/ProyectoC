#include <iostream>
#include <string>
#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000
#define MAX_LINE 10

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
            std::cout << "Gestion de Libros \n";
            strcpy(sendBuff, "PedirUsuarios");
            send(s, sendBuff, sizeof(sendBuff), 0);
            recv(s, recvBuff, sizeof(recvBuff), 0);
            std::cout << "Usuarios: " << recvBuff << "\n";
            break;
        case '2':
            std::cout << "Gestion de Usuarios \n";
            strcpy(sendBuff, "PedirUsuarios");
            send(s, sendBuff, sizeof(sendBuff), 0);
            recv(s, recvBuff, sizeof(recvBuff), 0);
            std::cout << "Usuarios: " << recvBuff << "\n";
            break;
        case '3':
            std::cout << "Gestion de Prestamos \n";
            strcpy(sendBuff, "EliminarUsuario");
            send(s, sendBuff, sizeof(sendBuff), 0);
            strcpy(sendBuff, "01");
            send(s, sendBuff, sizeof(sendBuff), 0);
            break;

        case '4':
            std::cout << "Configuracion del Sistema \n";
            strcpy(sendBuff, "EliminarUsuario");
            send(s, sendBuff, sizeof(sendBuff), 0);
            strcpy(sendBuff, "01");
            send(s, sendBuff, sizeof(sendBuff), 0);
            break;

        case '5':
            std::cout << "Generacion de Informes \n";
            strcpy(sendBuff, "GenerarInforme");
            send(s, sendBuff, sizeof(sendBuff), 0);
            strcpy(sendBuff, "01");
            send(s, sendBuff, sizeof(sendBuff), 0);
            break;
        }
    } while (opcion != 'q');

    std::cout << "Cerrando socket... \n";
    strcpy(sendBuff, "Bye");
    send(s, sendBuff, sizeof(sendBuff), 0);

    // CLOSING the socket and cleaning Winsock...
    closesocket(s);
    WSACleanup();

    return 0;
}
