#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Tömb generálása, szigorúan monoton növekvő
void generate_increasing_array(double *arr, size_t size) {
    arr[0] = (double)(rand() % 1000);
    for (size_t i = 1; i < size; i++) {
        arr[i] = arr[i - 1] + (rand() % 100) / 100.0; // Minden elem nagyobb az előzőnél
    }
}

// Szigorúan monoton növekvő sorozat ellenőrzése
bool is_increasing(double *arr, size_t size) {
    for (size_t i = 1; i < size; i++) {
        if (arr[i] <= arr[i - 1]) {
            return false; // Ha nem növekvő, visszatérünk false-szal
        }
    }
    return true;
}

// Bináris keresés iteratív módon
int binary_search_iterative(double *arr, size_t size, double target) {
    int left = 0, right = size - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1; // Ha nem található
}

// Bináris keresés rekurzív módon
int binary_search_recursive(double *arr, int left, int right, double target) {
    if (left > right) return -1;
    int mid = left + (right - left) / 2;
    if (arr[mid] == target) return mid;
    if (arr[mid] < target) return binary_search_recursive(arr, mid + 1, right, target);
    return binary_search_recursive(arr, left, mid - 1, target);
}

// Időmérés függvények
void measure_search_time(int (*search_func)(double*, int, int, double), double *arr, size_t size, double target) {
    clock_t start, end;
    start = clock();
    int result = search_func(arr, 0, size - 1, target);
    end = clock();
    printf("Search result: %d, Time: %.6f seconds\n", result, (double)(end - start) / CLOCKS_PER_SEC);
}

void measure_search_iterative_time(int (*search_func)(double*, size_t, double), double *arr, size_t size, double target) {
    clock_t start, end;
    start = clock();
    int result = search_func(arr, size, target);
    end = clock();
    printf("Search result: %d, Time: %.6f seconds\n", result, (double)(end - start) / CLOCKS_PER_SEC);
}

int main() {
    size_t size = 100000;  // Példa elemszám

    double *arr = (double *)malloc(size * sizeof(double));
    if (arr == NULL) {
        perror("Memory allocation failed");
        return 1;
    }

    // Tömb generálása és ellenőrzés
    generate_increasing_array(arr, size);
    if (is_increasing(arr, size)) {
        printf("Array is strictly increasing.\n");
    } else {
        printf("Array is NOT strictly increasing.\n");
    }

    // Véletlenszerű keresési cél
    double target = arr[rand() % size];

    // Bináris keresés iteratív és rekurzív módon
    printf("\nIterative binary search:\n");
    measure_search_iterative_time(binary_search_iterative, arr, size, target);

    printf("\nRecursive binary search:\n");
    measure_search_time(binary_search_recursive, arr, size, target);

    free(arr);
    return 0;
}
