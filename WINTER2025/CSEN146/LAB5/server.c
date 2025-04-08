/*
* Name: Austin Nguyen
* Date: Monday 2:15PM - 5:00 PM
* Title: Lab5 - server.c
* Description: This program is the UDP server c function. Implemented stop and wait 
* functionality through the use of calculating checksum, sequence numbers, and ACKs.
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
    int seq_ack;
    int len;
    int cksum;
} Header;

typedef struct {
    Header header;
    char data[10];
} Packet;

struct sockaddr_in servAddr, clienAddr;

int calcChecksum(Packet packet){
    packet.header.cksum = 0;
    int checksum = 0;
    char *ptr = (char *)&packet;
    char *end = ptr + sizeof(Header) + packet.header.len;
    while(ptr < end){
        checksum ^= *ptr++;
    }
    return checksum;
}

void printPacket(Packet packet) {
    printf("Packet{ header: { seq_acks: %d, len: %d, cksum: %d }, data \"", packet.header.seq_ack, packet.header.len, packet.header.cksum);
    fwrite(packet.data, (size_t)packet.header.len, 1, stdout);
    printf("\"}\n");
}

void serverSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, int seqnum) {
    if (rand() %5 == 0){
        printf("Dropping ACK\n");
    }
    
    Packet packet;
    packet.header.seq_ack = seqnum;
    packet.header.len = sizeof(packet.data);
    packet.header.cksum = calcChecksum(packet);
    sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);
    printf("\tServer sending ACK %d, Checksum %d \n", packet.header.seq_ack, packet.header.cksum);
}

Packet serverReceive(int sockfd, struct sockaddr *address, socklen_t *addrlen, int *seqnum) {
    Packet packet;
    while (1) {
        recvfrom(sockfd, &packet, sizeof(packet), 0, address, addrlen);
        printPacket(packet);

        int e_cksum = calcChecksum(packet);
        if (packet.header.cksum != e_cksum){
            printf("\t Server: Bad Checksum. Expected Checksum: %d\n", e_cksum);
            serverSend(sockfd, address, *addrlen, *seqnum);
        }
        else if(packet.header.seq_ack != *seqnum){
            printf("\t Server: Bad seqnum. Expected seqnum: %d \n", *seqnum);
            serverSend(sockfd, address, *addrlen, *seqnum);
        }else{
            printf("\t Server: Good Packet\n");
            serverSend(sockfd, address, *addrlen, *seqnum);
            *seqnum = 1 - *seqnum; // Toggle sequence number
            break;
        }
    }
    return packet;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <port> <dstfile>\n", argv[0]);
        exit(0);
    }

    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Failure to setup endpoint socket\n");
        exit(1);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(atoi(argv[1]));
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((bind(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr))) < 0) {
        perror("Failure to bind server socket to endpoint socket\n");
        exit(1);
    }

    int fp = open(argv[2], O_CREAT | O_RDWR, 0666);
    if (fp < 0) {
        perror("Failed to open file.\n");
        exit(1);
    }

    int seqnum = 0;
    Packet packet;
    socklen_t addr_len = sizeof(clienAddr);
    printf("Waiting for Packets to come...\n");

    do {
        packet = serverReceive(sockfd, (struct sockaddr *)&clienAddr, &addr_len, &seqnum);
        // write(fp, packet.data, strlen(packet.data));
        write(fp, packet.data, packet.header.len);
    } while (packet.header.len != 0);

    close(fp);
    close(sockfd);

    return 0;
}
