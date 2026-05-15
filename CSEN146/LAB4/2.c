#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <arpa/inet.h>

typedef struct{
    int len;
}Header;

typedef struct{
    Header header;
    char data[10];
}Packet;

void printPacket(Packet packet){
    printf("Packet{ header: { len: %d }, data \"", packet.header.len);
    fwrite(packet.data, (size_t)packet.header.len,1,stdout);
    printf("\}\n");
}

void clientSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, Packet packet){
    while(1){
        printf("Client sending packet\n");
        sendto(sockfd, &packet, sizeof(packet),0,address,addrlen);
        Packet recvpacket;
        recvfrom(sockfd, &recvpacket, sizeof(recvpacket),0,(struct sockaddr *)address, &addrlen);
        printPacket(recvpacket);
        break;
    }
}

