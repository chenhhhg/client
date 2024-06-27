#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#define SERVPORT 3333
#define MAXDATASIZE 100
#define BACKLOG 10//请求队列中允许的最大请求数

int main(int argc,char *argv[]) {
    int sockfd;
    struct hostent* host;
    struct sockaddr_in serv_addr;

    if(argc < 2) {//需要用户指定链接的地址
        fprintf(stderr,"Please enter the server's hostname");
        exit(1);
    }

    if((host = gethostbyname(argv[1])) == NULL) {//转换为hostent
        perror("gethostbyname");
        exit(1);
    }

    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1) {//创建socket
        perror("socket");
        exit(1);
    }

    //填充数据
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVPORT);
    serv_addr.sin_addr = *((struct in_addr *)host->h_addr);
    bzero(&(serv_addr.sin_zero),8);

    if((connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr))) == -1) {//发起对服务器的链接
        perror("connect");
        exit(1);
    }


    while (1) {
        char* buffer = calloc(MAXDATASIZE, sizeof(char));
        printf("enter the friendly domain name\n");
        scanf("%s",buffer);
        if (strcmp(buffer, "close")==0) {
            break;
        }
        send(sockfd, buffer, strlen(buffer), 0);
        recv(sockfd,buffer,MAXDATASIZE,0);
        printf("response from dns server: %s\n", buffer);
        free(buffer);
    }

    close(sockfd);

}