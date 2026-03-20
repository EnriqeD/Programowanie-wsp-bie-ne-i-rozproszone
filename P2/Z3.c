#include <pthread.h>
#include <stdio.h>

#define LICZBA_WATKOW 4

// 1. Definicja struktury DaneWatku
typedef struct {
    int id;           // Numer watku
    int n;            // Liczba do obliczenia silni
    long long wynik;  // Miejsce na wynik
} DaneWatku;

// Funkcja wykonywana przez kazdy watek
void* oblicz_silnie(void* arg) {
    // Rzutowanie wskaznika na typ naszej struktury
    DaneWatku* dane = (DaneWatku*)arg;
    
    long long silnia = 1;
    
    // Obliczanie n!
    for (int i = 1; i <= dane->n; i++) {
        silnia *= i;
    }
    
    // Zapisanie wyniku do pola w strukturze
    dane->wynik = silnia; 
    
    return NULL;
}

int main(void) {
    pthread_t tids[LICZBA_WATKOW];
    DaneWatku dane_watkow[LICZBA_WATKOW];
    
    // Przykładowe wartości n do policzenia (zgodnie z poleceniem)
    int wartosci_n[LICZBA_WATKOW] = {5, 8, 10, 12};
    
    // 2. Tworzenie 4 watkow
    for (int i = 0; i < LICZBA_WATKOW; i++) {
        dane_watkow[i].id = i + 1;
        dane_watkow[i].n = wartosci_n[i];
        dane_watkow[i].wynik = 0; // Inicjalizacja zerem
        
        // Przekazujemy do watku wskaznik na odpowiedni element tablicy struktur
        int ret = pthread_create(&tids[i], NULL, oblicz_silnie, &dane_watkow[i]);
        if (ret != 0) {
            printf("Blad tworzenia watku %d: %d\n", i + 1, ret);
            return 1;
        }
    }
    
    // Czekamy na zakonczenie pracy wszystkich watkow
    for (int i = 0; i < LICZBA_WATKOW; i++) {
        pthread_join(tids[i], NULL);
    }
    
    // 3. Wypisanie wynikow w glownym watku
    printf("--- Wyniki obliczen z watkow ---\n");
    for (int i = 0; i < LICZBA_WATKOW; i++) {
        printf("Watek %d obliczyl %d! = %lld\n", dane_watkow[i].id, dane_watkow[i].n, dane_watkow[i].wynik);
    }
    
    return 0;
}