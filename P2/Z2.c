#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define LICZBA_WATKOW 6 

void* pracownik(void* arg) {
    int id = *(int*)arg;
    printf("[Watek %d] Rozpoczynam prace...\n", id);
    
    // Obliczanie n (numer watku * 1000)
    int n = id * 1000;
    long long suma = 0;
    
    // Obliczanie sumy od 1 do n
    for (int i = 1; i <= n; i++) {
        suma += i;
    }
    
    usleep(500000 * id);   // Opoznienie: mikrosekundy (500ms * id)
    
    printf("[Watek %d] Praca zakonczona! Suma od 1 do %d wynosi: %lld\n", id, n, suma);
    
    return NULL;
}

int main(void) {
    pthread_t tids[LICZBA_WATKOW];
    int       parametry[LICZBA_WATKOW];

    for (int i = 0; i < LICZBA_WATKOW; i++) {
        parametry[i] = i + 1;
        int ret = pthread_create(&tids[i], NULL, pracownik, &parametry[i]);
        if (ret != 0) {
            printf("Blad tworzenia watku %d: %d\n", i + 1, ret);
            return 1;
        }
    }

    printf("[Main] Czekam na zakonczenie watkow...\n");

    for (int i = 0; i < LICZBA_WATKOW; i++) {
        pthread_join(tids[i], NULL);
    }

    printf("[Main] Wszystko gotowe!\n");
    return 0;
}