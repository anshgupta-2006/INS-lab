#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5

char matrix[SIZE][SIZE];

// Generate 5x5 Key Matrix
void generateMatrix(const char *key) {
    int visited[26] = {0};
    visited['J' - 'A'] = 1; // Treat 'J' as 'I'

    int row = 0, col = 0;

    // Insert key characters into matrix
    for (int i = 0; key[i] != '\0'; i++) {
        char ch = toupper(key[i]);
        if (ch == 'J') ch = 'I';

        if (isalpha(ch) && !visited[ch - 'A']) {
            visited[ch - 'A'] = 1;
            matrix[row][col++] = ch;
            if (col == SIZE) {
                col = 0;
                row++;
            }
        }
    }

    // Fill remaining positions with the rest of the alphabet
    for (char ch = 'A'; ch <= 'Z'; ch++) {
        if (!visited[ch - 'A']) {
            visited[ch - 'A'] = 1;
            matrix[row][col++] = ch;
            if (col == SIZE) {
                col = 0;
                row++;
            }
        }
    }
}

// Find position of a character in the matrix
void findPosition(char ch, int *row, int *col) {
    if (ch == 'J') ch = 'I';
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (matrix[i][j] == ch) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

// Preprocess plaintext: remove non-alphabetic chars, handle duplicates & odd lengths
int prepareText(const char *input, char *output) {
    char cleaned[500];
    int len = 0;

    // Filter alphabetic characters and convert to uppercase
    for (int i = 0; input[i] != '\0'; i++) {
        if (isalpha(input[i])) {
            char ch = toupper(input[i]);
            cleaned[len++] = (ch == 'J') ? 'I' : ch;
        }
    }

    int outIdx = 0;
    for (int i = 0; i < len; i++) {
        output[outIdx++] = cleaned[i];
        if (i + 1 < len) {
            if (cleaned[i] == cleaned[i + 1]) {
                output[outIdx++] = 'X'; // Insert filler character for identical pairs
            } else {
                output[outIdx++] = cleaned[++i];
            }
        }
    }

    // Pad with 'X' if total length is odd
    if (outIdx % 2 != 0) {
        output[outIdx++] = 'X';
    }
    output[outIdx] = '\0';
    return outIdx;
}

// Encrypt digrams
void encrypt(const char *plaintext, char *ciphertext) {
    int r1, c1, r2, c2;
    int len = strlen(plaintext);

    for (int i = 0; i < len; i += 2) {
        findPosition(plaintext[i], &r1, &c1);
        findPosition(plaintext[i + 1], &r2, &c2);

        if (r1 == r2) {
            // Same row -> shift right
            ciphertext[i]     = matrix[r1][(c1 + 1) % SIZE];
            ciphertext[i + 1] = matrix[r2][(c2 + 1) % SIZE];
        } else if (c1 == c2) {
            // Same column -> shift down
            ciphertext[i]     = matrix[(r1 + 1) % SIZE][c1];
            ciphertext[i + 1] = matrix[(r2 + 1) % SIZE][c2];
        } else {
            // Rectangle -> swap columns
            ciphertext[i]     = matrix[r1][c2];
            ciphertext[i + 1] = matrix[r2][c1];
        }
    }
    ciphertext[len] = '\0';
}

// Decrypt digrams
void decrypt(const char *ciphertext, char *plaintext) {
    int r1, c1, r2, c2;
    int len = strlen(ciphertext);

    for (int i = 0; i < len; i += 2) {
        findPosition(ciphertext[i], &r1, &c1);
        findPosition(ciphertext[i + 1], &r2, &c2);

        if (r1 == r2) {
            // Same row -> shift left (handle negative wrap-around)
            ciphertext[i];
            plaintext[i]     = matrix[r1][(c1 + SIZE - 1) % SIZE];
            plaintext[i + 1] = matrix[r2][(c2 + SIZE - 1) % SIZE];
        } else if (c1 == c2) {
            // Same column -> shift up (handle negative wrap-around)
            plaintext[i]     = matrix[(r1 + SIZE - 1) % SIZE][c1];
            plaintext[i + 1] = matrix[(r2 + SIZE - 1) % SIZE][c2];
        } else {
            // Rectangle -> swap columns
            plaintext[i]     = matrix[r1][c2];
            plaintext[i + 1] = matrix[r2][c1];
        }
    }
    plaintext[len] = '\0';
}

int main() {
    char key[] = "MONARCHY";
    char rawText[] = "INSTRUMENTS";

    char preparedText[500];
    char cipherText[500];
    char decryptedText[500];

    // Build the 5x5 key matrix
    generateMatrix(key);

    printf("=== 5x5 Key Matrix ===\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }

    // Prepare, Encrypt, and Decrypt
    prepareText(rawText, preparedText);
    encrypt(preparedText, cipherText);
    decrypt(cipherText, decryptedText);

    printf("\nOriginal Text:   %s", rawText);
    printf("\nPrepared Text:   %s", preparedText);
    printf("\nCiphertext:      %s", cipherText);
    printf("\nDecrypted Text:  %s\n", decryptedText);

    return 0;
}