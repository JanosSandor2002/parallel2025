#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

// Szekvenciális prefixösszeg kiszámítása
void sequential_prefix_sum(int *arr, int *prefix_sum, int n) {
    prefix_sum[0] = arr[0];
    for (int i = 1; i < n; i++) {
        prefix_sum[i] = prefix_sum[i - 1] + arr[i];
    }
}

// CREW_PREFIX algoritmus
void crew_prefix_sum(int *arr, int *prefix_sum, int n, int num_threads) {
    int step = 1;

    #pragma omp parallel num_threads(num_threads)
    {
        // "Felépítés" fázis
        while (step < n) {
            #pragma omp for
            for (int i = step; i < n; i++) {
                arr[i] = arr[i] + arr[i - step];
            }
            step *= 2;
            #pragma omp barrier
        }

        // "Összesítés" fázis
        #pragma omp for
        for (int i = 0; i < n; i++) {
            prefix_sum[i] = arr[i];
        }
    }
}

// EREW_PREFIX algoritmus
void erew_prefix_sum(int *arr, int *prefix_sum, int n, int num_threads) {
    int step = 1;

    #pragma omp parallel num_threads(num_threads)
    {
        while (step < n) {
            #pragma omp for
            for (int i = n - 1; i >= step; i--) {
                arr[i] = arr[i] + arr[i - step];
            }
            step *= 2;
            #pragma omp barrier
        }

        #pragma omp for
        for (int i = 0; i < n; i++) {
            prefix_sum[i] = arr[i];
        }
    }
}

int main() {
    int input_sizes[] = {100000, 500000, 1000000};  // Bemenet méretek
    int num_threads_list[] = {1, 2, 4, 8};  // Szálak száma
    int num_tests = 3;  // Bemenet méretek száma
    int num_thread_tests = 4;  // Szálak száma teszteléshez

    // CSV fájl megnyitása írásra
    FILE *file = fopen("prefix_results.csv", "w");
    if (file == NULL) {
        printf("Hiba a fájl megnyitásakor!\n");
        return 1;
    }

    // Fejléc írása a CSV fájlba
    fprintf(file, "Bemenet mérete,Szálak,Szekvenciális idő,CREW idő,EREW idő\n");

    for (int test = 0; test < num_tests; test++) {
        int n = input_sizes[test];
        int *arr = (int *)malloc(n * sizeof(int));
        int *prefix_sum = (int *)malloc(n * sizeof(int));
        int *backup_arr = (int *)malloc(n * sizeof(int));  // Biztonsági másolat az eredeti adathoz

        // Véletlenszerű adatok generálása
        srand(time(0));
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % 100;
            backup_arr[i] = arr[i];
        }

        // Szekvenciális prefixösszeg idő mérése
        clock_t start = clock();
        sequential_prefix_sum(arr, prefix_sum, n);
        clock_t end = clock();
        double seq_time = (double)(end - start) / CLOCKS_PER_SEC;

        // Szálak szerinti tesztelés
        for (int t = 0; t < num_thread_tests; t++) {
            int num_threads = num_threads_list[t];

            // CREW_PREFIX idő mérése
            // Adatok visszaállítása eredeti állapotba
            for (int i = 0; i < n; i++) arr[i] = backup_arr[i];

            double crew_start = omp_get_wtime();
            crew_prefix_sum(arr, prefix_sum, n, num_threads);
            double crew_end = omp_get_wtime();
            double crew_time = crew_end - crew_start;

            // EREW_PREFIX idő mérése
            for (int i = 0; i < n; i++) arr[i] = backup_arr[i];

            double erew_start = omp_get_wtime();
            erew_prefix_sum(arr, prefix_sum, n, num_threads);
            double erew_end = omp_get_wtime();
            double erew_time = erew_end - erew_start;

            // Eredmények írása a CSV fájlba
            fprintf(file, "%d,%d,%f,%f,%f\n", n, num_threads, seq_time, crew_time, erew_time);
        }

        free(arr);
        free(prefix_sum);
        free(backup_arr);
    }

    fclose(file);
    printf("Eredmények elmentve a 'prefix_results.csv' fájlba!\n");

    return 0;
}
