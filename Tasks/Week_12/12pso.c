#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NUM_PARTICLES 30
#define NUM_DIMENSIONS 2
#define MAX_ITERATIONS 1000
#define W 0.5 // Inertial weight
#define C1 1.5 // Cognitive weight
#define C2 1.5 // Social weight

// Részecske struktúra
typedef struct {
    double position[NUM_DIMENSIONS];
    double velocity[NUM_DIMENSIONS];
    double best_position[NUM_DIMENSIONS];
    double best_value;
} Particle;

// Funkció, amit optimalizálni akarunk (pl. f(x, y) = x^2 + y^2)
double objective_function(double position[]) {
    return position[0] * position[0] + position[1] * position[1];  // f(x, y) = x^2 + y^2
}

// Részecske inicializálása
void initialize_particle(Particle* p) {
    for (int i = 0; i < NUM_DIMENSIONS; i++) {
        p->position[i] = ((double)rand() / RAND_MAX) * 10 - 5;  // Véletlenszerű pozíció [-5, 5]
        p->velocity[i] = ((double)rand() / RAND_MAX) * 2 - 1;  // Véletlenszerű sebesség [-1, 1]
        p->best_position[i] = p->position[i];
    }
    p->best_value = objective_function(p->position);
}

// Részecske frissítése
void update_particle(Particle* p, double global_best_position[]) {
    for (int i = 0; i < NUM_DIMENSIONS; i++) {
        double r1 = ((double)rand() / RAND_MAX);
        double r2 = ((double)rand() / RAND_MAX);
        
        // Sebesség frissítése
        p->velocity[i] = W * p->velocity[i] + C1 * r1 * (p->best_position[i] - p->position[i]) +
                         C2 * r2 * (global_best_position[i] - p->position[i]);
        
        // Pozíció frissítése
        p->position[i] += p->velocity[i];
    }

    // Legjobb érték frissítése
    double value = objective_function(p->position);
    if (value < p->best_value) {
        p->best_value = value;
        for (int i = 0; i < NUM_DIMENSIONS; i++) {
            p->best_position[i] = p->position[i];
        }
    }
}

// PSO algoritmus végrehajtása
void pso() {
    Particle particles[NUM_PARTICLES];
    double global_best_position[NUM_DIMENSIONS];
    double global_best_value = INFINITY;

    // Inicializáljuk a részecskéket
    for (int i = 0; i < NUM_PARTICLES; i++) {
        initialize_particle(&particles[i]);
        if (particles[i].best_value < global_best_value) {
            global_best_value = particles[i].best_value;
            for (int j = 0; j < NUM_DIMENSIONS; j++) {
                global_best_position[j] = particles[i].best_position[j];
            }
        }
    }

    // PSO iterációk
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        for (int i = 0; i < NUM_PARTICLES; i++) {
            update_particle(&particles[i], global_best_position);
            if (particles[i].best_value < global_best_value) {
                global_best_value = particles[i].best_value;
                for (int j = 0; j < NUM_DIMENSIONS; j++) {
                    global_best_position[j] = particles[i].best_position[j];
                }
            }
        }

        // Kiíratás minden 100. iterációban
        if (iter % 100 == 0) {
            printf("Iteráció: %d, Legjobb érték: %f\n", iter, global_best_value);
        }
    }

    printf("Globális legjobb pozíció: (%f, %f) és érték: %f\n", global_best_position[0], global_best_position[1], global_best_value);
}

int main() {
    srand(time(NULL));
    pso();
    return 0;
}
