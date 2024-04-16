#pragma once

#define BUFFERSIZE 1500

int SendPing(int *socket,char *host);
unsigned short GetChecksum(unsigned short *buf,int size);
int RecvPing(int *sock);