#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUF 2048
#define MAX_CONN 10

int presentClients[MAX_CONN];
int presentClientsCount = 0;

int TCPconnect(int port)
{
    int sockfd;
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt));

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) != -1)
        printf("bind successful\n");

    return sockfd;
}

void sendToOthers(int cli_sock, char *recev)
{
    for (int i = 0; i < presentClientsCount; i++)
    {
        if (presentClients[i] != cli_sock)
        {
            send(presentClients[i], recev, strlen(recev), 0);
        }
    }
}

void handle_conn(void *sock_)
{
    int cli_sock = *(int *)sock_;
    char recev[BUF];
    char req[BUF];
    int n;
    while (1)
    {
        memset(recev, 0, BUF);
        if ((n = recv(cli_sock, recev, sizeof(recev), 0)) == -1)
        {
            perror("receving error\n");
            exit(1);
        }
        if (n == 0)
        {
            break;
        }
        printf("%s\n", recev);
        sendToOthers(cli_sock, recev);
    }

    close(cli_sock);
}

int main()
{
    int sockfd, n;

    sockfd = TCPconnect(2000);
    listen(sockfd, MAX_CONN);
    while (1)
    {
        struct sockaddr_in client;
        int cli_sock;
        int cli_add_size = sizeof(struct sockaddr_in);
        cli_sock = accept(sockfd, (struct sockaddr *)&client, &cli_add_size);
        presentClients[presentClientsCount++] = cli_sock;
        if (cli_sock == -1)
        {
            perror("error accepting\n");
            exit(1);
        }
        pthread_t tid;
        int *sock_ = (int *)malloc(sizeof(int));
        *sock_ = cli_sock;
        pthread_create(&tid, NULL, (void *)handle_conn, (void *)sock_);
    }

    // close(acceptfd);
    // close(sockfd);
    shutdown(sockfd, SHUT_RDWR);

    return 0;
}