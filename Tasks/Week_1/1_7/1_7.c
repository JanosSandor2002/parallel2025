#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Függvény, amely meghatározza, hogy egy szám prímszám-e
int is_prime(int num) {
    if (num <= 1) return 0; // 1 és a negatív számok nem prímszámok
    for (int i = 2; i <= sqrt(num); i++) {
        if (num % i == 0) return 0; // Ha osztható, nem prímszám
    }
    return 1; // Ha nem találtunk osztót, akkor prímszám
}

// Függvény, amely meghatározza a prímszámok számát az intervallumban
int count_primes(int start, int end) {
    int count = 0;
    for (int i = start; i <= end; i++) {
        if (is_prime(i)) {
            count++;
        }
    }
    return count;
}

int main() {
    // Paraméterek
    int start = 1, end = 10000; // Kezdeti és végpontok az intervallumban
    int step = 1000; // Különbség az intervallumok között

    // Idő méréséhez változók
    time_t start_time, end_time;
    double time_taken;

    // Kiírás a fejléchez
    printf("Intervallum kezdo\tIntervallum veg\tPrimes szama\tFutasi ido (masodperc)\n");

    // Végezzen el méréseket a különböző intervallumoknál
    for (int i = start; i <= end; i += step) {
        int current_end = i + step - 1; // A végpont

        if (current_end > end) {
            current_end = end; // Ha a végpont túlmegy a megadott end-en, állítsuk be a végpontot
        }

        // Időmérés kezdete
        time(&start_time);

        // Prímszámok számolása az intervallumban
        int prime_count = count_primes(i, current_end);

        // Időmérés vége
        time(&end_time);
        time_taken = difftime(end_time, start_time);

        // Kiírás az eredményről
        printf("%d\t\t\t%d\t\t\t%d\t\t%.6f\n", i, current_end, prime_count, time_taken);
    }

    return 0;
}
