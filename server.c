//
// Created by k0rb4k on 30/03/2021.
//

#include "server.h"

void tcpListen() {
    int serverFd;
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(17);

    serverFd = socket(AF_INET, SOCK_STREAM, 0);

    bind(serverFd, (const struct sockaddr *) &address, sizeof address);
    // TODO: listen and respond
}

void udpListen() {

}
