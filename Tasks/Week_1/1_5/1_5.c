#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int random_integer(int min, int max) {
    return rand() % (max - min + 1) + min; // Véletlenszám generálás a min és max közötti intervallumban
}

int main(int argc, char *argv[]) {
    // Argumentumok ellenőrzése
    if (argc != 3) {
        printf("Hiba: Két egész számot kell megadni! Használat: program min max\n");
        return 1;  // Hibakód
    }

    // Argumentumok konvertálása
    int min = atoi(argv[1]);
    int max = atoi(argv[2]);

    // Ellenőrizzük, hogy a min kisebb-e, mint a max
    if (min > max) {
        printf("Hiba: Az első számnak kisebbnek kell lennie, mint a második szám!\n");
        return 1;
    }

    // Véletlenszám generálása
    srand(time(NULL));  // Véletlenszám generálás inicializálása

    int random_num = random_integer(min, max);

    // Eredmény kiírása
    printf("A generált véletlenszám: %d\n", random_num);

    return 0;
}
