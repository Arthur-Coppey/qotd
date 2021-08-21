//
// Created by k0rb4k on 30/03/2021.
//

#ifndef QOTD_QUOTE_H
#define QOTD_QUOTE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bits/types/FILE.h>
#include <bits/types/struct_FILE.h>

#define QUOTES_FILE "./quotes.txt"

void getRandomQuote(char quote[2048]);

void getQuote(long line, char quote[2048]);

int getFileLineCount(char* filename);

#endif //QOTD_QUOTE_H
