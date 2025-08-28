#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Függvény, amely generálja a tömböt, és 0.5 valószínűséggel duplikál elemeket
void generate_array(int *arr, size_t size) {
    for (size_t i = 0; i < size; i++) {
        arr[i] = rand() % 1000; // Véletlenszerű szám generálása 0-999 között
        if (rand() % 2 == 0) {
            // 50% valószínűséggel duplikálunk egy számot
            size_t duplicate_index = rand() % i; // Véletlenszerűen egy előző számot választunk
            arr[i] = arr[duplicate_index];
        }
    }
}

// Függvény, amely ellenőrzi, hogy a tömb minden eleme egyedi-e
bool is_unique(int *arr, size_t size) {
    for (size_t i = 0; i < size; i++) {
        for (size_t j = i + 1; j < size; j++) {
            if (arr[i] == arr[j]) {
                return false; // Ha találunk duplikált elemet, visszatérünk false-szal
            }
        }
    }
    return true;
}

int main() {
    size_t size = 100000;  // Példa elemszám
    int *arr = (int *)malloc(size * sizeof(int));
    if (arr == NULL) {
        perror("Memory allocation failed");
        return 1;
    }

    // Mérjük a tömb generálásának idejét
    clock_t start_gen = clock();
    generate_array(arr, size);
    clock_t end_gen = clock();
    double gen_time = (double)(end_gen - start_gen) / CLOCKS_PER_SEC;

    // Mérjük az egyediség ellenőrzésének idejét
    clock_t start_check = clock();
    bool unique = is_unique(arr, size);
    clock_t end_check = clock();
    double check_time = (double)(end_check - start_check) / CLOCKS_PER_SEC;

    // Kiírjuk az eredményeket
    printf("Array generation time: %.6f seconds\n", gen_time);
    printf("Uniqueness check time: %.6f seconds\n", check_time);
    printf("Array is unique: %s\n", unique ? "Yes" : "No");

    free(arr);
    return 0;
}
