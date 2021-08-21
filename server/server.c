//
// Created by k0rb4k on 30/03/2021.
//

#include "server.h"

void tcpServer() {
    int serverSocket, clientSocket;
    long read_size;
    unsigned long addressLength;
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
    addressLength = sizeof(struct sockaddr_in);
    while ((clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, (socklen_t *) &addressLength)) >= 0) {
        puts("client connected");

        /*receive data*/
        while ((read_size = recv(clientSocket, clientMessage, MESSAGE_STRING_LENGTH, 0))) {
            if (read_size < 0) {
                puts("receive failed");
            } else {
                printf("client says: %s\n", clientMessage);

                /*send data*/
                getRandomQuote(quote);
                printf("responding: \"%s\"\n", quote);
                write(clientSocket, quote, strlen(quote));
            }
        }

        puts("client disconnected");
    }

    if (clientSocket < 0) {
        puts("client connection failed");
        exit(3);
    }
}

void udpServer() {
    //TODO: udp handling
}
