#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex_A;
pthread_mutex_t mutex_B;
pthread_mutex_t mutex_C; // Dodany trzeci mutex

void* watek1(void* arg) {
    pthread_mutex_lock(&mutex_A);  // zawsze najpierw A
    pthread_mutex_lock(&mutex_B);  // potem B
    pthread_mutex_lock(&mutex_C);  // na koncu C
    printf("Watek 1: zajal wszystkie trzy mutexy (A, B, C)\n");
    
    // Zwalnianie w odwrotnej kolejnosci
    pthread_mutex_unlock(&mutex_C);
    pthread_mutex_unlock(&mutex_B);
    pthread_mutex_unlock(&mutex_A);
    return NULL;
}

void* watek2(void* arg) {
    pthread_mutex_lock(&mutex_A);  // rowniez najpierw A
    pthread_mutex_lock(&mutex_B);  // potem B
    pthread_mutex_lock(&mutex_C);  // na koncu C -- ta sama kolejnosc!
    printf("Watek 2: zajal wszystkie trzy mutexy (A, B, C)\n");
    
    // Zwalnianie w odwrotnej kolejnosci
    pthread_mutex_unlock(&mutex_C);
    pthread_mutex_unlock(&mutex_B);
    pthread_mutex_unlock(&mutex_A);
    return NULL;
}

int main(void) {
    pthread_mutex_init(&mutex_A, NULL);
    pthread_mutex_init(&mutex_B, NULL);
    pthread_mutex_init(&mutex_C, NULL); // Inicjalizacja mutexu C
    
    pthread_t t1, t2;
    pthread_create(&t1, NULL, watek1, NULL);
    pthread_create(&t2, NULL, watek2, NULL);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    pthread_mutex_destroy(&mutex_A);
    pthread_mutex_destroy(&mutex_B);
    pthread_mutex_destroy(&mutex_C); // Zniszczenie mutexu C
    
    printf("Gotowe -- brak deadlocka przy 3 mutexach!\n");
    return 0;
}