#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>
#include <math.h>
#include <time.h>

#define MAX_THREADS 16
#define N 1000000  // Tömb mérete
#define MAX_DEPTH 10  // Maximális rekurziós mélység

// Véletlenszerű valós szám generálása [0, 1) intervallumban
double random_double() {
    return (double)rand() / RAND_MAX;
}

// Szekvenciális szorzás
double sequential_product(double *arr, long n) {
    double product = 1.0;
    for (long i = 0; i < n; i++) {
        product *= arr[i];
    }
    return product;
}

// Rekurzív szorzás fix mélységgel
double recursive_product(double *arr, long start, long end, int depth) {
    if (end - start == 1 || depth == 0) {
        return arr[start];
    }
    long mid = (start + end) / 2;
    double left = recursive_product(arr, start, mid, depth - 1);
    double right = recursive_product(arr, mid, end, depth - 1);
    return left * right;
}

// Párhuzamos szorzás POSIX szálakkal
typedef struct {
    double *arr;
    long start;
    long end;
    double result;
} ThreadData;

void* thread_product(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    double product = 1.0;
    for (long i = data->start; i < data->end; i++) {
        product *= data->arr[i];
    }
    data->result = product;
    return NULL;
}

double parallel_product_posix(double *arr, long n, int num_threads) {
    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];
    long chunk_size = n / num_threads;
    double product = 1.0;

    for (int i = 0; i < num_threads; i++) {
        thread_data[i].arr = arr;
        thread_data[i].start = i * chunk_size;
        thread_data[i].end = (i == num_threads - 1) ? n : (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, thread_product, (void *)&thread_data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        product *= thread_data[i].result;
    }

    return product;
}

// Párhuzamos szorzás OpenMP for ciklussal
double parallel_product_omp_for(double *arr, long n) {
    double product = 1.0;
    #pragma omp parallel for reduction(*:product)
    for (long i = 0; i < n; i++) {
        product *= arr[i];
    }
    return product;
}

// Párhuzamos szorzás OpenMP redukcióval
double parallel_product_omp_reduction(double *arr, long n) {
    double product = 1.0;
    #pragma omp parallel for reduction(*:product)
    for (long i = 0; i < n; i++) {
        product *= arr[i];
    }
    return product;
}

// Futási idő mérésére szolgáló függvény
double measure_time(double (*func)(double*, long), double *arr, long n) {
    clock_t start = clock();
    double result = func(arr, n);
    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    return time_taken;
}

// CSV fájlba írás
void write_to_csv(const char *filename, double sequential_time, double parallel_time_posix, double parallel_time_omp) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Unable to open file");
        return;
    }
    fprintf(file, "Method,Time (s)\n");
    fprintf(file, "Sequential,%f\n", sequential_time);
    fprintf(file, "Parallel (POSIX),%f\n", parallel_time_posix);
    fprintf(file, "Parallel (OpenMP for),%f\n", parallel_time_omp);
    fclose(file);
}

int main() {
    srand(time(NULL));

    double *arr = (double *)malloc(N * sizeof(double));
    for (long i = 0; i < N; i++) {
        arr[i] = random_double();
    }

    // Szekvenciális szorzás
    double seq_time = measure_time(sequential_product, arr, N);
    printf("Sequential time: %.6f seconds\n", seq_time);

    // Párhuzamos szorzás POSIX szálakkal
    int num_threads = 4;  // Példa szálak száma
    double par_posix_time = measure_time((double (*)(double*, long))parallel_product_posix, arr, N);
    printf("Parallel POSIX time: %.6f seconds\n", par_posix_time);

    // Párhuzamos szorzás OpenMP for ciklussal
    double par_omp_for_time = measure_time((double (*)(double*, long))parallel_product_omp_for, arr, N);
    printf("Parallel OpenMP for time: %.6f seconds\n", par_omp_for_time);

    // CSV fájlba írás
    write_to_csv("timing_results.csv", seq_time, par_posix_time, par_omp_for_time);

    // Memória felszabadítása
    free(arr);
    return 0;
}
