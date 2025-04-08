/*  
 * Name: Austin Nguyen  
 * Date: Monday 2:15PM-5:00 PM  
 * Title: Lab3 - Concurrent Server  
 * Description: A TCP server that transfers a file to multiple clients concurrently using threads. 
 * ./step4 src.dat 8080 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

typedef struct {
    int client_fd;
    char file_name[256];
} client_data_t;

void *handle_client(void *arg) {
    client_data_t *client_data = (client_data_t *)arg;
    int client_fd = client_data->client_fd;
    char *file_name = client_data->file_name;
    char buffer[BUFFER_SIZE] = {0};

    // Open the file to send
    FILE *file = fopen(file_name, "rb");
    if (file == NULL) {
        perror("File not found");
        close(client_fd);
        free(client_data);
        pthread_exit(NULL);
    }

    // Send the file to the client
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        send(client_fd, buffer, bytes_read, 0);
    }
    printf("File transfer to client completed.\n");

    // Clean up
    fclose(file);
    close(client_fd);
    free(client_data);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file_name> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *file_name = argv[1];
    int port = atoi(argv[2]);
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
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

    while (1) {
        // Accept a client connection
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
        if (client_fd < 0) {
            perror("Accept failed");
            continue; // Continue to accept new connections
        }
        printf("Connection established with a client.\n");

        // Allocate memory for client data
        client_data_t *client_data = malloc(sizeof(client_data_t));
        if (client_data == NULL) {
            perror("Memory allocation failed");
            close(client_fd);
            continue;
        }
        client_data->client_fd = client_fd;
        strncpy(client_data->file_name, file_name, sizeof(client_data->file_name) - 1);
        client_data->file_name[sizeof(client_data->file_name) - 1] = '\0';

        // Create a new thread to handle the client
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void *)client_data) != 0) {
            perror("Thread creation failed");
            close(client_fd);
            free(client_data);
        }

        // Detach the thread to allow automatic resource cleanup
        pthread_detach(thread_id);
    }

    close(server_fd);
    return 0;
}
