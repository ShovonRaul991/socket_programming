/*
    filename server_ipaddress portno argv[3]
*/

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<unistd.h>
#include<netdb.h>
#include<string.h>

void error(const char *msg){
    //function takes the error number and outputs the error description
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[255];
    if(argc <3){
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0){
        error("Error while opening socket");
    }

    server = gethostbyname(argv[1]);
    if(server == NULL){
        fprintf(stderr, "Error, No such host");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr,(char *) &serv_addr.sin_addr.s_addr, server->h_length );
    serv_addr.sin_port = htons(portno);

    if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        error("Connection failed");
    }

    while (1)
    {
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);
        n = write(sockfd, buffer, strlen(buffer));
        if(n<0){
            error("Error in writing");
        }
        bzero(buffer, 255);
        n = read(sockfd, buffer, 255);
        if(n<0){
            error("Error in reading");
        }
        printf("Server: %s", buffer);
        
        int l = strncmp("Bye", buffer, 3);
        if(l == 0)
            break;
    }
    close(sockfd);
    return 0;
    
}

