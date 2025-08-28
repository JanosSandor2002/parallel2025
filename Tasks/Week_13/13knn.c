#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_POINTS 100  // Maximum adatpontok száma
#define MAX_DIMENSIONS 2  // Adatok dimenziója (pl. 2D)

typedef struct {
    double data[MAX_DIMENSIONS];
    int label;  // Címke (osztály)
} Point;

// Euklideszi távolság számítása
double euclidean_distance(Point p1, Point p2) {
    double distance = 0.0;
    for (int i = 0; i < MAX_DIMENSIONS; i++) {
        distance += (p1.data[i] - p2.data[i]) * (p1.data[i] - p2.data[i]);
    }
    return sqrt(distance);
}

// k-NN algoritmus
int knn(Point* points, int num_points, Point query, int k) {
    double distances[MAX_POINTS];
    int labels[MAX_POINTS];

    // Távolságok kiszámítása
    for (int i = 0; i < num_points; i++) {
        distances[i] = euclidean_distance(points[i], query);
        labels[i] = points[i].label;
    }

    // A k legkisebb távolságok keresése
    for (int i = 0; i < k; i++) {
        for (int j = i + 1; j < num_points; j++) {
            if (distances[i] > distances[j]) {
                // Csere
                double temp_dist = distances[i];
                distances[i] = distances[j];
                distances[j] = temp_dist;

                int temp_label = labels[i];
                labels[i] = labels[j];
                labels[j] = temp_label;
            }
        }
    }

    // A k leggyakoribb címke visszaadása
    int count[2] = {0};  // Feltételezzük, hogy két osztály van: 0 és 1
    for (int i = 0; i < k; i++) {
        count[labels[i]]++;
    }

    return count[0] > count[1] ? 0 : 1;  // Visszaadjuk a gyakoribb osztályt
}

int main() {
    Point points[MAX_POINTS] = {
        {{2.0, 3.0}, 0},
        {{3.0, 3.0}, 0},
        {{6.0, 5.0}, 1},
        {{8.0, 8.0}, 1},
        {{1.0, 2.0}, 0},
        {{5.0, 4.0}, 1}
    };

    Point query = {{4.0, 4.0}, -1};  // A kérdéses pont, amit osztályozni akarunk
    int k = 3;

    int predicted_label = knn(points, 6, query, k);

    printf("A kérdéses pont osztálya: %d\n", predicted_label);
    return 0;
}
