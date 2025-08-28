#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_FIB_INDEX  50
#define MAX_PROB_SIZE 1000000
#define N_VARIANTS      2

long long fib_cache[MAX_FIB_INDEX+1];

// Naiv Fibonacci
long long fib_iter(int n) {
    if (n < 2) return n;
    long long a = 0, b = 1;
    for (int i = 2; i <= n; i++) {
        long long tmp = a + b;
        a = b;
        b = tmp;
    }
    return b;
}

// Cache-es Fibonacci
long long fib_memo(int n) {
    if (n < 2) return n;
    long long val;
    #pragma omp critical
    { val = fib_cache[n]; }
    if (val != -1) return val;
    long long v1 = fib_memo(n-1);
    long long v2 = fib_memo(n-2);
    long long res = v1 + v2;
    #pragma omp critical
    { fib_cache[n] = res; }
    return res;
}

// Random tomb feltoltese
void fill_random(int *data, int N) {
    for (int i = 0; i < N; i++)
        data[i] = rand() % MAX_FIB_INDEX + 1;
}
// main - tomb feltoltes - szalkezeles
int main() {
    srand(42);
    int sizes[]   = {700000, 750000, 800000, 850000, 900000, 950000, 1000000};
    int  n_sizes  = sizeof(sizes)/sizeof(sizes[0]);
    int threads[] = {1,2,4,8,16};
    int  n_thr    = sizeof(threads)/sizeof(threads[0]);

    static int data[MAX_PROB_SIZE];
    static long long result[MAX_PROB_SIZE];

    FILE *fp = fopen("fibonacci_mapping_results.csv", "w");
    if (!fp) { perror("fopen"); return 1; }

    fprintf(fp, "Variant,Size,Threads,SequentialTime,ParallelTime,Speedup,Efficiency\n");

    for (int var = 0; var < N_VARIANTS; var++) {
        const char *name = (var==0 ? "naive" : "cache");
        printf("---- Varians: %s ----\n", name);

        int FIX_T = 4;
        for (int si = 0; si < n_sizes; si++) {
            int N = sizes[si];
            printf("Meret: %d, Szalak (fix): %d\n", N, FIX_T);

            if (var == 1) {
                for (int i = 0; i <= MAX_FIB_INDEX; i++) fib_cache[i] = -1;
                printf("Cache inicializalva -1-re\n");
            }

            fill_random(data, N);
            printf("Random tomb feltoltve a sorrendi futashoz\n");
            double t0 = omp_get_wtime();
            for (int i = 0; i < N; i++) {
                if (var==0)
                    result[i] = fib_iter(data[i]);
                else
                    result[i] = fib_memo(data[i]);
            }
            double seq_time = omp_get_wtime() - t0;
            if (seq_time <= 0.0) seq_time = 1e-6;
            printf("Sorrendi futas ido: %.6f mp\n", seq_time);

            if (var == 1) {
                for (int i = 0; i <= MAX_FIB_INDEX; i++) fib_cache[i] = -1;
                printf("Cache ujrainicializalva a parhuzamos futas elott\n");
            }

            fill_random(data, N);
            printf("Random tomb feltoltve a parhuzamos futashoz\n");
            double p0 = omp_get_wtime();
            #pragma omp parallel num_threads(FIX_T)
            {
                #pragma omp for schedule(static)
                for (int i = 0; i < N; i++) {
                    if (var==0)
                        result[i] = fib_iter(data[i]);
                    else
                        result[i] = fib_memo(data[i]);
                }
            }
            double par_time = omp_get_wtime() - p0;
            if (par_time <= 0.0) par_time = 1e-6;
            printf("Parhuzamos futas ido (%d szal): %.6f mp\n", FIX_T, par_time);

            double speedup    = seq_time / par_time;
            double efficiency = speedup / FIX_T;
            printf("Speedup: %.6f, Hatasfok: %.6f\n", speedup, efficiency);

            fprintf(fp, "%s,%d,%d,%.6f,%.6f,%.6f,%.6f\n",
                    name, N, FIX_T, seq_time, par_time, speedup, efficiency);
        }
    }

    fclose(fp);
    printf("Meresek elmentve: fibonacci_mapping_results.csv\n");
    return 0;
}
