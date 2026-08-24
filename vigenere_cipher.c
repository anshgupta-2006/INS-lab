#include <stdio.h>
#include <string.h>
#include <ctype.h>

void encryptVigenere(const char *plaintext, const char *key, char *ciphertext) {
    int pLen = strlen(plaintext);
    int kLen = strlen(key);
    int keyIndex = 0;

    for (int i = 0; i < pLen; i++) {
        if (isalpha(plaintext[i])) {
            char base = isupper(plaintext[i]) ? 'A' : 'a';
            int pVal = toupper(plaintext[i]) - 'A';
            int kVal = toupper(key[keyIndex % kLen]) - 'A';

            ciphertext[i] = ((pVal + kVal) % 26) + base;
            keyIndex++; // Only advance the key on alphabetic characters
        } else {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[pLen] = '\0';
}

void decryptVigenere(const char *ciphertext, const char *key, char *plaintext) {
    int cLen = strlen(ciphertext);
    int kLen = strlen(key);
    int keyIndex = 0;

    for (int i = 0; i < cLen; i++) {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            int cVal = toupper(ciphertext[i]) - 'A';
            int kVal = toupper(key[keyIndex % kLen]) - 'A';

            plaintext[i] = ((cVal - kVal + 26) % 26) + base;
            keyIndex++;
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[cLen] = '\0';
}

int main() {
    char plaintext[] = "Attack at Dawn!";
    char key[] = "LEMON";
    char ciphertext[128];
    char decrypted[128];

    encryptVigenere(plaintext, key, ciphertext);
    decryptVigenere(ciphertext, key, decrypted);

    printf("Plaintext:  %s\n", plaintext);
    printf("Key:        %s\n", key);
    printf("Ciphertext: %s\n", ciphertext);
    printf("Decrypted:  %s\n", decrypted);

    return 0;
}