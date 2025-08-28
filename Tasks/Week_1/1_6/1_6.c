#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL)); // Véletlenszám generálás inicializálása

    // Véletlenszerű számok generálása
    int num1 = rand() % 100 + 1;  // 1 és 100 közötti véletlenszám
    int num2 = rand() % 100 + 1;  // 1 és 100 közötti véletlenszám

    // Kiírás a felhasználónak
    printf("Számold ki a két szám összegét: %d + %d = ?\n", num1, num2);

    // Időmérés kezdete
    time_t start_time, end_time;
    time(&start_time);

    // Felhasználói válasz beolvasása
    int user_answer;
    scanf("%d", &user_answer);

    // Időmérés vége
    time(&end_time);
    double time_taken = difftime(end_time, start_time);

    // Ellenőrzés és visszajelzés
    if (user_answer == (num1 + num2)) {
        printf("Helyes válasz!\n");
    } else {
        printf("Hibás válasz! A helyes válasz: %d\n", num1 + num2);
    }

    // Idő kiírása
    printf("A válaszadás ideje: %.2f másodperc\n", time_taken);

    return 0;
}
