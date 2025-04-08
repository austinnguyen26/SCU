#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

//define header struct for use in packet
typedef struct {
    int len;
} Header;

//define packet struct
typedef struct {
    Header header;
    char data[1024];
} Packet;

//prints packet 
void printPacket(Packet packet) {
    printf("Packet{ header: { len: %d }, data: \"%s\" }\n", packet.header.len, packet.data);
}

//sends data
void clientSend(int sockfd, const struct sockaddr_in *servAddr, Packet packet) {
    sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)servAddr, sizeof(*servAddr));
    printPacket(packet);
    printf("Client sending packet\n");

    // Receiving ACK from the server
    Packet recvpacket;
    socklen_t addr_len = sizeof(*servAddr);
    recvfrom(sockfd, &recvpacket, sizeof(recvpacket), 0, (struct sockaddr *)servAddr, &addr_len);
    printPacket(recvpacket);
    printf("Server acknowledged: %s\n", recvpacket.data);
}

int main(int argc, char **argv) {

    //check args 
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <server IP> <port> <input file>\n", argv[0]);
        exit(1);
    }

    //create socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Cannot create socket");
        return 1;
    }
    //servAddr args
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &servAddr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return 1;
    }

    //open file
    int fp = open(argv[3], O_RDONLY);
    if (fp < 0) {
        perror("Failed to open input file");
        return 1;
    }

    //send server
    Packet packet;
    ssize_t bytes_read;
    while ((bytes_read = read(fp, packet.data, sizeof(packet.data) - 1)) > 0) {
        packet.data[bytes_read] = '\0';  // Ensure null-terminated string
        packet.header.len = bytes_read;
        clientSend(sockfd, &servAddr, packet);
    }
    
    // Send "Done" packet
    strcpy(packet.data, "Done");
    packet.header.len = strlen(packet.data);
    clientSend(sockfd, &servAddr, packet);

    //close file pointer and socket
    close(fp);
    close(sockfd);
    return 0;
}
