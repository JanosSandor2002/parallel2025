#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Számlálja a különböző tulajdonságú elemeket
void count_integers(int *arr, int size) {
    int even_count = 0, odd_count = 0, zero_count = 0, positive_count = 0, negative_count = 0;

    for (int i = 0; i < size; i++) {
        if (arr[i] == 0) {
            zero_count++;
        } else if (arr[i] > 0) {
            positive_count++;
        } else {
            negative_count++;
        }

        if (arr[i] % 2 == 0) {
            even_count++;
        } else {
            odd_count++;
        }
    }

    printf("Páros számok: %d\n", even_count);
    printf("Páratlan számok: %d\n", odd_count);
    printf("Nulla értékek: %d\n", zero_count);
    printf("Pozitív számok: %d\n", positive_count);
    printf("Negatív számok: %d\n", negative_count);
}

int main() {
    int n;
    printf("Adja meg a tömb méretét: ");
    scanf("%d", &n);

    // Dinamikusan allokáljuk a tömböt
    int *arr = (int *)malloc(n * sizeof(int));

    // Véletlenszerű számok generálása
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 201 - 100;  // [-100, 100] intervallum
    }

    // Időmérés indítása
    clock_t start = clock();

    // Szekvenciális számítás
    count_integers(arr, n);

    // Időmérés vége
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Szekvenciális futási idő: %f másodperc\n", time_spent);

    // Memória felszabadítása
    free(arr);

    return 0;
}
