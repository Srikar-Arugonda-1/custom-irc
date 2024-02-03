#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main()
{
    int sockfd, acceptfd, n;
    struct sockaddr_in addr;
    char recev[2048];

    addr.sin_family = AF_INET;
    addr.sin_port = htons(2000);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) != -1)
        printf("bind successful\n");

    listen(sockfd, 10);

    acceptfd = accept(sockfd, (struct sockaddr *)NULL, NULL);

    // memset(recev, 0, 2048);
    // recv(acceptfd, recev, sizeof(recev), 0);
    // printf("Response:\n%s\n", recev);

    // snprintf(recev, 2048, "Hello again\n");
    // send(acceptfd, recev, sizeof(recev), 0);

    char req[1024];
    int i = 5;
    while (i--)
    {
        memset(recev, 0, 2048);
        if ((n = recv(acceptfd, recev, sizeof(recev), 0)) == -1)
        {
            perror("receving error\n");
            exit(1);
        }
        // recev[n] = '\0';
        printf("%s\n", recev);
        fgets(req, 1024, stdin);
        send(acceptfd, req, strlen(req), 0);
    }

    close(acceptfd);
    close(sockfd);
}