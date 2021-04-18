//
// Created by k0rb4k on 30/03/2021.
//

#include "quote.h"

void getRandomQuote(char quote[1024]) {
    // TODO: true random between 0 and number of lines in file
    int rand = 0;
    getQuote(rand, quote);
}

void getQuote(int line, char quote[1024]) {
    FILE* list;
//    char quote[1024];

    // TODO: get specified x line

    list = fopen("quotes.txt", "r");

    fgets(quote, 1024, list);

    fclose(list);
}
