#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int random_integer(int min, int max) {
    return rand() % (max - min + 1) + min;
}

float random_float(float min, float max) {
    float range = max - min;
    return (rand() / (float)RAND_MAX) * range + min;
}

int main() {
    // Véletlenszám generálás beállítása
    srand(time(NULL));

    // Két egész szám közötti véletlenszám
    int int_min = 10, int_max = 50;
    int rand_int = random_integer(int_min, int_max);
    printf("Véletlen egész szám: %d\n", rand_int);

    // Két lebegőpontos szám közötti véletlenszám
    float float_min = 1.5, float_max = 10.5;
    float rand_float = random_float(float_min, float_max);
    printf("Véletlen lebegőpontos szám: %.2f\n", rand_float);

    return 0;
}
