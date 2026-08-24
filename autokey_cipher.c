#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void encryptAutokey(const char *plaintext, const char *key, char *ciphertext) {
    int pLen = strlen(plaintext);
    int kLen = strlen(key);
    char *fullKey = (char *)malloc(pLen + 1);

    for (int i = 0; i < kLen && i < pLen; i++) {
        fullKey[i] = toupper(key[i]);
    }
    for (int i = kLen; i < pLen; i++) {
        fullKey[i] = toupper(plaintext[i - kLen]);
    }
    fullKey[pLen] = '\0';

    for (int i = 0; i < pLen; i++) {
        if (isalpha(plaintext[i])) {
            char base = isupper(plaintext[i]) ? 'A' : 'a';
            int pVal = toupper(plaintext[i]) - 'A';
            int kVal = fullKey[i] - 'A';
            ciphertext[i] = ((pVal + kVal) % 26) + base;
        } else {
            ciphertext[i] = plaintext[i]; // Keep non-alphabet characters as-is
        }
    }
    ciphertext[pLen] = '\0';
    free(fullKey);
}

void decryptAutokey(const char *ciphertext, const char *key, char *plaintext) {
    int cLen = strlen(ciphertext);
    int kLen = strlen(key);
    char *fullKey = (char *)malloc(cLen + 1);

    for (int i = 0; i < kLen && i < cLen; i++) {
        fullKey[i] = toupper(key[i]);
    }

    // Plaintext characters dynamically extend the keystream
    for (int i = 0; i < cLen; i++) {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            int cVal = toupper(ciphertext[i]) - 'A';
            int kVal = fullKey[i] - 'A';
            int pVal = (cVal - kVal + 26) % 26;

            plaintext[i] = pVal + base;

            if (i + kLen < cLen) {
                fullKey[i + kLen] = toupper(plaintext[i]);
            }
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[cLen] = '\0';
    free(fullKey);
}

int main() {
    char plaintext[] = "ATTACKATDAWN";
    char key[] = "QUEEN";

    int len = strlen(plaintext);
    char *ciphertext = (char *)malloc(len + 1);
    char *decrypted = (char *)malloc(len + 1);

    encryptAutokey(plaintext, key, ciphertext);
    decryptAutokey(ciphertext, key, decrypted);

    printf("Plaintext:  %s\n", plaintext);
    printf("Key:        %s\n", key);
    printf("Ciphertext: %s\n", ciphertext);
    printf("Decrypted:  %s\n", decrypted);

    free(ciphertext);
    free(decrypted);
    return 0;
}