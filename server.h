//
// Created by k0rb4k on 30/03/2021.
//

#ifndef QOTD_SERVER_H
#define QOTD_SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>

void tcpListen();

void udpListen();

#endif //QOTD_SERVER_H
