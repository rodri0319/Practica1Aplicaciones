//Servidor de chat en C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define MAX_CONNECTIONS 10

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    // Crear el socket del servidor
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Error al crear el socket del servidor");
        exit(EXIT_FAILURE);
    }

    // Configurar la dirección del servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Vincular el socket a la dirección y el puerto
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error al vincular el socket");
        exit(EXIT_FAILURE);
    }

    // Escuchar por conexiones entrantes
    if (listen(serverSocket, MAX_CONNECTIONS) == -1) {
        perror("Error al escuchar por conexiones entrantes");
        exit(EXIT_FAILURE);
    }

    printf("Servidor en espera en el puerto %d...\n", PORT);

    // Aceptar conexiones entrantes y manejarlas
    while (1) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            perror("Error al aceptar la conexión del cliente");
            continue;
        }

        printf("Cliente conectado desde %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

        // Manejar la comunicación con el cliente
        char buffer[1024];
        int bytesRead;
        while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
            buffer[bytesRead] = '\0';
            printf("Cliente dice: %s", buffer);

            // Enviar una respuesta al cliente
            const char* response = "Mensaje recibido por el servidor desde lenguaje C\n";
            send(clientSocket, response, strlen(response), 0);
        }

        if (bytesRead == 0) {
            printf("Cliente desconectado.\n");
        } else {
            perror("Error al recibir datos del cliente");
        }

        // Cerrar la conexión con el cliente
        close(clientSocket);
    }

    // Cerrar el socket del servidor
    close(serverSocket);

    return 0;
}

