#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_POINTS 100

// Pont struktúra
typedef struct {
    int x, y;
} Point;

// Segéd függvények
int orientation(Point p, Point q, Point r) {
    // Visszatér az iránytól függően:
    // 0 -> P, Q, R három pont egy vonalban van
    // 1 -> P -> Q -> R jobbra
    // -1 -> P -> Q -> R balra
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    return (val > 0) ? 1 : (val < 0) ? -1 : 0;
}

void swap(Point* p1, Point* p2) {
    Point temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

// Távolság számítása egy pont és egy egyenes között
int distance(Point p, Point q, Point r) {
    return abs((r.y - q.y) * p.x - (r.x - q.x) * p.y + r.x * q.y - r.y * q.x);
}

// Visszaadja a konvex burok pontjait
void quickHull(Point points[], int n, Point p1, Point p2, int side, Point convexHull[], int* hull_size) {
    int index = -1;
    int max_dist = 0;

    // A legnagyobb távolságú pont keresése a vonaltól (p1, p2)
    for (int i = 0; i < n; i++) {
        int dist = distance(points[i], p1, p2);
        if (orientation(p1, p2, points[i]) == side && dist > max_dist) {
            index = i;
            max_dist = dist;
        }
    }

    // Ha nincs pont, akkor hozzáadjuk a két pontot a konvex burokhoz
    if (index == -1) {
        convexHull[*hull_size] = p1;
        (*hull_size)++;
        convexHull[*hull_size] = p2;
        (*hull_size)++;
        return;
    }

    // Rekurzív lépések: bal és jobb oldali pontok keresése
    Point p = points[index];
    quickHull(points, n, p1, p, -orientation(p1, p, p2), convexHull, hull_size);
    quickHull(points, n, p, p2, -orientation(p, p2, p1), convexHull, hull_size);
}

// Funkció a konvex burok meghatározására
void convexHull(Point points[], int n) {
    // Ha kevesebb, mint 3 pont van, nem lehet konvex burok
    if (n < 3) {
        printf("Konvex burok nem létezik, kevesebb mint 3 pont.\n");
        return;
    }

    int hull_size = 0;
    Point convexHull[MAX_POINTS];

    // Legszélső bal és jobb pontok keresése
    int leftmost = 0, rightmost = 0;
    for (int i = 1; i < n; i++) {
        if (points[i].x < points[leftmost].x)
            leftmost = i;
        if (points[i].x > points[rightmost].x)
            rightmost = i;
    }

    // Hívjuk a rekurszív QuickHull algoritmust
    quickHull(points, n, points[leftmost], points[rightmost], 1, convexHull, &hull_size);
    quickHull(points, n, points[leftmost], points[rightmost], -1, convexHull, &hull_size);

    // Kiíratjuk a konvex burok pontjait
    printf("\nA konvex burok pontjai:\n");
    for (int i = 0; i < hull_size; i++) {
        printf("(%d, %d)\n", convexHull[i].x, convexHull[i].y);
    }
}

// Példa a QuickHull algoritmus használatára
int main() {
    Point points[] = {
        {0, 3}, {2, 2}, {1, 1}, {2, 1}, {3, 0}, {0, 0}, {3, 3}, {5, 1}, {4, 4}
    };
    int n = sizeof(points) / sizeof(points[0]);

    convexHull(points, n);
    return 0;
}
