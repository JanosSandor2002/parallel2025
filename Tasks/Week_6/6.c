#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define N 100000  // Minta mérete
#define MAX_VALUE 1000  // Az elemek maximum értéke

// Párhuzamos COPY művelet
void parallel_copy(int arr[], int start, int end, int temp_arr[], int temp_start) {
    #pragma omp parallel for
    for (int i = start; i <= end; i++) {
        temp_arr[temp_start + i - start] = arr[i];
    }
}

// Párhuzamos merge
void parallel_merge(int arr[], int start, int mid, int end) {
    int n1 = mid - start + 1;
    int n2 = end - mid;
    int *left = malloc(n1 * sizeof(int));
    int *right = malloc(n2 * sizeof(int));

    parallel_copy(arr, start, mid, left, 0);
    parallel_copy(arr, mid + 1, end, right, 0);

    int i = 0, j = 0, k = start;
    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        } else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = left[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = right[j];
        j++;
        k++;
    }

    free(left);
    free(right);
}

// Párhuzamos merge sort
void merge_sort(int arr[], int start, int end) {
    if (start < end) {
        int mid = (start + end) / 2;
        merge_sort(arr, start, mid);
        merge_sort(arr, mid + 1, end);
        parallel_merge(arr, start, mid, end);
    }
}

// Véletlenszerű tömb generálása
void generate_random_array(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % MAX_VALUE;
    }
}

// Szekvenciális merge (párhuzamos verzió nélkül)
void sequential_merge(int arr[], int start, int mid, int end) {
    int n1 = mid - start + 1;
    int n2 = end - mid;
    int *left = malloc(n1 * sizeof(int));
    int *right = malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++) left[i] = arr[start + i];
    for (int j = 0; j < n2; j++) right[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = start;
    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        } else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = left[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = right[j];
        j++;
        k++;
    }

    free(left);
    free(right);
}

// Szekvenciális merge sort
void sequential_merge_sort(int arr[], int start, int end) {
    if (start < end) {
        int mid = (start + end) / 2;
        sequential_merge_sort(arr, start, mid);
        sequential_merge_sort(arr, mid + 1, end);
        sequential_merge(arr, start, mid, end);
    }
}

// Minimumkiválasztásos rendezés (javítva)
void selection_sort(int arr[], int start, int end) {
    for (int i = start; i < end; i++) {
        int min_idx = i;
        for (int j = i + 1; j < end; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        int temp = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = temp;
    }
}

// Idő mérés
double measure_time(void (*sort_func)(int[], int, int), int arr[], int n) {
    double start_time = omp_get_wtime();
    sort_func(arr, 0, n);
    double end_time = omp_get_wtime();
    return end_time - start_time;
}

// CSV fájlba írás
void write_to_csv(const char *filename, const char *alg_name, double time_taken) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Hiba a fájl megnyitásakor");
        return;
    }
    fprintf(file, "%s,%f\n", alg_name, time_taken);
    fclose(file);
}

int main() {
    int arr[N];
    srand(time(NULL));  // Véletlenszerű számok generálása

    // CSV fájl készítése
    FILE *file = fopen("sort_times.csv", "w");
    if (file == NULL) {
        perror("Hiba a fájl megnyitásakor");
        return 1;
    }
    fprintf(file, "Algoritmus,Növekedési idő\n");
    fclose(file);

    // Szekvenciális merge sort
    generate_random_array(arr, N);
    double time_taken = measure_time(sequential_merge_sort, arr, N);
    printf("Szekvenciális merge sort futási ideje: %f másodperc\n", time_taken);
    write_to_csv("sort_times.csv", "Szekvenciális merge sort", time_taken);

    // Párhuzamos merge sort
    generate_random_array(arr, N);
    time_taken = measure_time(merge_sort, arr, N);
    printf("Párhuzamos merge sort futási ideje: %f másodperc\n", time_taken);
    write_to_csv("sort_times.csv", "Párhuzamos merge sort", time_taken);

    // Minimumkiválasztásos rendezés
    generate_random_array(arr, N);
    time_taken = measure_time(selection_sort, arr, N);
    printf("Minimumkiválasztásos rendezés futási ideje: %f másodperc\n", time_taken);
    write_to_csv("sort_times.csv", "Minimumkiválasztásos rendezés", time_taken);

    return 0;
}
