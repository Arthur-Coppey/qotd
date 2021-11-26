//
// Created by k0rb4k on 30/03/2021.
//

#ifndef QOTD_SERVER_H
#define QOTD_SERVER_H

#include <sys/socket.h>
//#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "quote.h"

#define MESSAGE_STRING_LENGTH 2048

void server(int tcp, int ipv6);

void tcpServer();

_Noreturn void udpServer();

#endif //QOTD_SERVER_H
