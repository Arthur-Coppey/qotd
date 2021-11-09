//
// Created by k0rb4k on 30/03/2021.
//

#include "server.h"

//TODO: refactor
//TODO: object-like C

extern int errno;

unsigned long addressLength = sizeof(struct sockaddr_in);
long recvLen;
int sock, serverSocket, clientSocket, socketType, socketProtocol;
struct sockaddr * srvAddr;
struct sockaddr_in serverAddress, clientAddress;
struct sockaddr_in serverAddress4, clientAddress4;
struct sockaddr_in6 serverAddress6, clientAddress6;
char quote[MESSAGE_STRING_LENGTH] = "";
char clientMessage[MESSAGE_STRING_LENGTH] = "";

void server(int tcp, int ipv6) {
    // TODO: things common between tcp/udp here
    // init sock info
    memset(&serverAddress, 0, addressLength);
    memset(&clientAddress, 0, addressLength);

    if (ipv6) {
        serverAddress6.sin6_family = AF_INET6;
        serverAddress6.sin6_addr = in6addr_any;
        serverAddress6.sin6_port = htons(17);
        srvAddr = (struct sockaddr *) &serverAddress6;
    } else {
        serverAddress4.sin_family = AF_INET;
        serverAddress4.sin_addr.s_addr = INADDR_ANY;
        serverAddress4.sin_port = htons(17);
        srvAddr = (struct sockaddr *) &serverAddress4;
    }

    // create sock
    if (tcp) {
        socketType = SOCK_STREAM;
        socketProtocol = IPPROTO_TCP;
    } else {
        socketType = SOCK_DGRAM;
        socketProtocol = IPPROTO_UDP;
    }

    serverSocket = socket(srvAddr->sa_family, socketType, socketProtocol);
    if (serverSocket == -1) {
        puts("socket not created");
        exit(1);
    }
    puts("socket created");

    // bind sock
    if (bind(serverSocket, srvAddr, addressLength) < 0) {
        puts("bind failed");
        exit(2);
    }

    // TODO: udp/tcp

}

void tcpServer() {
    /*initialize socket info*/
    memset(&serverAddress, 0, addressLength);
    memset(&clientAddress, 0, addressLength);

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(17);

    /*create socket*/
    serverSocket = socket(serverAddress.sin_family, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == -1) {
        puts("socket not created");
        exit(1);
    }
    puts("socket created");

    /*bind socket to address:port*/
    if (bind(serverSocket, (const struct sockaddr *) &serverAddress, addressLength) < 0) {
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
    if (bind(sock, (const struct sockaddr *) &serverAddress, addressLength) < 0) {
        puts("bind failed");
        exit(2);
    }
    puts("address bound");

    //TODO: t h r e a d s ?
    for (;;) {
        /*receive data*/
        recvLen = recvfrom(sock, clientMessage, MESSAGE_STRING_LENGTH, 0,
                               (struct sockaddr *) &clientAddress, (socklen_t *) &addressLength);

        uint16_t port = ntohs(clientAddress.sin_port);
        char *ipv4 = inet_ntoa(clientAddress.sin_addr);

        printf("recvLen = %ld from = %s:%d\nmsg = `%s`\n", recvLen, ipv4, port, clientMessage);

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
