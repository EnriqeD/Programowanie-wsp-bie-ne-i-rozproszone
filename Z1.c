#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* funkcja_watku(void* arg) {
    int numer = *(int*)arg;
    
    for (int i = 0; i < 3; i++) {
        printf("Witaj z watku numer %d!\n", numer);
        usleep(300000);
    }
    
    return NULL;
}

int main(void) {
    pthread_t tid;
    int param = 1;
    
    printf("Glowny watek startuje.\n");
    
    int ret = pthread_create(&tid, NULL, funkcja_watku, &param);
    if (ret != 0) {
        printf("Blad tworzenia watku: %d\n", ret);
        return 1;
    }
    
    printf("Glowny watek konczy prace. Koniec programu.\n");
    
    return 0;
}