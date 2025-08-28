#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_THREADS 16

// Strukturálisan definiált száladatok
typedef struct {
    int *arr;         // A tömb, amelynek elemeit össze kell adni
    long start_index; // Kezdő index
    long end_index;   // Befejező index
    long result;      // Az összeg (eredmény)
} ThreadData;

// A szálak által használt összegző függvény
void* sum_array_part(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    long sum = 0;
    for (long i = data->start_index; i < data->end_index; i++) {
        sum += data->arr[i];
    }
    data->result = sum;
    return NULL;
}

// Szekvenciális összegző függvény (módosítva)
long sequential_sum(int *arr, long n, int num_threads) {
    long sum = 0;
    for (long i = 0; i < n; i++) {
        sum += arr[i];
    }
    return sum;
}

// Párhuzamos összegzés több szál használatával
long parallel_sum(int *arr, long n, int num_threads) {
    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];
    long chunk_size = n / num_threads;
    long sum = 0;

    // Szálak létrehozása és az adataink elküldése
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].arr = arr;
        thread_data[i].start_index = i * chunk_size;
        thread_data[i].end_index = (i == num_threads - 1) ? n : (i + 1) * chunk_size;
        thread_data[i].result = 0;
        pthread_create(&threads[i], NULL, sum_array_part, (void *)&thread_data[i]);
    }

    // Várakozás a szálak befejezésére
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        sum += thread_data[i].result;  // Az összeg hozzáadása a végső összeghez
    }

    return sum;
}

// Rekurzív megoldás a tömb összegzésére (felosztás)
long recursive_sum(int *arr, long start, long end) {
    if (end - start <= 1) {  // Alap eset: ha csak egy elem van
        return arr[start];
    } else {
        long mid = (start + end) / 2;
        long left_sum = recursive_sum(arr, start, mid);
        long right_sum = recursive_sum(arr, mid, end);
        return left_sum + right_sum;
    }
}

// Futási idő mérésére szolgáló függvény
double measure_time(long (*func)(int*, long, int), int *arr, long n, int num_threads) {
    clock_t start, end;
    start = clock();
    long result = func(arr, n, num_threads);  // A függvény most már elfogadja a num_threads argumentumot
    end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Result: %ld\n", result);
    return time_taken;
}

int main() {
    long n = 1000000;  // A tömb mérete
    int num_threads = 4;  // A szálak száma

    // Tömb létrehozása és inicializálása
    int *arr = (int *)malloc(n * sizeof(int));
    for (long i = 0; i < n; i++) {
        arr[i] = 1;  // Minden elem 1 (így egyszerűsítettük az összegzést)
    }

    // Szekvenciális összegzés
    printf("Sequential sum:\n");
    double seq_time = measure_time(sequential_sum, arr, n, 1); // 1 szálat használunk
    printf("Sequential time: %.6f seconds\n", seq_time);

    // Párhuzamos összegzés (több szál használatával)
    printf("\nParallel sum with %d threads:\n", num_threads);
    double par_time = measure_time(parallel_sum, arr, n, num_threads);
    printf("Parallel time: %.6f seconds\n", par_time);

    // Rekurzív összegzés
    clock_t start, end;  // Declare start and end for clock
    printf("\nRecursive sum:\n");
    start = clock();
    long rec_result = recursive_sum(arr, 0, n);
    end = clock();
    double rec_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Recursive result: %ld\n", rec_result);
    printf("Recursive time: %.6f seconds\n", rec_time);

    // Memória felszabadítása
    free(arr);

    return 0;
}
