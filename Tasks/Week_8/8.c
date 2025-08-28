#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <float.h>
#include <time.h>

#define NUM_POINTS 1000000  // A sztochasztikus integrálás során használt pontok száma

// Funkció a numerikus integráláshoz (példa: gauss-eloszlás)
double func(double x) {
    return exp(-x * x);  // gauss-eloszlás
}

// Sztochasztikus integrálás (Monte Carlo)
double monte_carlo_integral(double (*f)(double), double a, double b, int n) {
    double sum = 0.0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; i++) {
        double x = a + (b - a) * ((double) rand() / RAND_MAX);  // Véletlen pontok generálása
        sum += f(x);
    }
    return (b - a) * sum / n;
}

// Átlag számítása (de problémás adat típusú példa)
void calculate_average() {
    float a = FLT_MAX;
    float b = FLT_MAX;
    float sum = a + b;
    printf("Átlag számítás: %f\n", sum);  // Várható végeredmény: INF (végtelen)
}

// Medián számítása (szekvenciális)
void quicksort(int arr[], int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = (low - 1);
        for (int j = low; j < high; j++) {
            if (arr[j] <= pivot) {
                i++;
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        int temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;
        int pi = i + 1;

        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

int find_median(int arr[], int n) {
    quicksort(arr, 0, n - 1);
    if (n % 2 == 0) {
        return (arr[n / 2] + arr[n / 2 - 1]) / 2;  // Páratlan számú elemből mediánt számolunk
    } else {
        return arr[n / 2];
    }
}

// Hisztogram számítása
void compute_histogram(int arr[], int size, int min, int max) {
    int range = max - min + 1;
    int hist[range];
    
    // Inicializáljuk a hisztogramot
    for (int i = 0; i < range; i++) {
        hist[i] = 0;
    }

    // Hisztogram számítása
    for (int i = 0; i < size; i++) {
        if (arr[i] >= min && arr[i] <= max) {
            hist[arr[i] - min]++;
        }
    }

    // Hisztogram kiírása
    for (int i = 0; i < range; i++) {
        printf("Érték: %d, Gyakoriság: %d\n", i + min, hist[i]);
    }
}

// Mérés funkció (időmérés)
double measure_time(double (*integral_func)(double (*f)(double), double, double, int), 
                    double (*f)(double), double a, double b, int n) {
    clock_t start = clock();
    double result = integral_func(f, a, b, n);
    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    return time_taken;
}

// Az eredményeket CSV fájlba mentjük
void write_csv(double time_taken, double result, const char* method) {
    FILE* file = fopen("results.csv", "a");
    if (file == NULL) {
        printf("Hiba a fájl megnyitásakor!\n");
        return;
    }
    fprintf(file, "%s,%.6f,%.6f\n", method, time_taken, result);
    fclose(file);
}

int main() {
    // Átlag számítása
    printf("Átlag számítása:\n");
    calculate_average();
    
    // Medián számítása
    int arr[] = {1, 5, 3, 2, 4};
    int n = sizeof(arr) / sizeof(arr[0]);
    printf("\nMedián számítása:\n");
    printf("Medián: %d\n", find_median(arr, n));

    // Hisztogram számítása
    int data[] = {1, 5, 3, 5, 2, 3, 3, 1};
    int data_size = sizeof(data) / sizeof(data[0]);
    printf("\nHisztogram számítása:\n");
    compute_histogram(data, data_size, 1, 5);

    // Sztochasztikus integrálás
    double a = 0.0, b = 3.0;
    printf("\nSztochasztikus integrálás:\n");
    double result_mc = monte_carlo_integral(func, a, b, NUM_POINTS);
    double time_mc = measure_time(monte_carlo_integral, func, a, b, NUM_POINTS);
    printf("Sztochasztikus integrál eredmény: %f\n", result_mc);
    printf("Idő: %.6f másodperc\n", time_mc);
    write_csv(time_mc, result_mc, "Monte Carlo");

    // CSV fájl eredmények kiírása (sztochasztikus integrálás példájával)
    printf("\nEredmények CSV fájlba mentve.\n");

    return 0;
}
