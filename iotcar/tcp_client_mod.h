#ifndef __TCP_CLIENT_MOD_H__
#define __TCP_CLIENT_MOD_H__

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct sockaddr_in client_initial(int sock,char* IP_address);
void client_send_char(int sock,struct sockaddr_in sa,char c);
char client_recv_char(int sock,struct sockaddr_in sa);

int sock_tcp;
struct sockaddr_in socka;
char tcp_char;
#endif
