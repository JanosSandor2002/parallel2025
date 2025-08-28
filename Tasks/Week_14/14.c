#include <stdio.h>
#include <stdbool.h>

#define SIZE 9  // Sudoku tábla mérete (9x9)

// Sudoku tábla érvényességének ellenőrzése
bool is_valid(int board[SIZE][SIZE]) {
    bool row_check[SIZE][SIZE + 1] = {false};  // Sorok érvényességi állapotai
    bool col_check[SIZE][SIZE + 1] = {false};  // Oszlopok érvényességi állapotai
    bool block_check[SIZE][SIZE + 1] = {false}; // 3x3-as blokkok érvényességi állapotai

    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            int num = board[row][col];
            
            if (num != 0) {  // Üres helyeket figyelmen kívül hagyjuk
                int block_index = (row / 3) * 3 + (col / 3);  // A blokk indexének kiszámítása
                
                // Ha a szám már szerepel az adott sorban, oszlopban vagy blokkban, akkor nem érvényes
                if (row_check[row][num] || col_check[col][num] || block_check[block_index][num]) {
                    return false;
                }
                
                // Frissítjük a sor, oszlop és blokk ellenőrzési állapotait
                row_check[row][num] = true;
                col_check[col][num] = true;
                block_check[block_index][num] = true;
            }
        }
    }
    return true;  // Ha nem történt ütközés, akkor érvényes a tábla
}

int main() {
    // Példa Sudoku tábla (0 jelenti az üres helyet)
    int board[SIZE][SIZE] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    // Ellenőrizzük a tábla érvényességét
    if (is_valid(board)) {
        printf("A Sudoku tábla érvényes!\n");
    } else {
        printf("A Sudoku tábla érvénytelen!\n");
    }

    return 0;
}
