#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Függvények a számításokhoz

// Elemösszeg
int sum_iterative(int arr[], size_t size) {
    int sum = 0;
    for (size_t i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum;
}

// Minimum és maximum érték iteratívan
void find_min_max_iterative(int arr[], size_t size, int *min, int *max) {
    *min = arr[0];
    *max = arr[0];
    for (size_t i = 1; i < size; i++) {
        if (arr[i] < *min) *min = arr[i];
        if (arr[i] > *max) *max = arr[i];
    }
}

// Rekurzív függvények
int sum_recursive(int arr[], size_t size) {
    if (size == 0) return 0;
    return arr[size - 1] + sum_recursive(arr, size - 1);
}

void find_min_max_recursive(int arr[], size_t size, int *min, int *max) {
    if (size == 0) return;

    if (arr[size - 1] < *min) *min = arr[size - 1];
    if (arr[size - 1] > *max) *max = arr[size - 1];

    find_min_max_recursive(arr, size - 1, min, max);
}

// Időmérés és tesztelés
void measure_time(void (*func)(int[], size_t, int*, int*), int arr[], size_t size) {
    clock_t start, end;
    start = clock();
    int min = arr[0], max = arr[0];
    func(arr, size, &min, &max);
    end = clock();
    printf("Time: %.6f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
}

void measure_sum_time(int (*func)(int[], size_t), int arr[], size_t size) {
    clock_t start, end;
    start = clock();
    int sum = func(arr, size);
    end = clock();
    printf("Sum: %d\n", sum);
    printf("Time: %.6f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
}

int main() {
    size_t size = 100000;  // Példa elemszám

    int *arr = (int *)malloc(size * sizeof(int));
    if (arr == NULL) {
        perror("Memory allocation failed");
        return 1;
    }

    // Véletlenszerű értékek feltöltése
    for (size_t i = 0; i < size; i++) {
        arr[i] = rand() % 1000;
    }

    // Iteratív számítások
    printf("Iterative sum and min-max calculation:\n");
    measure_sum_time(sum_iterative, arr, size);
    int min, max;
    find_min_max_iterative(arr, size, &min, &max);
    printf("Min: %d, Max: %d\n", min, max);

    // Rekurzív számítások
    printf("\nRecursive sum and min-max calculation:\n");
    measure_sum_time(sum_recursive, arr, size);
    find_min_max_recursive(arr, size, &min, &max);
    printf("Min: %d, Max: %d\n", min, max);

    free(arr);
    return 0;
}
