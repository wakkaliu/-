#include "tcp_client_mod.h"
#include <unistd.h>

int main(int argc,char* argv[]){
        
        char a;

        if( (sock_tcp=socket(AF_INET, SOCK_STREAM, 0)) < 0 ) /* PF_INET */
        {
        perror("socket()");
        return -1;
        }
    
        //socka=client_initial(sock_tcp,"192.168.5.152");
        socka=client_initial(sock_tcp,"127.0.0.1");
        
                tcp_char='b';
                //fscanf(stdin, "%c", &tcp_char);
                client_send_char(sock_tcp,socka,tcp_char);
                tcp_char=client_recv_char(sock_tcp,socka);
                printf("tcp_char: %c\n",tcp_char);

                sleep(2);

                tcp_char='c';
                //fscanf(stdin, "%c", &tcp_char);
                client_send_char(sock_tcp,socka,tcp_char);
                tcp_char=client_recv_char(sock_tcp,socka);
                printf("tcp_char: %c\n",tcp_char);

                sleep(2);

                tcp_char='a';
                //fscanf(stdin, "%c", &tcp_char);
                client_send_char(sock_tcp,socka,tcp_char);
                tcp_char=client_recv_char(sock_tcp,socka);
                printf("tcp_char: %c\n",tcp_char);

                sleep(1);

                tcp_char='B';
                //fscanf(stdin, "%c", &tcp_char);
                client_send_char(sock_tcp,socka,tcp_char);
                tcp_char=client_recv_char(sock_tcp,socka);
                printf("tcp_char: %c\n",tcp_char);

                

                sleep(2);

                tcp_char='d';
                //fscanf(stdin, "%c", &tcp_char);
                client_send_char(sock_tcp,socka,tcp_char);
                tcp_char=client_recv_char(sock_tcp,socka);
                printf("tcp_char: %c\n",tcp_char);

                

        
        close(sock_tcp);
        return 0;
}

