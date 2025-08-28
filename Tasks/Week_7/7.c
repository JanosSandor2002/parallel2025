#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define N 1000000  // A felhasznált részletek száma (nagy szám a pontos eredmény érdekében)
#define MAX_ITER 100  // A párhuzamos tesztelésnél végrehajtott iterációk száma

// A függvény, amit integrálni akarunk
double func(double x) {
    return exp(-x * x);  // Példa: gauss-eloszlás integrálása
}

// Szekvenciális téglalap módszer
double rectangular_integral_seq(double (*f)(double), double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        double x = a + i * h;
        sum += f(x);
    }
    return sum * h;
}

// Szekvenciális trapéz módszer
double trapezoidal_integral_seq(double (*f)(double), double a, double b, int n) {
    double h = (b - a) / n;
    double sum = (f(a) + f(b)) / 2.0;
    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        sum += f(x);
    }
    return sum * h;
}

// Párhuzamos téglalap módszer
double rectangular_integral_parallel(double (*f)(double), double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; i++) {
        double x = a + i * h;
        sum += f(x);
    }
    return sum * h;
}

// Párhuzamos trapéz módszer
double trapezoidal_integral_parallel(double (*f)(double), double a, double b, int n) {
    double h = (b - a) / n;
    double sum = (f(a) + f(b)) / 2.0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        sum += f(x);
    }
    return sum * h;
}

// Idő mérés függvénye
double measure_time(double (*integral_func)(double (*f)(double), double, double, int), double (*f)(double), double a, double b, int n) {
    double start_time = omp_get_wtime();
    integral_func(f, a, b, n);
    double end_time = omp_get_wtime();
    return end_time - start_time;
}

// Az analitikus megoldás a gauss-eloszlás integráljára
double exact_integral(double a, double b) {
    return 0.5 * (erf(b) - erf(a));  // Az erf a hibafüggvény, amely a gauss-eloszlás integrálja
}

int main() {
    double a = 0.0, b = 3.0;  // Integrálási tartomány
    int n = N;  // A felhasznált részek száma

    // Az analitikus eredmény
    double exact_result = exact_integral(a, b);

    // Futásidő mérés és eredmények kiírása CSV fájlba
    FILE *file = fopen("integration_results.csv", "w");
    if (file == NULL) {
        perror("Hiba a fájl megnyitásakor");
        return 1;
    }
    fprintf(file, "Method,Time,Error\n");

    // Szekvenciális téglalap
    double time_taken = measure_time(rectangular_integral_seq, func, a, b, n);
    double error = fabs(exact_result - rectangular_integral_seq(func, a, b, n));
    fprintf(file, "Rectangular (Seq),%f,%f\n", time_taken, error);

    // Szekvenciális trapéz
    time_taken = measure_time(trapezoidal_integral_seq, func, a, b, n);
    error = fabs(exact_result - trapezoidal_integral_seq(func, a, b, n));
    fprintf(file, "Trapezoidal (Seq),%f,%f\n", time_taken, error);

    // Párhuzamos téglalap
    time_taken = measure_time(rectangular_integral_parallel, func, a, b, n);
    error = fabs(exact_result - rectangular_integral_parallel(func, a, b, n));
    fprintf(file, "Rectangular (Parallel),%f,%f\n", time_taken, error);

    // Párhuzamos trapéz
    time_taken = measure_time(trapezoidal_integral_parallel, func, a, b, n);
    error = fabs(exact_result - trapezoidal_integral_parallel(func, a, b, n));
    fprintf(file, "Trapezoidal (Parallel),%f,%f\n", time_taken, error);

    fclose(file);

    printf("Az integrál analitikus eredménye: %f\n", exact_result);
    printf("Eredmények kiírva a 'integration_results.csv' fájlba.\n");

    return 0;
}
