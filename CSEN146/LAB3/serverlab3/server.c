/*  
 * Name: Name: Austin Nguyen  
 * Date: Monday 2:15PM-5:00 PM   
 * Title: Lab3 - Server  
 * Description: A TCP server that transfers a file specified by the user to the client.  
 * ./server src.dat 8080
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file_name> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *file_name = argv[1];
    int port = atoi(argv[2]);
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE] = {0};
    socklen_t addr_len = sizeof(client_addr);

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind socket
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d...\n", port);

    // Accept a client connection
    if ((client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len)) < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Connection established with client.\n");

    // Open file to send
    FILE *file = fopen(file_name, "rb");
    if (file == NULL) {
        perror("File not found");
        close(client_fd);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Send file
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        send(client_fd, buffer, bytes_read, 0);
    }
    printf("File transfer completed.\n");

    // Clean up
    fclose(file);
    close(client_fd);
    close(server_fd);

    return 0;
}
