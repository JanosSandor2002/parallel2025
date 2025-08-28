#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_POINTS 100  // Maximum adatpontok száma
#define MAX_DIMENSIONS 2  // Adatok dimenziója (pl. 2D)
#define K 2  // Klaszterek száma
#define MAX_ITERATIONS 100

typedef struct {
    double data[MAX_DIMENSIONS];
} Point;

// Euklideszi távolság számítása
double euclidean_distance(Point p1, Point p2) {
    double distance = 0.0;
    for (int i = 0; i < MAX_DIMENSIONS; i++) {
        distance += (p1.data[i] - p2.data[i]) * (p1.data[i] - p2.data[i]);
    }
    return sqrt(distance);
}

// K-means klaszterezés
void kmeans(Point* points, int num_points, Point* centroids, int* labels) {
    int changed;
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        changed = 0;

        // Klaszterek hozzárendelése
        for (int i = 0; i < num_points; i++) {
            double min_distance = INFINITY;
            int label = -1;

            for (int k = 0; k < K; k++) {
                double dist = euclidean_distance(points[i], centroids[k]);
                if (dist < min_distance) {
                    min_distance = dist;
                    label = k;
                }
            }

            // Ha a címke megváltozott, frissítjük
            if (labels[i] != label) {
                labels[i] = label;
                changed = 1;
            }
        }

        // Új középpontok számítása
        Point new_centroids[K] = {{{0}}};
        int cluster_sizes[K] = {0};

        for (int i = 0; i < num_points; i++) {
            int label = labels[i];
            for (int j = 0; j < MAX_DIMENSIONS; j++) {
                new_centroids[label].data[j] += points[i].data[j];
            }
            cluster_sizes[label]++;
        }

        for (int k = 0; k < K; k++) {
            if (cluster_sizes[k] > 0) {
                for (int j = 0; j < MAX_DIMENSIONS; j++) {
                    new_centroids[k].data[j] /= cluster_sizes[k];
                }
            }
        }

        // Ha nem történt változás, akkor leállunk
        if (!changed) break;

        // Frissítjük a középpontokat
        for (int k = 0; k < K; k++) {
            centroids[k] = new_centroids[k];
        }
    }
}

int main() {
    Point points[MAX_POINTS] = {
        {{1.0, 2.0}},
        {{1.5, 1.8}},
        {{5.0, 8.0}},
        {{8.0, 8.0}},
        {{1.0, 0.6}},
        {{9.0, 11.0}}
    };

    Point centroids[K] = {{{1.0, 2.0}}, {{5.0, 8.0}}};
    int labels[MAX_POINTS] = {0};

    kmeans(points, 6, centroids, labels);

    // Eredmények kiíratása
    for (int i = 0; i < 6; i++) {
        printf("Pont: (%f, %f), Klaszter: %d\n", points[i].data[0], points[i].data[1], labels[i]);
    }

    return 0;
}
