#include <pthread.h>
#include <stdio.h>

#define WATKI    4
#define ITERACJE 1000000

long licznik = 0; // wspoldzielona zmienna - BRAK OCHRONY!

void* inkrementuj(void* arg) {
    for (int i = 0; i < ITERACJE; i++) {
        licznik++; // NIE jest atomowe! READ-MODIFY-WRITE
    }
    return NULL;
}

int main(void) {
    pthread_t tids[WATKI];
    for (int i = 0; i < WATKI; i++)
        pthread_create(&tids[i], NULL, inkrementuj, NULL);
    for (int i = 0; i < WATKI; i++)
        pthread_join(tids[i], NULL);

    printf("Oczekiwano: %d\n", WATKI * ITERACJE);
    printf("Otrzymano:  %ld\n", licznik); // rozny wynik przy kazdym uruchomieniu!
    return 0;
}