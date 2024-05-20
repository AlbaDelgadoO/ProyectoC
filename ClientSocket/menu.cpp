#include <iostream>

#include <fstream>
#include <cstring>
#include <winsock2.h>
#include "menu.h"

void crearFicheroLog(){
    std::ofstream archivoLog("registro.log");
    if(!archivoLog){
        std::cerr<<"Error al crear el archivo de resgistro"<<std::endl;
        return;
    }
    archivoLog<<"Inicio del registro..."<<std::endl;
    archivoLog.close();
    std::cout <<"Archivo de resgistro creado correctamente";
    
}

void leerFicheroConfiguracion(char *recvBuff, SOCKET s){
    int n;
	n = recv(s,recvBuff,100,0);
	recvBuff[n]='\0';
    while(strncmp(recvBuff,"FIN",3)!=0){
        std::cout<<recvBuff<<std::endl;
        recvBuff[0]='\0';
        n=recv(s,recvBuff,100,0);
        recvBuff[n]='\0';
    }
}
void mostrarMenuConfiguracionFicheros(){
    std::cout<<"\n=== Fichero de Configuracion ===\n1. Leer Fichero de Configuracion con las rutas de los archivos del programa\n2.Volver al Menu de Configuracion del Sistema\n"; 
    std::cout<<"Seleccione una Opcion: ";
}
void ejecutarMenuConfiguracionFicheros(char *sendBuff, char *recvBuff, SOCKET s){
    int opcion;
    do{
        mostrarMenuConfiguracionFicheros();
        std::cin >> opcion; 
        sprintf(sendBuff, "%d", opcion);
        send(s, sendBuff, strlen(sendBuff), 0);
        switch(opcion){
            case 1:
                leerFicheroConfiguracion(recvBuff,s);
                break;
            case 2:
                std::cout<<"Volviendo al menu de configuracion del sistema...";
                break;
            default:
                std::cout<<"Opcion no valida. Por favor, seleccione una opcion valida.\n";
                break;
            
        }
    }while (opcion !=2);
}

void actualizarParametros(const char *clave, const char *nuevoValor){
    std::ifstream archivo("politicas.txt");
    if(!archivo){
       std::cerr << "No se pudo encontrar"<< std::endl;
        return;
    }
   
   std::ofstream temp("politicas_temp.txt");
   if(!temp){
    std::cerr <<"No se pudo abrir el archivo temporal para la escritura de los cambios" <<std::endl;
    archivo.close();
    return;
   }

   std:: string linea;
   int claveActualizada= 0;

   while (std:: getline(archivo, linea))
   { 
    std:: string copiaLinea = linea;
    char *token = strtok(const_cast<char*>(copiaLinea.c_str()), "=\n");


    if( token != nullptr  && strcmp(token, clave)== 0){
        temp << clave << "= " << nuevoValor <<"\n";
        claveActualizada=1;
    }else{
        temp << linea << "\n";
    }
   }
   
    archivo.close();
    temp.close();

   std:: remove("politicas.txt");
   std:: rename("politicas_temp.txt", "politicas.txt");
   

   if(claveActualizada==1){
    std::cout<< "Configuracion ' " << clave << " ' actualizada con exito \n\n";
   }else{
    std::cout << "Clave '" << clave << "' no encontrada\n";
   }
   
}
void mostrarMenuConfiguracionParametros(){
    std::cout<<"\n=== Configuracion de Politicas y Parametros del Sistema ===\n1. Configurar duracion maxima de prestamo \n2. Configurar cantidad maxima de libros prestados por usuario al dia\n3.Configurar multa por devolucion tardia\n4. Volver al Menu de Configurarcion del Sistema\n"; 
    std::cout<<"Seleccione una Opcion: ";
}
void ejecutarMenuConfiguracionParametros(char *sendBuff, char *recvBruff, SOCKET s){
    int opcion;
    char nuevoValor[50];
     do{
        mostrarMenuConfiguracionParametros();
        std::cin >> opcion;
        sprintf(sendBuff, "%d", opcion);
        send(s, sendBuff, strlen(sendBuff), 0);
        
        switch (opcion){
        case 1:
            std::cout << "\nIngrese la nueva duración maxima de prestamo (en dias): ";
            std::cin >> nuevoValor;
            sprintf(sendBuff, "%s", nuevoValor);
            send(s, sendBuff, strlen(sendBuff), 0);
        
            //actualizarParametros("DuracionMaximaPrestamo", nuevoValor);
            
            break;
        case 2: 
            std:: cout <<"\nIngrese la nueva cantidad maxima de libros prestados por usuario: ";
            std:: cin >> nuevoValor;
            sprintf(sendBuff, "%s", nuevoValor);
            send(s, sendBuff, strlen(sendBuff), 0);
            break;
        case 3:
            std::cout << "\nIngrese el nuevo numero de dias de sanción en caso de devolución tardia por dia: ";
            std::cin>> nuevoValor;
            sprintf(sendBuff, "%s", nuevoValor);
            send(s, sendBuff, strlen(sendBuff), 0);
            break;
        case 4: 
            std::cout << "\nVolviendo al Menu de Configuracioon del Sistema...\n\n";
            break;
        default:
           std::cout << "\nOpcion no valida. Por favor, seleccione una opcion valida.\n\n";
            break;
        }
    } while (opcion != 4);
    
}
void mostrarMenuConfiguracion(){
    std::cout<<"\n=== Configuracion del Sistema\n1. Fichero de Configuracion\n2. Politicas y Parametros del Sistema\n3. Volver al Menu Principal\n"; 
    std::cout<<"Seleccione una Opcion: ";
}
void ejecutarMenuConfiguracion(char *sendBuff, char *recvBuff, SOCKET s){
    int opcion; 
    do{
        mostrarMenuConfiguracion();
        std:: cin >>opcion;
        sprintf(sendBuff, "%d", opcion);
        send(s, sendBuff, strlen(sendBuff), 0);
        
        switch (opcion){
        case 1:
            ejecutarMenuConfiguracionFicheros(sendBuff,recvBuff,s);
            break;
        case 2: 
            ejecutarMenuConfiguracionParametros(sendBuff,recvBuff,s);
            break;
        case 3:
            std::cout<<"\n";
            std::cout<<"Volviendo al Menu Principal...\n\n";
            break;
        default:
            std::cout<<"Opcion no valida. Por favor, seleccione una opcion valida.\n";
            break;
        }
    } while (opcion != 3);
}