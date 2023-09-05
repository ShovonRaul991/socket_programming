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

    FILE *fp;
    int ch = 0;
    fp = fopen("glad_received.txt", "a");
    int words;

    read(newsockfd, &words, sizeof(int));

    while(ch != words){
        read(newsockfd, buffer, 255);
        fprintf(fp, "%s ", buffer);
        ch++;
    }
    printf("File has been received successfully and saved by the namne glad_received.txt");

    close(newsockfd);
    close(sockfd);
    return 0;
}