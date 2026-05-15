#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

// Define header struct for use in packet
typedef struct {
    int len;
} Header;

// Define packet struct
typedef struct {
    Header header;
    char data[1024];
} Packet;

// Prints received packet
void printPacket(Packet packet) {
    printf("Packet{ header: { len: %d }, data: \"%s\" }\n", packet.header.len, packet.data);
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <port> <output file>\n", argv[0]);
        exit(1);
    }

    // Create socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Cannot create socket");
        return 1;
    }

    // Server address structure
    struct sockaddr_in servAddr, clientAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_port = htons(atoi(argv[1]));

    // Bind socket
    if (bind(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    // Open destination file
    int fp = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fp < 0) {
        perror("Failed to open output file");
        close(sockfd);
        return 1;
    }

    printf("Server listening on port %s, writing to file: %s\n", argv[1], argv[2]);

    Packet packet;
    socklen_t addr_len = sizeof(clientAddr);

    while (1) {
        // Receive packet
        ssize_t recv_len = recvfrom(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&clientAddr, &addr_len);
        if (recv_len < 0) {
            perror("Receive failed");
            continue;
        }

        // Print received packet
        printPacket(packet);

        // Check for termination packet
        if (strcmp(packet.data, "Done") == 0) {
            printf("Received 'Done' packet. Shutting down server.\n");
            break;
        }

        // Write to output file
        if (write(fp, packet.data, packet.header.len) < 0) {
            perror("Write to file failed");
            break;
        }

        // Send ACK back to client
        sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&clientAddr, addr_len);
    }

    // Close file and socket
    close(fp);
    close(sockfd);
    return 0;
}
