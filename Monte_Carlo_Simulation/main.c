#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

typedef struct {
    int points_per_thread;
    long long inside_circle;
    double elapsed_time;
} thread_data_t;

// Szálfüggvény
void *monte_carlo(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    long long count = 0;
    clock_t start = clock();

    for (int i = 0; i < data->points_per_thread; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;

        if (x * x + y * y <= 1.0) {
            count++;
        }
    }

    clock_t end = clock();
    data->inside_circle = count;
    data->elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int num_threads = 4;

    // Problémaméretek listája
    int problem_sizes[] = {500000, 1000000, 1500000, 2000000, 2500000, 3000000, 3500000, 4000000, 4500000, 5000000};
    int num_sizes = sizeof(problem_sizes) / sizeof(problem_sizes[0]);

    // Fájlok megnyitása
    FILE *fp = fopen("montecarlo_results_with_size.csv", "w");
    if (fp == NULL) {
        fprintf(stderr, "Nem sikerült megnyitni a fajlt irasra.\n");
        return 1;
    }

    // Fejléc
    fprintf(fp, "ProblemSize,Threads,SequentialTime,ParallelTime,Speedup,Efficiency\n");

    for (int p = 0; p < num_sizes; p++) {
        int NUM_POINTS = problem_sizes[p];
        pthread_t threads[num_threads];
        thread_data_t thread_data[num_threads];
        int points_per_thread = NUM_POINTS / num_threads;

        srand(50);
        
        // Szekvenciális futásidő mérése
        clock_t seq_start = clock();
        long long seq_count = 0;
        for (int i = 0; i < NUM_POINTS; i++) {
            double x = (double)rand() / RAND_MAX;
            double y = (double)rand() / RAND_MAX;
            if (x * x + y * y <= 1.0) seq_count++;
        }
        clock_t seq_end = clock();
        double seq_time = (double)(seq_end - seq_start) / CLOCKS_PER_SEC;

        srand(50);
        
        // Párhuzamos futás mérése
        clock_t start_time = clock();
        for (int i = 0; i < num_threads; i++) {
            thread_data[i].points_per_thread = points_per_thread;
            thread_data[i].inside_circle = 0;
            thread_data[i].elapsed_time = 0.0;
            pthread_create(&threads[i], NULL, monte_carlo, (void *)&thread_data[i]);
        }

        long long total_inside_circle = 0;
        for (int i = 0; i < num_threads; i++) {
            pthread_join(threads[i], NULL);
            total_inside_circle += thread_data[i].inside_circle;
        }
        clock_t end_time = clock();
        double parallel_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

        double pi = 4.0 * total_inside_circle / NUM_POINTS;
        double speedup;
        if (parallel_time <= 0.0) {
            speedup = 0.0;  // nincs gyorsítás, ha nincs érvényes párhuzamos idő
            } else {
            speedup = seq_time / parallel_time;
            }
        double efficiency = speedup / num_threads;


        printf("Problem size: %d\n", NUM_POINTS);
        printf("Estimated Pi = %f\n", pi);
        printf("Sequential time = %f s\n", seq_time);
        printf("Parallel time = %f s\n", parallel_time);
        printf("Speedup = %f\n", speedup);
        printf("Efficiency = %f\n", efficiency);
        printf("\n");

        double pi_seq = 4.0 * seq_count / NUM_POINTS;
        printf("Sequential estimated Pi = %f\n", pi_seq);


        // Eredmények fájlba írása
        fprintf(fp, "%d,%d,%f,%f,%f,%f\n", NUM_POINTS, num_threads, seq_time, parallel_time, speedup, efficiency);
    }

    fclose(fp);
    printf("Osszes meres elmentve: montecarlo_results_with_size.csv\n");

    return 0;
}
