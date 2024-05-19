#ifndef MENU_H
#define MENU_H

#include <winsock2.h>
#include <iomanip>


void crearFicheroLog();
void leerFicheroConfiguracion(char *recvBuff, SOCKET s);
void mostrarMenuConfiguracionFicheros();
void ejecutarMenuConfiguracionFicheros(char *sendBuff, char *recvBuff, SOCKET s);
void actualizarParametros(const char *clave, const char *nuevoValor);
void mostrarMenuConfiguracionParametros();
void ejecutarMenuConfiguracionParametros(char *sendBuff, char *recvBuff, SOCKET s);
void mostrarMenuConfiguracion();
void ejecutarMenuConfiguracion(char *sendBuff, char *recvBuff, SOCKET s);


#endif