//
// Created by k0rb4k on 30/03/2021.
//

#include "quote.h"
#include "config.h"

//TODO: error handling

void getRandomQuote(char quote[2048]) {
    int lineCount;
    long line;

    if ((lineCount = getFileLineCount(QUOTES_FILE)) == -1) {
        puts("file does not exist");
        exit(-1);
    }

    line = random() % lineCount;

    getQuote(line, quote);
}

void getQuote(long line, char quote[2048]) {
    FILE* list;

    list = fopen(QUOTES_FILE, "r");

    /*skip lines*/
    while (line > 0) {
        if (fgetc(list) == '\n') {
            line--;
        }
    }
    fgets(quote, 2048, list);
    quote[strlen(quote) - 1] = '\0';

    fclose(list);
}

int getFileLineCount(char *filename) {
    char c;
    int lineCount = 0;
    FILE* quotesFile = fopen(filename, "r");

    if (quotesFile == NULL) {
        printf("no quote file found at \"%s\"\n", filename);
        return -1;
    }

    while ((c = (char) fgetc(quotesFile)) != EOF) {
        if (c == '\n') {
            lineCount++;
        }
    }

    fclose(quotesFile);

    return lineCount;
}
