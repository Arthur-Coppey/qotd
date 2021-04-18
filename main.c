#include "quote.h"
#include "server.h"

int main(int argc, char *argv[]) {
    char quote[1024];
    getRandomQuote(quote);
    printf("%s\n", quote);

    return 0;
}
