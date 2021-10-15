//
// Created by k0rb4k on 30/03/2021.
//

#include "server.h"

//TODO: refactor
//TODO: object-like C

extern int errno;

unsigned long addressLength = sizeof(struct sockaddr_in);


void tcpServer() {
    int serverSocket, clientSocket;
    long recvLen;
//    unsigned long addressLength = sizeof(struct sockaddr_in);
    struct sockaddr_in serverAddress, clientAddress;
    char quote[MESSAGE_STRING_LENGTH] = "";
    char clientMessage[MESSAGE_STRING_LENGTH] = "";

    /*initialize socket info*/
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(17);

    /*create socket*/
    serverSocket = socket(serverAddress.sin_family, SOCK_STREAM, IPPROTO_IP);
    if (serverSocket == -1) {
        puts("socket not created");
        exit(1);
    }
    puts("socket created");

    /*bind socket to address:port*/
    if (bind(serverSocket, (const struct sockaddr *) &serverAddress, sizeof serverAddress) < 0) {
        puts("bind failed");
        exit(2);
    }
    puts("address bound");

    /*start listening*/
    listen(serverSocket, 5);
    puts("server listening");

    /*accept connections*/
    //TODO: t h r e a d s
    while ((clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, (socklen_t *) &addressLength)) >=
           0) {
        puts("client connected");

        /*receive data*/
        while ((recvLen = recv(clientSocket, clientMessage, MESSAGE_STRING_LENGTH, 0))) {
            if (recvLen < 0) {
                puts("receive failed");
            } else {
                printf("client says: %s\n", clientMessage);

                /*send data*/
                getRandomQuote(quote);
                printf("responding: \"%s\"\n", quote);
                send(clientSocket, quote, strlen(quote), 0);
            }
        }

        puts("client disconnected");
    }

    if (clientSocket < 0) {
        puts("client connection failed");
        exit(3);
    }
}

_Noreturn void udpServer() {
    int sock;
//    unsigned long addressLength = sizeof(struct sockaddr_in);
    struct sockaddr_in serverAddress, clientAddress;
    char quote[MESSAGE_STRING_LENGTH] = "";
    char clientMessage[MESSAGE_STRING_LENGTH] = "";

    /*initialize sock info*/
    memset(&serverAddress, 0, addressLength);
    memset(&clientAddress, 0, addressLength);

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(17);

    /*create sock*/
    sock = socket(serverAddress.sin_family, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == -1) {
        puts("sock not created");
        exit(1);
    }
    puts("sock created");

    /*bind sock to address:port*/
    if (bind(sock, (const struct sockaddr *) &serverAddress, sizeof serverAddress) < 0) {
        puts("bind failed");
        exit(2);
    }
    puts("address bound");

    /*accept connections*/
    //TODO: t h r e a d s ?
    while (1) {
        /*receive data*/
        int recvLen = recvfrom(sock, clientMessage, MESSAGE_STRING_LENGTH, 0,
                               (struct sockaddr *) &clientAddress, (socklen_t *) &addressLength);

        uint16_t port = ntohs(clientAddress.sin_port);
        char *ipv4 = inet_ntoa(clientAddress.sin_addr);

        printf("recvLen = %d from = %s:%d\nmsg = `%s`\n", recvLen, ipv4, port, clientMessage);

        perror("recvfrom");
        if (recvLen >= 0) {
            printf("client says: %s\n", clientMessage);

            /*send data*/
            getRandomQuote(quote);
            printf("responding: `%s`\n", quote);
            sendto(sock, strcat(quote, "\n"), sizeof quote, 0, (struct sockaddr *) &clientAddress, addressLength);
            perror("sendto");
        }
    }
}
