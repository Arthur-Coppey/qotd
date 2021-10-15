//
// Created by k0rb4k on 11/10/2021.
//

#ifndef QOTD_CLIENT_H
#define QOTD_CLIENT_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int tcp();

int udp();

#endif //QOTD_CLIENT_H
