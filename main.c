#include "server.h"
#include <pthread.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    // TODO: shell args
//    pthread_t threadHandle;
//    pthread_attr_t attr;
//    pthread_attr_init(&attr);
//    pthread_create(&threadHandle, &attr, (void *(*)(void *)) threadTest, "a");
//    void * ret;
//    pthread_join(threadHandle, &ret);
//    printf("%d", ret);
    server(1, 1);

    return 0;
}
