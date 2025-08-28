#include <stdio.h> 

// Szóközökkel történő kiírás (jobbra igazítva)
void print_with_spaces(int num) {
    printf("%8d\n", num);  // Jobbra igazítva, szóközökkel kitöltve
}

// 0-ás kitöltéssel történő kiírás (jobbra igazítva)
void print_with_zeros(int num) {
    printf("%08d\n", num);  // Jobbra igazítva, 0-val kitöltve
}

//Alapértelmezett kiírás //
void print_default(int num) {
    printf("%d\n", num);  // Alapértelmezett kiírás
}

int main() {
    int num = 42;  // Példaszám

    // Különböző függvények meghívása
    printf("Szóközökkel kitöltve:\n");
    print_with_spaces(num);
    
    printf("0-val kitöltve:\n");
    print_with_zeros(num);

    printf("Alapértelmezett kiírás:\n");
    print_default(num);

    return 0;
}
