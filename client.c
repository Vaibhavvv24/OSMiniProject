#include <stdio.h>
#include"string.h"
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

     char username[100];
    char password[100];
    char auth_message[100];

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    // Send username to server
    send(sd, username, strlen(username), 0);

    // Send password to server
    send(sd, password, strlen(password), 0);

    // Receive authentication result from server
    recv(sd, auth_message, sizeof(auth_message), 0);
    printf("%s\n", auth_message);
    if (strcmp(auth_message, "Authenticated") == 0) {
        printf("Authentication successful!\n");

        while(1){
            printf("Welcome to Library Management System \n");
            printf("Enter your choice: \n");
            printf("A- Issue book \n");
            printf("B- Return book \n");
            printf("C- Exit \n");
            char choice;
            scanf("%c", choice);
            if(choice == 3){
                break;
            }
            send(sd,choice, sizeof(choice), 0);

            switch (choice)
            {
            case 'A':
                issueBook(sd);
                break;

            case 'B':
                returnBook(sd);
                break;
            
            default:
                break;
            }

        }

        
    }
    else{
        printf("Authentication failed!\n");
        exit(0);
    }

    close(sd);

    // char buf[100];
    // write(sd,"This is msg from client\n",sizeof("This is msg from client\n"));
    // read(sd,buf,sizeof(buf));
    // printf("%s\n",buf);
    // close(sd);
}

void issueBook(int sd){
    char bookName[100];
    char author[100];
    int bookId;

    printf("Enter book name: ");
    scanf("%s", bookName);

    printf("Enter author name: ");
    scanf("%s", author);

    printf("Enter book id: ");
    scanf("%d", &bookId);

    send(sd, bookName, sizeof(bookName), 0);
    send(sd, author, sizeof(author), 0);
    send(sd, &bookId, sizeof(bookId), 0);
    char result[100];
    recv(sd, result, sizeof(result), 0);
    printf("%s\n", result);
}

void returnBook(int sd){
    int bookId;
    printf("Enter book id to return: ");
    scanf("%d", &bookId);
    send(sd, bookId, sizeof(bookId), 0);

    char result[100];
    recv(sd, result, sizeof(result), 0);
    printf("%s\n", result);
}