#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

void error(const char *msg){
    //function takes the error number and outputs the error description
    perror(msg);
    exit(1);
}

//argv[0] contain the filename and argv[1] contain the port number

int main(int argc, char *argv[]){
    if(argc<2)
    {
        fprintf(stderr, "Port number not provided. Program terminated\n");
        exit(1);
    }
    int sockfd, newsockfd, portno, n;
    char buffer[255];

    struct sockaddr_in serv_addr, cli_addr; //internet address
    socklen_t clilen;  //32bit datatype
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0){
        error("Error opening socket");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr)); //It clears everything
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    //htons stands for host to network flow

    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        error("Binding failed!");
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

    if(newsockfd<0){
        error("Error on accept");
    }

    while(1){
        bzero(buffer, 255);
        n = read(newsockfd, buffer, 255);
        if(n<0){
            error("Error on reading");
        }
        printf("Client: %s", buffer);
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);

        n = write(newsockfd, buffer, strlen(buffer));
        if(n<0){
            error("Error on writing");
        }

        int l = strncmp("Bye", buffer, 3);
        if(l == 0)
            break;
    }
    close(newsockfd);
    close(sockfd);
    return 0;
}