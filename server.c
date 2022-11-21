//
// Created by k0rb4k on 30/03/2021.
//

#include "server.h"
#include "config.h"

//TODO: object-like C
//TODO: refactoring, cleanup

extern int errno;

_Noreturn void udpListen(void *args[]) {
    struct sockinfo *server = (struct sockinfo *) args[0];
    struct sockinfo *client = (struct sockinfo *) args[1];
    char quote[MESSAGE_STRING_LENGTH] = "", clientMessage[MESSAGE_STRING_LENGTH] = "";

    // create args
    server->handle = socket(server->address->sa_family, SOCK_DGRAM, IPPROTO_UDP);
    if (server->handle == -1) {
        perror("socket not created");
        pthread_exit((void *) 1);
    }
    puts("socket created");

    // bind args
    if (bind(server->handle, server->address, server->addressLength) < 0) {
        perror("bind failed");
        pthread_exit((void *) 2);
    }

    for (;;) {
        // receive data
        if (recvfrom(server->handle, clientMessage, MESSAGE_STRING_LENGTH, 0, client->address,
                     &client->addressLength) >= 0) {
            printf("received: %s\n", clientMessage);

            // send data
            getRandomQuote(quote);
            printf("sending: %s\n", quote);
            if (sendto(server->handle, quote, (long) strlen(quote), 0, client->address, client->addressLength) == -1) {
                perror("[UDP: SENDTO]");
            }
        } else {
            perror("[UDP: RECVFROM]");
        }
    }
}

void tcpListen(void *args[]) {
    struct sockinfo *server = (struct sockinfo *) args[0];
    struct sockinfo *client = (struct sockinfo *) args[1];
    char quote[MESSAGE_STRING_LENGTH] = "", clientMessage[MESSAGE_STRING_LENGTH] = "";

    // create args
    server->handle = socket(server->address->sa_family, SOCK_STREAM, IPPROTO_TCP);
    if (server->handle == -1) {
        perror("socket not created");
        pthread_exit((void *) 1);
    }
    puts("socket created");

    // bind args
    if (bind(server->handle, server->address, server->addressLength) < 0) {
        perror("bind failed");
        pthread_exit((void *) 2);
    }

    listen(server->handle, 5);
    puts("server listening");

    // accept connections
    //TODO: t h r e a d s
    while ((client->handle = accept(server->handle, client->address, &client->addressLength)) >= 0) {
        puts("client connected");

        // receive data
        if (recv(client->handle, clientMessage, MESSAGE_STRING_LENGTH, 0) >= 0) {
            printf("client says: %s\n", clientMessage);

            // send data
            getRandomQuote(quote);
            printf("responding: \"%s\"\n", quote);
            if (send(client->handle, quote, (long) strlen(quote), 0) == -1) {
                perror("[TCP: SEND]");
            }
        } else {
            perror("[TCP: RECV]");
        }

        close(client->handle);

        puts("closed connection");
    }

    if (client->handle < 0) {
        perror("client connection failed");
        pthread_exit((void *) 3);
    }
}

void server() {
    struct sockinfo client;
    struct sockinfo udp, tcp;
    struct sockaddr_in6 serverAddress, clientAddress;

    serverAddress.sin6_family = AF_INET6;
    serverAddress.sin6_addr = in6addr_any;
    serverAddress.sin6_port = htons(DEFAULT_LISTEN_PORT);

    tcp.address = (struct sockaddr *) &serverAddress;
    udp.address = (struct sockaddr *) &serverAddress;
    tcp.addressLength = sizeof serverAddress;
    udp.addressLength = sizeof serverAddress;

    clientAddress.sin6_family = AF_INET6;
    client.address = (struct sockaddr *) &clientAddress;
    client.addressLength = sizeof clientAddress;

    void *tcpArgs[] = {&tcp, &client};
    void *udpArgs[] = {&udp, &client};

    // TODO: threads to run both at the same time
    pthread_t tcpHandle, udpHandle;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tcpHandle, &attr, (void *(*)(void *)) tcpListen, tcpArgs);
    pthread_create(&udpHandle, &attr, (void *(*)(void *)) udpListen, udpArgs);

    void *tcpReturn, *udpReturn;
    pthread_join(tcpHandle, tcpReturn);
    pthread_join(udpHandle, udpReturn);
}
