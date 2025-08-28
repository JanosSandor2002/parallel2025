#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Tömbrangsort implementáció
#define MAX_VALUE 10  // Maximális érték a listaelemek között
#define BUCKET_COUNT 10 // Bucketek száma (a maximális érték alapján)

void bucket_sort(int arr[], int n) {
    int i, j;
    
    // Létrehozzuk a bucketeket
    int buckets[BUCKET_COUNT][n];
    int bucket_sizes[BUCKET_COUNT] = {0};

    // Az összes elem csoportosítása a megfelelő bucketekbe
    for (i = 0; i < n; i++) {
        int index = arr[i] / BUCKET_COUNT;  // bucket index kiszámítása
        buckets[index][bucket_sizes[index]] = arr[i];
        bucket_sizes[index]++;
    }

    // Minden bucketet külön-külön rendezünk
    for (i = 0; i < BUCKET_COUNT; i++) {
        if (bucket_sizes[i] > 0) {
            // Egyszerű buborékrendezés a bucketekben
            for (j = 0; j < bucket_sizes[i] - 1; j++) {
                for (int k = j + 1; k < bucket_sizes[i]; k++) {
                    if (buckets[i][j] > buckets[i][k]) {
                        int temp = buckets[i][j];
                        buckets[i][j] = buckets[i][k];
                        buckets[i][k] = temp;
                    }
                }
            }
        }
    }

    // Elemek kiírása a rendezett sorrendben
    int idx = 0;
    for (i = 0; i < BUCKET_COUNT; i++) {
        for (j = 0; j < bucket_sizes[i]; j++) {
            arr[idx++] = buckets[i][j];
        }
    }
}

// Vezetőválasztás szimulációk
// LeLann és Chang-Roberts algoritmusok

// LeLann algoritmus
void leLann_algorithm(int n) {
    int steps = 0;
    int messages = 0;
    
    // Szimuláljuk a LeLann algoritmust: minden csomópont küld üzenetet és vár választ
    // n csomópont esetén n lépést szükséges végrehajtani
    steps = n - 1;
    messages = 2 * (n - 1);  // Mivel két üzenetet küld minden csomópont

    printf("LeLann algoritmus:\n");
    printf("Lépések száma: %d\n", steps);
    printf("Üzenetek száma: %d\n", messages);
}

// Chang-Roberts algoritmus
void changRoberts_algorithm(int n) {
    int steps = 0;
    int messages = 0;

    // Szimuláljuk a Chang-Roberts algoritmust: minden csomópont iterál
    steps = (int)log2(n);
    messages = n * (int)log2(n);  // Néhány üzenet áramlik át minden lépésben

    printf("Chang-Roberts algoritmus:\n");
    printf("Lépések száma: %d\n", steps);
    printf("Üzenetek száma: %d\n", messages);
}

int main() {
    // Tömbrangsort példa
    int arr[] = {5, 1, 3, 2, 7, 4, 2};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Eredeti tömb: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    bucket_sort(arr, n); // Tömbrangsort alkalmazása

    printf("Rendezett tömb: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Vezetőválasztás szimuláció
    int n_nodes = 8;  // Gyűrű mérete, 8 csomóponttal
    leLann_algorithm(n_nodes);  // LeLann algoritmus szimulálása
    changRoberts_algorithm(n_nodes);  // Chang-Roberts algoritmus szimulálása

    return 0;
}
