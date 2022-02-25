//
// Created by k0rb4k on 30/03/2021.
//

#ifndef QOTD_SERVER_H
#define QOTD_SERVER_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>

#include "quote.h"

struct sockinfo {
    int handle;
    struct sockaddr * address;
    unsigned long addressLength;
};

void server(int tcp, int ipv6);

void tcpListen(int serverSocket, struct sockaddr *clientAddress, unsigned long addressLength);

_Noreturn void udpListen(int serverSocket, struct sockaddr *clientAddress, unsigned long addressLength);

#endif //QOTD_SERVER_H
