//
// Created by k0rb4k on 30/03/2021.
//

#include "server.h"
#include "config.h"

//TODO: object-like C
//TODO: refactoring, cleanup

extern int errno;

_Noreturn void udpListen(void * args[]) {
    struct sockinfo * server = (struct sockinfo *) args[0];
    struct sockinfo * client = (struct sockinfo *) args[1];
    char quote[MESSAGE_STRING_LENGTH] = "", clientMessage[MESSAGE_STRING_LENGTH] = "";

    // create args
    server->handle = socket(server->address->sa_family, SOCK_DGRAM, IPPROTO_UDP);
    if (server->handle == -1) {
        perror("socket not created");
        exit(1);
    }
    puts("socket created");

    // bind args
    if (bind(server->handle, server->address, server->addressLength) < 0) {
        perror("bind failed");
        exit(2);
    }

    for (;;) {
        // receive data
        if (recvfrom(server->handle, clientMessage, MESSAGE_STRING_LENGTH, IPV6_V6ONLY, client->address, &client->addressLength) >= 0) {
            printf("received: %s\n", clientMessage);

            // send data
            getRandomQuote(quote);
            printf("sending: %s\n", quote);
            if (sendto(server->handle, quote, (long) strlen(quote), IPV6_V6ONLY, client->address, client->addressLength) == -1) {
                perror("[UDP: SENDTO]");
            }
        } else {
            perror("[UDP: RECVFROM]");
        }
    }
}

void tcpListen(void * args[]) {
    struct sockinfo * server = (struct sockinfo *) args[0];
    struct sockinfo * client = (struct sockinfo *) args[1];
    char quote[MESSAGE_STRING_LENGTH] = "", clientMessage[MESSAGE_STRING_LENGTH] = "";

    // create args
    server->handle = socket(server->address->sa_family, SOCK_STREAM, IPPROTO_TCP);
    if (server->handle == -1) {
        perror("socket not created");
        exit(1);
    }
    puts("socket created");

    // bind args
    if (bind(server->handle, server->address, server->addressLength) < 0) {
        perror("bind failed");
        exit(2);
    }

    listen(server->handle, 5);
    puts("server listening");

    // accept connections
    //TODO: t h r e a d s
    while ((client->handle = accept(server->handle, client->address, &client->addressLength)) >= 0) {
        puts("client connected");

        // receive data
        if (recv(client->handle, clientMessage, MESSAGE_STRING_LENGTH, IPV6_V6ONLY) >= 0) {
            printf("client says: %s\n", clientMessage);

            // send data
            getRandomQuote(quote);
            printf("responding: \"%s\"\n", quote);
            if (send(client->handle, quote, (long) strlen(quote), IPV6_V6ONLY) == -1) {
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
        exit(3);
    }
}

void server(int tcp, int ipv6) {
    struct sockinfo client4, client6;
    struct sockinfo udp4, udp6, tcp4, tcp6;
    struct sockaddr_in serverAddress4, clientAddress4;
    struct sockaddr_in6 serverAddress6, clientAddress6;

    serverAddress6.sin6_family = AF_INET6;
    serverAddress6.sin6_addr = in6addr_any;
    serverAddress6.sin6_port = htons(DEFAULT_LISTEN_PORT);

    tcp6.address = (struct sockaddr *) &serverAddress6;
    udp6.address = (struct sockaddr *) &serverAddress6;
    tcp6.addressLength = sizeof serverAddress6;
    udp6.addressLength = sizeof serverAddress6;

    clientAddress6.sin6_family = AF_INET6;
    client6.address = (struct sockaddr *) &clientAddress6;
    client6.addressLength = sizeof clientAddress6;

    serverAddress4.sin_family = AF_INET;
    serverAddress4.sin_addr.s_addr = INADDR_ANY;
    serverAddress4.sin_port = htons(DEFAULT_LISTEN_PORT);

    tcp4.address = (struct sockaddr *) &serverAddress4;
    udp4.address = (struct sockaddr *) &serverAddress4;
    tcp4.addressLength = sizeof serverAddress4;
    udp4.addressLength = sizeof serverAddress4;

    clientAddress4.sin_family = AF_INET;
    client4.address = (struct sockaddr *) &clientAddress4;
    client4.addressLength = sizeof clientAddress4;

    void * tcp6Args[] = {&tcp6, &client6};
    void * udp6Args[] = {&udp6, &client6};
    void * tcp4Args[] = {&tcp4, &client4};
    void * udp4Args[] = {&udp4, &client4};

    // TODO: threads to run both at the same time
    pthread_t tcp6Handle, udp6Handle, tcp4Handle, udp4Handle;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tcp6Handle, &attr, (void *(*)(void *)) tcpListen, tcp6Args);
    pthread_create(&udp6Handle, &attr, (void *(*)(void *)) udpListen, udp6Args);
//    pthread_create(&tcp4Handle, &attr, (void *(*)(void *)) tcpListen, tcp4Args);
//    pthread_create(&udp4Handle, &attr, (void *(*)(void *)) udpListen, udp4Args);

    void * tcp6Return, * udp6Return, * tcp4Return, * udp4Return;
    pthread_join(tcp6Handle, tcp6Return);
    pthread_join(udp6Handle, udp6Return);
//    pthread_join(tcp4Handle, tcp4Return);
//    pthread_join(udp4Handle, udp4Return);
}
