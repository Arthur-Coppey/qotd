//
// Created by k0rb4k on 11/10/2021.
//

#include "client.h"

int tcp() {
    struct sockaddr_in server;
    int sock;

    int proto = IPPROTO_IP;
    enum __socket_type socketType = SOCK_STREAM;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(17);

    sock = socket(server.sin_family, socketType, proto);
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
    if (recv(sock, response, 2048, 0) < 0) {
        puts("receive failed");
        return 4;
    }
    printf("server response : %s\n", response);

    close(sock);

    return 0;
}

int udp() {
    struct sockaddr_in server;
    int sock;

    int proto = IPPROTO_UDP;
    enum __socket_type socketType = SOCK_DGRAM;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(17);

    sock = socket(server.sin_family, socketType, proto);
    if (sock == -1) {
        puts("socket not created");
        return 1;
    }
    puts("socket created");

    char *payload = "I dream of sushi";
    if (sendto(sock, payload, strlen(payload), 0, (const struct sockaddr *) &server, sizeof server) < 0) {
        puts("error while sending payload");
        return 3;
    }
    puts("payload sent");

    char response[2048];
    if (recvfrom(sock, response, 2048, 0, (struct sockaddr *) &server, (socklen_t *) sizeof server) < 0) {
        puts("receive failed");
        return 4;
    }
    printf("server response : %s\n", response);

    close(sock);

    return 0;
}