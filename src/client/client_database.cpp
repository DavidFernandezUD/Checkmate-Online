#include "client_database.h"

void send_username_to_server(SOCKET s, const std::string& data) {
    // Convertir el string a un array de caracteres
    const char* buffer = data.c_str();

    // Enviar los datos al servidor
    send(s, buffer, strlen(buffer), 0);
}