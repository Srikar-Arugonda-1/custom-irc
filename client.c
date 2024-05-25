#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUF 2048
#define NAME_SZ 10

int msg_came = 0;

int TCPconnect(char *ip, int port)
{
    int sockfd;
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr.s_addr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == 0)
        return sockfd;
    else
        return -1;
}

void readMsg(void *sock_)
{
    int sockfd = *(int *)sock_;
    char recev[BUF];
    int n;

    while (1)
    {
        memset(recev, 0, BUF);
        if ((n = recv(sockfd, recev, sizeof(recev), 0)) != -1)
        {
            msg_came = 1;
            printf("\r\033[K");
            printf("%s\n", recev);
            printf("You: ");
        }
    }

    close(sockfd);
}

void ReadMsgFromServer(int sockfd)
{
    pthread_t tid;
    int *sock_ = (int *)malloc(sizeof(int));
    *sock_ = sockfd;
    pthread_create(&tid, NULL, (void *)readMsg, (void *)sock_);
}

void sendMsgToServer(int sockfd)
{
    char name[NAME_SZ], buf[BUF - NAME_SZ], msg[BUF];
    printf("Name: ");
    fgets(name, NAME_SZ, stdin);
    name[strlen(name) - 1] = '\0';

    while (1)
    {
        printf("You: ");
        fgets(buf, BUF - NAME_SZ, stdin);
        buf[strlen(buf) - 1] = '\0';
        if (strcmp(buf, "/exit") == 0)
        {
            shutdown(sockfd, SHUT_RDWR);
            break;
        }
        snprintf(msg, sizeof(msg), "%s:%s", name, buf);
        send(sockfd, msg, strlen(msg), 0);
    }
}

int main()
{
    char *ip = "127.0.0.1";
    int sockfd, n;
    char recev[BUF];
    sockfd = TCPconnect(ip, 2000);
    if (sockfd == -1)
    {
        perror("Connection error\n");
        exit(1);
    }

    ReadMsgFromServer(sockfd);
    sendMsgToServer(sockfd);

    close(sockfd);

    return 0;
}