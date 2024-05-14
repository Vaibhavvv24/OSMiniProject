#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
//client
int main(){
    struct sockaddr_in serv;
    int sd = socket(AF_INET,SOCK_STREAM,0);
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv.sin_port = htons(5000);
    connect(sd,(struct sockaddr *)&serv,sizeof(serv));
    perror("Connection with server");
    char buf[100];
    write(sd,"This is msg from client\n",sizeof("This is msg from client\n"));
    read(sd,buf,sizeof(buf));
    printf("%s\n",buf);
    close(sd);
}