#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex_A;
pthread_mutex_t mutex_B;

void* watek1(void* arg) {
    pthread_mutex_lock(&mutex_A);  // 1. zajmuje A
    printf("Watek 1: zajal A, czeka na B...\n");
    usleep(100000);                // dajemy szanse watek2 na zajecie B
    pthread_mutex_lock(&mutex_B);  // 2. czeka na B -- DEADLOCK!
    printf("Watek 1: zajal oba\n");
    pthread_mutex_unlock(&mutex_B);
    pthread_mutex_unlock(&mutex_A);
    return NULL;
}

void* watek2(void* arg) {
    pthread_mutex_lock(&mutex_B);  // 1. zajmuje B
    printf("Watek 2: zajal B, czeka na A...\n");
    usleep(100000);
    pthread_mutex_lock(&mutex_A);  // 2. czeka na A -- DEADLOCK!
    printf("Watek 2: zajal oba\n");
    pthread_mutex_unlock(&mutex_A);
    pthread_mutex_unlock(&mutex_B);
    return NULL;
}

int main(void) {
    pthread_mutex_init(&mutex_A, NULL);
    pthread_mutex_init(&mutex_B, NULL);
    pthread_t t1, t2;
    pthread_create(&t1, NULL, watek1, NULL);
    pthread_create(&t2, NULL, watek2, NULL);
    pthread_join(t1, NULL); // program zawiesi sie tutaj na zawsze
    pthread_join(t2, NULL);
    return 0;
}