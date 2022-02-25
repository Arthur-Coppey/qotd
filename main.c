//#include "server.h"
#include <pthread.h>
#include <stdio.h>
void threadTest(void *arg) {
    char* txt = (char*) arg;
    puts(txt);
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    // TODO: shell args
    pthread_t threadHandle;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&threadHandle, &attr, (void *(*)(void *)) threadTest, "a");
    void * ret;
    pthread_join(threadHandle, &ret);
    puts(ret);
//    server(1, 1);

    return 0;
}
