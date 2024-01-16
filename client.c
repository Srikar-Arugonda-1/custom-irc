#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main()
{

    char *ip = "142.250.183.238";

    int connfd, sockfd, n;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    inet_pton(AF_INET, ip, &addr.sin_addr.s_addr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (connfd = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == 0)
        printf("Connection success\n");

    char *req = "GET / HTTP/1.1\nHost: google.com\n\n";
    send(sockfd, req, strlen(req), 0);

    printf(":\n");

    char recev[2048];
    // memset(recv, 0, 2048);
    // while ((n = read(sockfd, recev, sizeof(recev))) > 0)
    // {
    //     printf("%s\n", recev);
    //     memset(recv, 0, 2048);
    // }

    if ((n = recv(sockfd, recev, sizeof(recev) - 1, 0)) == -1)
    {
        perror("receving error\n");
        exit(1);
    }
    recev[n] = '\0';
    printf("%s\n", recev);

    close(sockfd);

    return 0;
}