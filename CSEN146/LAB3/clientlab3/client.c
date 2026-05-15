/*  
 * Name: Name: Austin Nguyen  
 * Date: Monday 2:15PM-5:00 PM  
 * Title: Lab3 - Client  
 * Description: A TCP client that connects to a server and receives a specified file.  
 * ./client 127.0.0.1 8080 dst.dat
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <server_ip> <port> <file_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *server_ip = argv[1];
    int port = atoi(argv[2]);
    char *file_name = argv[3];
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    // Convert IP address to binary form and set it
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }
    printf("Connected to server.\n");

    // Open file to save received data
    FILE *file = fopen(file_name, "wb");
    if (file == NULL) {
        perror("File creation failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Receive file
    size_t bytes_received;
    while ((bytes_received = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, 1, bytes_received, file);
    }
    printf("File received and saved as %s.\n", file_name);

    // Clean up
    fclose(file);
    close(sock);

    return 0;
}
