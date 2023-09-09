//Pedroza Velarde Luis Rodrigo
//Cliente en C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define MAX_MESSAGE_LEN 1024

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char message[MAX_MESSAGE_LEN];
    char receivedMessage[MAX_MESSAGE_LEN];

    // Crear el socket del cliente
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Error al crear el socket del cliente");
        exit(1);
    }

    // Configurar la dirección del servidor
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &(serverAddr.sin_addr));

    // Conectar al servidor
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error al conectar al servidor");
        close(clientSocket);
        exit(1);
    }

    printf("Conectado al servidor de chat.\n");
    printf("Escribe 'exit' para salir.\n");

    while (1) {
        printf("Escribe un mensaje: ");
        fgets(message, MAX_MESSAGE_LEN, stdin);
        message[strlen(message) - 1] = '\0'; // Eliminar el salto de línea

        // Enviar el mensaje al servidor
        if (send(clientSocket, message, strlen(message), 0) == -1) {
            perror("Error al enviar el mensaje");
            break;
        }

        // Salir si el mensaje es "exit"
        if (strcmp(message, "exit") == 0) {
            break;
        }

        // Recibir una respuesta del servidor
        int bytesRead = recv(clientSocket, receivedMessage, sizeof(receivedMessage) - 1, 0);
        if (bytesRead == -1) {
            perror("Error al recibir respuesta del servidor");
            break;
        }
        receivedMessage[bytesRead] = '\0';
        printf("Respuesta del servidor: %s\n", receivedMessage);
    }

    // Cerrar el socket y salir
    close(clientSocket);
    printf("Desconectado del servidor de chat.\n");

    return 0;
}

