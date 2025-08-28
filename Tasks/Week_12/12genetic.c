#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define POP_SIZE 10
#define GENOME_SIZE 5
#define MAX_GENERATIONS 100
#define MUTATION_RATE 0.01

// Célfüggvény (pl. fitness funkció, amit maximalizálni akarunk)
int fitness(int genome[]) {
    int score = 0;
    for (int i = 0; i < GENOME_SIZE; i++) {
        score += genome[i];
    }
    return score;
}

// Véletlenszerű gén generálása
void generate_genome(int genome[]) {
    for (int i = 0; i < GENOME_SIZE; i++) {
        genome[i] = rand() % 2; // Bináris értékek (0 vagy 1)
    }
}

// Kiválasztás: A legjobb két gén kiválasztása a populációból
void selection(int population[POP_SIZE][GENOME_SIZE], int fitness_values[], int *parent1, int *parent2) {
    int best1 = -1, best2 = -1;
    int best_fitness1 = -1, best_fitness2 = -1;

    for (int i = 0; i < POP_SIZE; i++) {
        if (fitness_values[i] > best_fitness1) {
            best2 = best1;
            best_fitness2 = best_fitness1;
            best1 = i;
            best_fitness1 = fitness_values[i];
        } else if (fitness_values[i] > best_fitness2) {
            best2 = i;
            best_fitness2 = fitness_values[i];
        }
    }
    *parent1 = best1;
    *parent2 = best2;
}

// Keresztezés: Két szülő kombinálása
void crossover(int parent1[], int parent2[], int child[]) {
    int crossover_point = rand() % GENOME_SIZE;
    for (int i = 0; i < GENOME_SIZE; i++) {
        if (i < crossover_point) {
            child[i] = parent1[i];
        } else {
            child[i] = parent2[i];
        }
    }
}

// Mutáció: Véletlenszerű változtatás egy génen
void mutate(int genome[]) {
    for (int i = 0; i < GENOME_SIZE; i++) {
        if ((rand() % 100) / 100.0 < MUTATION_RATE) {
            genome[i] = !genome[i];
        }
    }
}

// Genetikus algoritmus
void genetic_algorithm() {
    int population[POP_SIZE][GENOME_SIZE];
    int fitness_values[POP_SIZE];

    // Populáció inicializálása
    for (int i = 0; i < POP_SIZE; i++) {
        generate_genome(population[i]);
    }

    // Generációk
    for (int generation = 0; generation < MAX_GENERATIONS; generation++) {
        // Fitness számítás
        for (int i = 0; i < POP_SIZE; i++) {
            fitness_values[i] = fitness(population[i]);
        }

        // Kiválasztás
        int parent1, parent2;
        selection(population, fitness_values, &parent1, &parent2);

        // Keresztezés és mutáció
        int child[GENOME_SIZE];
        crossover(population[parent1], population[parent2], child);
        mutate(child);

        // Populáció frissítése (az új generáció beillesztése)
        for (int i = 0; i < GENOME_SIZE; i++) {
            population[parent1][i] = child[i];
        }

        // A legjobb eredmény kiíratása
        printf("Generáció %d, legjobb fitness: %d\n", generation, fitness_values[parent1]);
    }
}

int main() {
    srand(time(NULL));
    genetic_algorithm();
    return 0;
}
