//
// Created by k0rb4k on 30/03/2021.
//

#include "server.h"

void tcpServer() {
    int serverSocket, clientSocket;
    unsigned long addressLength;
    struct sockaddr_in serverAddress, clientAddress;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(17);

    serverSocket = socket(serverAddress.sin_family, SOCK_STREAM, IPPROTO_IP);
    if (serverSocket == -1) {
        puts("socket not created");
        exit(1);
    }
    puts("socket created");

    if (bind(serverSocket, (const struct sockaddr *) &serverAddress, sizeof serverAddress) < 0) {
        puts("bind failed");
        exit(2);
    }

    listen(serverSocket, 5);

    addressLength = sizeof(struct sockaddr_in);
    clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, (socklen_t *) addressLength);
    if (clientSocket < 0) {
        puts("client connection failed");
        exit(3);
    }
    puts("client connected");

    write(clientSocket, "test", strlen("test"));
    // TODO: listen and respond
}

void udpServer() {

}
