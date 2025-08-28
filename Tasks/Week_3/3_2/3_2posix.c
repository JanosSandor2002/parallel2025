#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 4

typedef struct {
    int *arr;
    int start;
    int end;
    int even_count;
    int odd_count;
    int zero_count;
    int positive_count;
    int negative_count;
} ThreadData;

void* count_integers_thread(void *arg) {
    ThreadData *data = (ThreadData*)arg;
    data->even_count = 0;
    data->odd_count = 0;
    data->zero_count = 0;
    data->positive_count = 0;
    data->negative_count = 0;

    for (int i = data->start; i < data->end; i++) {
        if (data->arr[i] == 0) {
            data->zero_count++;
        } else if (data->arr[i] > 0) {
            data->positive_count++;
        } else {
            data->negative_count++;
        }

        if (data->arr[i] % 2 == 0) {
            data->even_count++;
        } else {
            data->odd_count++;
        }
    }

    return NULL;
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

    // Több szál létrehozása
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    int part_size = n / NUM_THREADS;

    // Időmérés indítása
    clock_t start = clock();

    // Szálak indítása
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].arr = arr;
        thread_data[i].start = i * part_size;
        thread_data[i].end = (i == NUM_THREADS - 1) ? n : (i + 1) * part_size;
        pthread_create(&threads[i], NULL, count_integers_thread, &thread_data[i]);
    }

    // Szálak összegyűjtése
    int total_even = 0, total_odd = 0, total_zero = 0, total_positive = 0, total_negative = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        total_even += thread_data[i].even_count;
        total_odd += thread_data[i].odd_count;
        total_zero += thread_data[i].zero_count;
        total_positive += thread_data[i].positive_count;
        total_negative += thread_data[i].negative_count;
    }

    // Időmérés vége
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Páros számok: %d\n", total_even);
    printf("Páratlan számok: %d\n", total_odd);
    printf("Nulla értékek: %d\n", total_zero);
    printf("Pozitív számok: %d\n", total_positive);
    printf("Negatív számok: %d\n", total_negative);

    printf("Többszálú futási idő: %f másodperc\n", time_spent);

    // Memória felszabadítása
    free(arr);

    return 0;
}
