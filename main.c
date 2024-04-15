#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>

#include "main.h"

#define ECHO_HEADER_SIZE 8
#define PACKET_SIZE 64




int main(int argc,char *argv[]){
    int sock;
    int status;

    sock = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP);

    if(sock < 0){
        perror("socket");
        return -1;
    }

    status = SendPing(&sock,argv[1]);
    close(sock);
    return 0;
}

int SendPing(int *sock,char *host){
    struct icmphdr *header;
    struct sockaddr_in addr;
    char buff[1500];
    unsigned char *ptr;
    int result = 0;
    int datasize = PACKET_SIZE;

    unsigned char data[] = "tekusiba kawaii";
    memset(&buff,0,sizeof(buff));
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host);
    
    header = (struct icmphdr *)buff;
    header->type = ICMP_ECHO;
    header->code = 0;
    header->checksum = 0;
    header->un.echo.id = 0;
    header->un.echo.sequence = 0;

    ptr = (unsigned char *)&buff[ECHO_HEADER_SIZE];
    datasize = PACKET_SIZE - ECHO_HEADER_SIZE;
    for(;datasize;datasize--){
        *ptr++=(unsigned char)0xA5;
    }
    ptr = (unsigned char *)&buff[ECHO_HEADER_SIZE];
    memcpy(ptr,&data,sizeof(data));
    header->checksum = GetChecksum((unsigned short*)header,PACKET_SIZE);
    
    result = sendto(*sock,header,PACKET_SIZE,0,(struct sockaddr *)&addr,sizeof(addr));
    if(result < 1){
        perror("send");
        return -1;
    }
    return 0;
}


unsigned short GetChecksum(unsigned short *buf,int size){
    unsigned long sum = 0;
    while (size > 1)
    {
        sum += *buf;
        buf++;
        size -= 2;
    }
    if(size == 1){
        sum+=*(unsigned char *)buf;
    }
    sum = (sum & 0xffff) + (sum >> 16);
    sum = (sum & 0xffff) + (sum >> 16);

    return (unsigned short)~sum;
}