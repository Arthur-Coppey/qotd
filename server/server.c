//
// Created by k0rb4k on 30/03/2021.
//

#include "server.h"

//TODO: object-like C

extern int errno;

_Noreturn void udpListen(int serverSocket, struct sockaddr * clientAddress, unsigned long addressLength) {
    int recvLen;
    char clientMessage[MESSAGE_STRING_LENGTH], quote[MESSAGE_STRING_LENGTH];

    for (;;) {
        // receive data
        recvLen = recvfrom(serverSocket, clientMessage, MESSAGE_STRING_LENGTH, 0, clientAddress, (socklen_t *) &addressLength);

        perror("recvfrom");
        if (recvLen >= 0) {
            printf("client says: %s\n", clientMessage);

            // send data
            getRandomQuote(quote);
            printf("responding: `%s`\n", quote);
            sendto(serverSocket, quote, strlen(quote), 0, clientAddress, addressLength);
            perror("sendto");
        }
    }
}

void tcpListen(int serverSocket, struct sockaddr * clientAddress, unsigned long addressLength) {
    int clientSocket;
    char quote[MESSAGE_STRING_LENGTH] = "", clientMessage[MESSAGE_STRING_LENGTH] = "";

    listen(serverSocket, 5);
    puts("server listening");

    // accept connections
    //TODO: t h r e a d s
    while ((clientSocket = accept(serverSocket, clientAddress, (socklen_t *) &addressLength)) >=
           0) {
        puts("client connected");

        // receive data
        while (recv(clientSocket, clientMessage, MESSAGE_STRING_LENGTH, 0) >= 0) {
            printf("client says: %s\n", clientMessage);

            // send data
            getRandomQuote(quote);
            printf("responding: \"%s\"\n", quote);
            send(clientSocket, quote, strlen(quote), 0);
        }

        puts("client disconnected");
    }

    if (clientSocket < 0) {
        puts("client connection failed");
        exit(3);
    }
}

void server(int tcp, int ipv6) {
    int serverSocket, socketType, socketProtocol;
    unsigned long serverAddressLength, clientAddressLength;
    struct sockaddr *serverAddress, *clientAddress;
    struct sockaddr_in serverAddress4, clientAddress4;
    struct sockaddr_in6 serverAddress6, clientAddress6;

    if (ipv6) {
        serverAddress6.sin6_family = AF_INET6;
        serverAddress6.sin6_addr = in6addr_any;
        serverAddress6.sin6_port = htons(17);
        serverAddress = (struct sockaddr *) &serverAddress6;
        serverAddressLength = sizeof serverAddress6;

        clientAddress6.sin6_family = AF_INET6;
        clientAddress = (struct sockaddr *) &clientAddress6;
        clientAddressLength = sizeof clientAddress6;
    } else {
        serverAddress4.sin_family = AF_INET;
        serverAddress4.sin_addr.s_addr = INADDR_ANY;
        serverAddress4.sin_port = htons(17);
        serverAddress = (struct sockaddr *) &serverAddress4;
        serverAddressLength = sizeof serverAddress4;

        clientAddress4.sin_family = AF_INET;
        clientAddress = (struct sockaddr *) &clientAddress4;
        clientAddressLength = sizeof clientAddress4;
    }

    if (tcp) {
        socketType = SOCK_STREAM;
        socketProtocol = IPPROTO_TCP;
    } else {
        socketType = SOCK_DGRAM;
        socketProtocol = IPPROTO_UDP;
    }

    // create sock
    serverSocket = socket(serverAddress->sa_family, socketType, socketProtocol);
    if (serverSocket == -1) {
        puts("socket not created");
        exit(1);
    }
    puts("socket created");

    // bind sock
    if (bind(serverSocket, serverAddress, serverAddressLength) < 0) {
        puts("bind failed");
        exit(2);
    }

    // TODO: threads to run both at the same time
    if (tcp) {
        tcpListen(serverSocket, clientAddress, clientAddressLength);
    } else {
        udpListen(serverSocket, clientAddress, clientAddressLength);
    }
}
