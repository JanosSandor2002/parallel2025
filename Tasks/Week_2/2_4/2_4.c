#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

// Véletlenszerű karakter generálása a megadott karakterek közül (kis és nagy betűk, számjegyek, szóközök és sortörések)
char generate_random_char() {
    int rand_choice = rand() % 5;
    
    if (rand_choice == 0) {
        return 'a' + rand() % 26;  // Kisbetűk
    } else if (rand_choice == 1) {
        return 'A' + rand() % 26;  // Nagybetűk
    } else if (rand_choice == 2) {
        return '0' + rand() % 10;  // Számjegyek
    } else if (rand_choice == 3) {
        return ' ';  // Szóköz
    } else {
        return '\n'; // Sortörés
    }
}

// Fájl generálása véletlenszerű karakterekkel
void generate_random_file(const char *filename, long num_chars) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Fájl megnyitása hiba");
        return;
    }

    for (long i = 0; i < num_chars; i++) {
        fputc(generate_random_char(), file);
    }

    fclose(file);
}

// Iteratív megoldás a sorok számának meghatározására
int count_lines_iterative(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Fájl megnyitása hiba");
        return -1;
    }

    int line_count = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            line_count++;
        }
    }

    fclose(file);
    return line_count;
}

// Rekurzív megoldás a sorok számának meghatározására
int count_lines_recursive_helper(FILE *file, int line_count) {
    char ch = fgetc(file);
    if (ch == EOF) {
        return line_count;
    }
    if (ch == '\n') {
        line_count++;
    }
    return count_lines_recursive_helper(file, line_count);
}

int count_lines_recursive(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Fájl megnyitása hiba");
        return -1;
    }

    int line_count = count_lines_recursive_helper(file, 0);

    fclose(file);
    return line_count;
}

// Üres sorok számának meghatározása
int count_empty_lines(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Fájl megnyitása hiba");
        return -1;
    }

    int empty_lines = 0;
    char ch;
    int is_empty = 1;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            if (is_empty) {
                empty_lines++;
            }
            is_empty = 1;
        } else {
            is_empty = 0;
        }
    }

    fclose(file);
    return empty_lines;
}

int main() {
    // Véletlenszerű szám generálása
    srand(time(NULL));

    const char *filename = "random_text.txt";
    long num_chars = 10000;  // Generálásra kerülő karakterek száma

    // Fájl generálása véletlenszerű karakterekkel
    generate_random_file(filename, num_chars);

    // Iteratív sorok száma
    clock_t start = clock();
    int line_count_iter = count_lines_iterative(filename);
    clock_t end = clock();
    double iter_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Rekurzív sorok száma
    start = clock();
    int line_count_rec = count_lines_recursive(filename);
    end = clock();
    double rec_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Üres sorok száma
    int empty_lines = count_empty_lines(filename);

    // Eredmények
    printf("Iteratív sorok száma: %d, idő: %.6f másodperc\n", line_count_iter, iter_time);
    printf("Rekurzív sorok száma: %d, idő: %.6f másodperc\n", line_count_rec, rec_time);
    printf("Üres sorok száma: %d\n", empty_lines);

    return 0;
}
