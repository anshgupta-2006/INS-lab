#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Extended Euclidean Algorithm to find modular inverse of a number mod m
int modInverse(int a, int m) {
    a = (a % m + m) % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1)
            return x;
    }
    return -1; // Inverse does not exist
}

// Computes the inverse of a 2x2 matrix modulo 26
int getInverseMatrix(int key[2][2], int invKey[2][2]) {
    int det = key[0][0] * key[1][1] - key[0][1] * key[1][0];
    det = (det % 26 + 26) % 26; // Positive modulo

    int detInv = modInverse(det, 26);
    if (detInv == -1) {
        return 0; // Matrix is not invertible mod 26
    }

    // Inverse of 2x2 matrix = det_inv * adj(K) mod 26
    invKey[0][0] = ( key[1][1] * detInv) % 26;
    invKey[0][1] = (-key[0][1] * detInv % 26 + 26) % 26;
    invKey[1][0] = (-key[1][0] * detInv % 26 + 26) % 26;
    invKey[1][1] = ( key[0][0] * detInv) % 26;

    return 1;
}

// Preprocesses text: converts to uppercase and removes non-alphabet characters
void cleanText(const char *input, char *output) {
    int j = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (isalpha(input[i])) {
            output[j++] = toupper(input[i]);
        }
    }
    output[j] = '\0';
}

void encryptHill(const char *plaintext, int key[2][2], char *ciphertext) {
    int len = strlen(plaintext);
    int idx = 0;

    for (int i = 0; i < len; i += 2) {
        int p1 = plaintext[i] - 'A';
        int p2 = plaintext[i + 1] - 'A';

        // Vector multiplication: C = K * P mod 26
        ciphertext[idx++] = ((key[0][0] * p1 + key[0][1] * p2) % 26) + 'A';
        ciphertext[idx++] = ((key[1][0] * p1 + key[1][1] * p2) % 26) + 'A';
    }
    ciphertext[idx] = '\0';
}

void decryptHill(const char *ciphertext, int invKey[2][2], char *plaintext) {
    int len = strlen(ciphertext);
    int idx = 0;

    for (int i = 0; i < len; i += 2) {
        int c1 = ciphertext[i] - 'A';
        int c2 = ciphertext[i + 1] - 'A';

        // Vector multiplication: P = K^(-1) * C mod 26
        plaintext[idx++] = ((invKey[0][0] * c1 + invKey[0][1] * c2) % 26) + 'A';
        plaintext[idx++] = ((invKey[1][0] * c1 + invKey[1][1] * c2) % 26) + 'A';
    }
    plaintext[idx] = '\0';
}

int main() {
    // Key Matrix K = [[9, 4], [5, 7]]
    int key[2][2] = {
        {9, 4},
        {5, 7}
    };
    
    int invKey[2][2];
    if (!getInverseMatrix(key, invKey)) {
        printf("Error: Key matrix is not invertible modulo 26.\n");
        return 1;
    }

    char rawText[] = "ACTS";
    char cleanedText[256];
    
    cleanText(rawText, cleanedText);

    // Pad with 'X' if length is odd
    int len = strlen(cleanedText);
    if (len % 2 != 0) {
        cleanedText[len] = 'X';
        cleanedText[len + 1] = '\0';
    }

    char ciphertext[256];
    char decryptedText[256];

    encryptHill(cleanedText, key, ciphertext);
    decryptHill(ciphertext, invKey, decryptedText);

    printf("Original Text : %s\n", rawText);
    printf("Processed Text: %s\n", cleanedText);
    printf("Ciphertext    : %s\n", ciphertext);
    printf("Decrypted Text: %s\n", decryptedText);

    return 0;
}