/*
* Name: Austin Nguyen
* Date: Monday 2:15PM - 5:00 PM
* Title: Lab5 - client.c
* Description: This program is the UDP client c function. Implemented stop and wait 
* functionality through the use of calculating checksum, sequence numbers, and ACKs.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/time.h>

#define BUFFER_SIZE 10 // Max data size per packet
#define TIMEOUT_SEC 1  // Timeout in seconds
#define MAX_RETRIES 3  // Maximum number of resends for last packet

// Define a structure for the packet header
typedef struct {
    int seq_ack;
    int len;  // Length of the data in the packet
    int cksum;
} Header;

// Define the Packet structure, which contains a header and a fixed-size data array
typedef struct {
    Header header;
    char data[BUFFER_SIZE];  // Data field limited to 10 bytes
} Packet;

// Function to calculate the checksum
int calcChecksum(Packet packet) {
    packet.header.cksum = 0;
    int checksum = 0;
    char *ptr = (char *)&packet;
    char *end = ptr + sizeof(Header) + packet.header.len;
    
    while (ptr < end) {
        checksum ^= *ptr++;
    }
    return checksum;
}

// Function to send a packet with Stop-and-Wait ARQ
void send_packet(int sockfd, struct sockaddr_in *server_addr, socklen_t addr_size, Packet *pkt) {
    int retries = 0;
    struct timeval tv;
    fd_set readfds;
    int rv;

    while (retries < MAX_RETRIES) {
        // Calculate and set checksum
        pkt->header.cksum = calcChecksum(*pkt);

        // Send the packet
        sendto(sockfd, pkt, sizeof(Packet), 0, (struct sockaddr *)server_addr, addr_size);
        printf("Sent packet (SEQ %d, LEN %d, CHECKSUM %d)\n", pkt->header.seq_ack, pkt->header.len, pkt->header.cksum);

        // Set up timeout for ACK
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);
        tv.tv_sec = TIMEOUT_SEC;
        tv.tv_usec = 0;

        rv = select(sockfd + 1, &readfds, NULL, NULL, &tv);
        if (rv == 0) {
            // Timeout occurred
            printf("Timeout! Resending packet (SEQ %d)\n", pkt->header.seq_ack);
            retries++;
        } else {
            // Receive ACK
            Packet ack_pkt;
            recvfrom(sockfd, &ack_pkt, sizeof(Packet), 0, NULL, NULL);

            // Verify ACK
            int ack_checksum = calcChecksum(ack_pkt);
            if (ack_pkt.header.cksum == ack_checksum && ack_pkt.header.seq_ack == pkt->header.seq_ack) {
                printf("Received valid ACK (SEQ %d)\n", ack_pkt.header.seq_ack);
                return; // Packet acknowledged, move to next
            } else {
                printf("Invalid ACK! Resending packet (SEQ %d)\n", pkt->header.seq_ack);
                retries++;
            }
        }
    }

    printf("Max retries reached. Moving to next packet.\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <port> <source_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);
    char *filename = argv[2];

    // Open source file for reading
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Create UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Server address setup
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Change as needed
    socklen_t addr_size = sizeof(server_addr);

    Packet pkt;
    int seq = 0; // Alternating sequence number (0 or 1)

    while (1) {
        // Read data from file
        size_t bytes_read = fread(pkt.data, 1, BUFFER_SIZE, file);
        pkt.header.len = bytes_read;
        pkt.header.seq_ack = seq;

        // If no data left, send termination packet
        if (bytes_read == 0) {
            pkt.header.len = 0;
            printf("Sending termination packet.\n");
            send_packet(sockfd, &server_addr, addr_size, &pkt);
            break;
        }

        // Send packet with Stop-and-Wait ARQ
        send_packet(sockfd, &server_addr, addr_size, &pkt);

        // Alternate sequence number (0 ↔ 1)
        seq = 1 - seq;
    }

    printf("File transmission completed.\n");

    // Cleanup
    fclose(file);
    close(sockfd);
    return 0;
}