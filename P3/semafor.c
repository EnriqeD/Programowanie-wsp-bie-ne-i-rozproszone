#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define PRODUCENTOW 3
#define KONSUMENTOW 2
#define BUFOR_MAX   5

// Klasyczny problem producent-konsument z semaforem
sem_t sem_wolne;   // ile jest wolnych miejsc w buforze
sem_t sem_zajete;  // ile jest elementow do pobrania
pthread_mutex_t mutex_bufor;

int bufor[BUFOR_MAX];
int wpisz = 0, odczytaj = 0;
int elementow_w_buforze = 0; // Dodany licznik elementow

void* producent(void* arg) {
    int id = *(int*)arg;

    for (int i = 0; i < 4; i++) {
        int element = id * 10 + i;

        sem_wait(&sem_wolne);           // czekaj na wolne miejsce
        pthread_mutex_lock(&mutex_bufor);
        
        bufor[wpisz] = element;
        wpisz = (wpisz + 1) % BUFOR_MAX;
        elementow_w_buforze++; // Zwieksz licznik po dodaniu
        
        printf("[P%d] wpisano: %d | W buforze: %d\n", id, element, elementow_w_buforze);
        
        pthread_mutex_unlock(&mutex_bufor);
        sem_post(&sem_zajete);          // zasygnalizuj nowy element
        usleep(100000);
    }
    return NULL;
}

void* konsument(void* arg) {
    int id = *(int*)arg;
    // Calkowita liczba elementow to PRODUCENTOW * 4. 
    // Dzielimy ja przez liczbe konsumentow, wiec kazdy pobierze polowe (po 6 elementow).
    int ile_pobrac = (PRODUCENTOW * 4) / KONSUMENTOW;

    for (int i = 0; i < ile_pobrac; i++) {
        sem_wait(&sem_zajete);          // czekaj az bedzie co pobierac
        pthread_mutex_lock(&mutex_bufor);
        
        int element = bufor[odczytaj];
        odczytaj = (odczytaj + 1) % BUFOR_MAX;
        elementow_w_buforze--; // Zmniejsz licznik po pobraniu
        
        printf("[K%d] pobrano: %d | W buforze: %d\n", id, element, elementow_w_buforze);
        
        pthread_mutex_unlock(&mutex_bufor);
        sem_post(&sem_wolne);           // zwolnij miejsce w buforze
        usleep(150000);
    }
    return NULL;
}

int main(void) {
    sem_init(&sem_wolne, 0, BUFOR_MAX); // na poczatku bufor pusty
    sem_init(&sem_zajete, 0, 0);        // brak elementow
    pthread_mutex_init(&mutex_bufor, NULL);

    pthread_t prod[PRODUCENTOW], kons[KONSUMENTOW];
    int ids_prod[PRODUCENTOW];
    int ids_kons[KONSUMENTOW];

    // Tworzenie watkow producentow
    for (int i = 0; i < PRODUCENTOW; i++) {
        ids_prod[i] = i + 1;
        pthread_create(&prod[i], NULL, producent, &ids_prod[i]);
    }
    
    // Tworzenie watkow konsumentow
    for (int i = 0; i < KONSUMENTOW; i++) {
        ids_kons[i] = i + 1;
        pthread_create(&kons[i], NULL, konsument, &ids_kons[i]);
    }

    // Oczekiwanie na zakonczenie watkow producentow
    for (int i = 0; i < PRODUCENTOW; i++) {
        pthread_join(prod[i], NULL);
    }
    
    // Oczekiwanie na zakonczenie watkow konsumentow
    for (int i = 0; i < KONSUMENTOW; i++) {
        pthread_join(kons[i], NULL);
    }

    sem_destroy(&sem_wolne);
    sem_destroy(&sem_zajete);
    pthread_mutex_destroy(&mutex_bufor);
    return 0;
}