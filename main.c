#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

#define PORT 8080
#define BACKLOG 10

void *handle_connection(void *socket_desc){
    int sock = *(int *)socket_desc;
    free(socket_desc);

    // Buffer para armazenar a requisição
    char buffer[1024] = {0};
    int read_size;

    // Lê a requisição
    read_size = read(sock, buffer, sizeof(buffer) - 1);
    if (read_size < 0){
        perror("read");
        close(sock);
        return NULL;
    }

    // Envia uma resposta simples
    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 13\r\n"
        "Connection: close\r\n"
        "\r\n"
        "Hello, World!";
    write(sock, response, strlen(response));

    // Fecha o socket
    close(sock);
    return NULL;
}

int main(){
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    pthread_t thread_id;

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(server_fd, BACKLOG) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);
    while (1){
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (new_socket < 0)
        {
            perror("accept");
            continue;
        }
        printf("Accepted a new connection\n");
        int *new_sock = malloc(sizeof(int));
        *new_sock = new_socket;
        if (pthread_create(&thread_id, NULL, handle_connection, (void *)new_sock) < 0)
        {
            perror("could not create thread");
            exit(EXIT_FAILURE);
        }
        pthread_detach(thread_id);
    }

    return 0;
}
