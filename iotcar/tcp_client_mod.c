#include "tcp_client_mod.h"

struct sockaddr_in client_initial(int sock,char* IP_address){
	struct sockaddr_in sa;
    
    //memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(8000);
    sa.sin_addr.s_addr = inet_addr(IP_address);
    
    if ( connect(sock, (struct sockaddr*)&sa, sizeof(sa)) < 0 ){
        perror("connect()");
    }
    
    return sa;
}

void client_send_char(int sock,struct sockaddr_in sa,char c){
	
	if ( send(sock, &c, sizeof(c), 0) != sizeof(c) ){
        //perror("send()");
    }
}

char client_recv_char(int sock,struct sockaddr_in sa){
	char c;
	if( recv(sock, &c, sizeof(c), 0) == sizeof(c) ){
		return c;
	}
    else
    {
        perror("recvfrom()");
        return 'e';
    }
}
