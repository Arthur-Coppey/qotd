//
// Created by k0rb4k on 30/03/2021.
//

#ifndef QOTD_SERVER_H
#define QOTD_SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "quote.h"

#define MESSAGE_STRING_LENGTH 2048

void tcpServer();

void udpServer();

#endif //QOTD_SERVER_H
