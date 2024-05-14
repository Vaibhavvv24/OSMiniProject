
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 5


typedef struct{
    char name[100];
    char password[100];
    int mmid;

} Member;

Member members[2]={
    {"admin","admin",1},
    {"user","user",2}
};

void authenticate(int client_socket){
    char name[100];
    char password[100];
    int authenticated=0;

    recv(client_socket, name, sizeof(name), 0);

    // Receive password from client
    recv(client_socket, password, sizeof(password), 0);


    for(int i=0;i<2;i++){
        if(strcmp(name,members[i].name)==0 && strcmp(password,members[i].password)==0){
            authenticated=1;
            break;
        }
    }
    if(authenticated){
        send(client_socket, "Authenticated", sizeof("Authenticated"), 0);
    }
    else{
        send(client_socket, "Not Authenticated", sizeof("Not Authenticated"), 0);
    }

}


void *handleconnect(void *nsd) {
    int sd = *((int *)nsd);
    char buf[1000];
    authenticate(sd);
    read(sd, buf, 1000);
    printf("Message from client: %s", buf);
    write(sd, "Hello, from server\n", sizeof("Hello, from server\n"));
    close(sd);
    pthread_exit(NULL);
}

int main() {
    struct sockaddr_in serv, cli;
    pthread_t threads[MAX_CLIENTS];
    int i=0;
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv.sin_family = AF_INET;
    serv.sin_port = htons(5000);
    bind(sd, (struct sockaddr *)&serv, sizeof(serv));
    listen(sd, 5);
    printf("Server listening on port 5000\n");
    while (1) {
        int len = sizeof(cli);
        int nsd = accept(sd, (struct sockaddr *)&cli, &len);
        pthread_create(&threads[i++], NULL, handleconnect, (void *)&nsd);
        if (i >= MAX_CLIENTS)
        {
            i = 0;
            // Join finished threads to release resources
            while (i < MAX_CLIENTS)
            {
                pthread_join(threads[i++], NULL);
            }
            i = 0;
        }
    }
    close(sd);
    return 0;
}
