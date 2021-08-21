//
// Created by k0rb4k on 22/04/2021.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    struct sockaddr_in server;
    int sock;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(17);

    sock = socket(server.sin_family, SOCK_STREAM, IPPROTO_IP);
    if (sock == -1) {
        puts("socket not created");
        return 1;
    }
    puts("socket created");

    if (connect(sock, (const struct sockaddr *) &server, sizeof(server)) < 0) {
        puts("connection failed");
        return 2;
    }
    puts("connected to server");

    char *payload = "I dream of sushi";
    if (send(sock, payload, strlen(payload), 0) < 0) {
        puts("error while sending payload");
        return 3;
    }
    puts("payload sent");

    char response[2048];
    if (recv(sock, response, 2048, 0)) {
        puts("receive failed");
        return 4;
    }
    puts("server response :");
    puts(response);

    close(sock);

    return 0;
}