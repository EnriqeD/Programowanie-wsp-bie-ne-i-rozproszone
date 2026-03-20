#include <pthread.h>
#include <stdio.h>

#define WATKI    4
#define ITERACJE 1000000

long licznik = 0;
pthread_mutex_t mutex; // mutex chroniacy licznik

void* modyfikuj(void* arg) {
    for (int i = 0; i < ITERACJE; i++) {
        // Sekcja krytyczna - inkrementacja
        pthread_mutex_lock(&mutex);
        licznik++;
        pthread_mutex_unlock(&mutex);

        // Sekcja krytyczna - dekrementacja
        pthread_mutex_lock(&mutex);
        licznik--;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(void) {
    pthread_mutex_init(&mutex, NULL); // inicjalizuj mutex przed uzyciem!

    pthread_t tids[WATKI];
    for (int i = 0; i < WATKI; i++)
        pthread_create(&tids[i], NULL, modyfikuj, NULL);
        
    for (int i = 0; i < WATKI; i++)
        pthread_join(tids[i], NULL);

    // Skoro kazdy watek robi tyle samo operacji ++ co --, oczekujemy 0
    printf("Oczekiwano: 0\n");
    printf("Otrzymano:  %ld\n", licznik); // przy poprawnym uzyciu mutexu zawsze 0
    
    pthread_mutex_destroy(&mutex);
    return 0;
}