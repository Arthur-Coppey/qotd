//
// Created by k0rb4k on 30/03/2021.
//

#include "server.h"
#include "config.h"

//TODO: object-like C

extern int errno;

_Noreturn void udpListen(void * sock) {
    struct sockinfo * socket = (struct sockinfo *) sock;

    char clientMessage[MESSAGE_STRING_LENGTH], quote[MESSAGE_STRING_LENGTH];
    int serverSocket = socket->handle;
    struct sockaddr * clientAddress = socket->address;
    unsigned long addressLength = socket->addressLength;

    for (;;) {
        // receive data

        if (recvfrom(serverSocket, clientMessage, MESSAGE_STRING_LENGTH, 0, clientAddress, (socklen_t *) &addressLength) >= 0) {
            printf("received: %s\n", clientMessage);

            // send data
            getRandomQuote(quote);
            printf("sending: %s\n", quote);
            if (sendto(serverSocket, quote, strlen(quote), 0, clientAddress, addressLength) == -1) {
                perror("udp: sendto");
            }
        } else {
            perror("udp: recvfrom");
        }
    }
}

void tcpListen(void * sock) {
    struct sockinfo * socket = (struct sockinfo *) sock;

    int clientSocket;
    char quote[MESSAGE_STRING_LENGTH] = "", clientMessage[MESSAGE_STRING_LENGTH] = "";
    int serverSocket = socket->handle;
    struct sockaddr * clientAddress = socket->address;
    unsigned long addressLength = socket->addressLength;

    listen(serverSocket, 5);
    puts("server listening");

    // accept connections
    //TODO: t h r e a d s
    while ((clientSocket = accept(serverSocket, clientAddress, (socklen_t *) &addressLength)) >= 0) {
        puts("client connected");

        // receive data
        if (recv(clientSocket, clientMessage, MESSAGE_STRING_LENGTH, 0) >= 0) {
            printf("client says: %s\n", clientMessage);

            // send data
            getRandomQuote(quote);
            printf("responding: \"%s\"\n", quote);
            if (send(clientSocket, quote, strlen(quote), 0) == -1) {
                perror("tcp: send");
            }
        } else {
            perror("tcp: recv");
        }

        close(clientSocket);

        puts("closed connection");
    }

    if (clientSocket < 0) {
        puts("client connection failed");
        exit(3);
    }
}

void server(int tcp, int ipv6) {
    int socketType, socketProtocol;
    struct sockinfo server, client;
    struct sockaddr_in serverAddress4, clientAddress4;
    struct sockaddr_in6 serverAddress6, clientAddress6;

    if (ipv6) {
        serverAddress6.sin6_family = AF_INET6;
        serverAddress6.sin6_addr = in6addr_any;
        serverAddress6.sin6_port = htons(DEFAULT_LISTEN_PORT);

        server.address = (struct sockaddr *) &serverAddress6;
        server.addressLength = sizeof serverAddress6;

        clientAddress6.sin6_family = AF_INET6;
        client.address = (struct sockaddr *) &clientAddress6;
        client.addressLength = sizeof clientAddress6;
    } else {
        serverAddress4.sin_family = AF_INET;
        serverAddress4.sin_addr.s_addr = INADDR_ANY;
        serverAddress4.sin_port = htons(DEFAULT_LISTEN_PORT);

        server.address = (struct sockaddr *) &serverAddress4;
        server.addressLength = sizeof serverAddress4;

        clientAddress4.sin_family = AF_INET;
        client.address = (struct sockaddr *) &clientAddress4;
        client.addressLength = sizeof clientAddress4;
    }

    if (tcp) {
        socketType = SOCK_STREAM;
        socketProtocol = IPPROTO_TCP;
    } else {
        socketType = SOCK_DGRAM;
        socketProtocol = IPPROTO_UDP;
    }

    // create sock
    server.handle = socket(server.address->sa_family, socketType, socketProtocol);
    if (server.handle == -1) {
        puts("socket not created");
        exit(1);
    }
    puts("socket created");

    // bind sock
    if (bind(server.handle, server.address, server.addressLength) < 0) {
        puts("bind failed");
        exit(2);
    }

    // TODO: threads to run both at the same time
    pthread_t tcpHandle, udpHandle;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tcpHandle, &attr, (void *(*)(void *)) tcpListen, (void *) &server);
    pthread_create(&udpHandle, &attr, (void *(*)(void *)) udpListen, (void *) &server);
    void * tcpReturn, * udpReturn;
    pthread_join(tcpHandle, tcpReturn);
    pthread_join(udpHandle, udpReturn);
//    if (tcp) {
//        tcpListen(&server);
//    } else {
//        udpListen(&server);
//    }
}
