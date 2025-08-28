#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generate_random_numbers(const char *filename, size_t element_count) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    srand(time(NULL)); // Random seed based on current time

    // Write random integers to file
    for (size_t i = 0; i < element_count; i++) {
        int num = rand();  // Random integer
        fwrite(&num, sizeof(int), 1, file);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <filename> <element_count>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    size_t element_count = atoi(argv[2]);

    // Measure time to generate random numbers
    clock_t start_gen = clock();
    generate_random_numbers(filename, element_count);
    clock_t end_gen = clock();
    double gen_time = (double)(end_gen - start_gen) / CLOCKS_PER_SEC;

    // Measure time to save the file
    clock_t start_save = clock();
    generate_random_numbers(filename, element_count); // Saving again for accurate measurement
    clock_t end_save = clock();
    double save_time = (double)(end_save - start_save) / CLOCKS_PER_SEC;

    printf("Random number generation time for %zu elements: %.6f seconds\n", element_count, gen_time);
    printf("File save time for %zu elements: %.6f seconds\n", element_count, save_time);

    return 0;
}
