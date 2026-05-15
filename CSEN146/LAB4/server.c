/*
* Name: Austin Nguyen
* Date: Monday 2:15PM - 5:00 PM
* Title: Lab4 - server.c
* Description: This program is the UDP server c function. 
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

// Define a structure for the packet header
typedef struct {
    int len;  // Length of the data in the packet
} Header;

// Define the Packet structure, which contains a header and a fixed-size data array
typedef struct {
    Header header;
    char data[10];  // Data field limited to 10 bytes
} Packet;

// Global server and client address structures
struct sockaddr_in servAddr, clienAddr;

// Function to print the contents of a packet
void printPacket(Packet packet) {
    printf("Packet{ header: { len: %d }, data \"", packet.header.len);
    fwrite(packet.data, (size_t)packet.header.len, 1, stdout);
    printf("\"}\n");
}

// Function to send a response packet from the server
void serverSend(int sockfd, const struct sockaddr *address, socklen_t addrlen) {
    Packet packet;
    strcpy(packet.data, "Ack");  // Set response message
    packet.header.len = sizeof(packet.data);  // Set packet length
    sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);  // Send the packet
    printf("\tServer sending %s \n", packet.data);
}

// Function to receive a packet from a client
Packet serverReceive(int sockfd, struct sockaddr *address, socklen_t *addrlen) {
    Packet packet;
    while (1) {
        recvfrom(sockfd, &packet, sizeof(packet), 0, address, addrlen);  // Receive data
        printPacket(packet);  // Print received packet details
        serverSend(sockfd, address, *addrlen);  // Send acknowledgment
        break;  // Exit loop after receiving one packet
    }
    return packet;
}

int main(int argc, char *argv[]) {
    // Ensure correct command-line arguments
    if (argc != 3) {
        printf("Usage: %s <port> <dstfile>\n", argv[0]);
        exit(0);
    }

    int sockfd;
    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Failure to setup endpoint socket\n");
        exit(1);
    }

    // Configure server address settings
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(atoi(argv[1]));  // Convert port argument to network byte order
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);  // Accept connections from any IP

    // Bind the socket to the server address
    if ((bind(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr))) < 0) {
        perror("Failure to bind server socket to endpoint socket\n");
        exit(1);
    }

    // Open the destination file for writing
    int fp = open(argv[2], O_CREAT | O_RDWR, 0666);
    if (fp < 0) {
        perror("Failed to open file.\n");
        exit(1);
    }

    Packet packet;
    socklen_t addr_len = sizeof(clienAddr);
    printf("Waiting for Packets to come...\n");

    // Loop to receive packets until an empty packet (header length 0) is received
    do {
        packet = serverReceive(sockfd, (struct sockaddr *)&clienAddr, &addr_len);
        write(fp, packet.data, strlen(packet.data));  // Write received data to the file
    } while (packet.header.len != 0);

    // Close file and socket before exiting
    close(fp);
    close(sockfd);

    return 0;
}
