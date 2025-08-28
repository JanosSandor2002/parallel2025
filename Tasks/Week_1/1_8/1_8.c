#include <stdio.h>
#include <stdlib.h>

void write_int_to_file(const char *filename, int *arr, size_t size) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    fwrite(arr, sizeof(int), size, file);
    fclose(file);
}

void write_long_to_file(const char *filename, long *arr, size_t size) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    fwrite(arr, sizeof(long), size, file);
    fclose(file);
}

void write_float_to_file(const char *filename, float *arr, size_t size) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    fwrite(arr, sizeof(float), size, file);
    fclose(file);
}

long get_file_size(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);
    return size;
}

void read_int_from_file(const char *filename, int *arr, size_t size) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    fread(arr, sizeof(int), size, file);
    fclose(file);
}

void read_long_from_file(const char *filename, long *arr, size_t size) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    fread(arr, sizeof(long), size, file);
    fclose(file);
}

void read_float_from_file(const char *filename, float *arr, size_t size) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    fread(arr, sizeof(float), size, file);
    fclose(file);
}

int main() {
    // Example for int
    int int_arr[] = {1, 2, 3, 4, 5};
    size_t int_size = sizeof(int_arr) / sizeof(int_arr[0]);
    const char *int_file = "int_data.bin";
    write_int_to_file(int_file, int_arr, int_size);
    printf("File size (int): %ld bytes\n", get_file_size(int_file));

    // Example for long
    long long_arr[] = {100000L, 200000L, 300000L};
    size_t long_size = sizeof(long_arr) / sizeof(long_arr[0]);
    const char *long_file = "long_data.bin";
    write_long_to_file(long_file, long_arr, long_size);
    printf("File size (long): %ld bytes\n", get_file_size(long_file));

    // Example for float
    float float_arr[] = {1.5f, 2.5f, 3.5f};
    size_t float_size = sizeof(float_arr) / sizeof(float_arr[0]);
    const char *float_file = "float_data.bin";
    write_float_to_file(float_file, float_arr, float_size);
    printf("File size (float): %ld bytes\n", get_file_size(float_file));

    return 0;
}
