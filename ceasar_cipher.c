#include <stdio.h>
#include <ctype.h>
#include <string.h>

void encryptCaesar(char text[], int shift) {
    // Normalize shift to stay within [0, 25]
    shift = (shift % 26 + 26) % 26;

    for (int i = 0; text[i] != '\0'; i++) {
        if (isupper(text[i])) {
            text[i] = (char)(((text[i] - 'A' + shift) % 26) + 'A');
        } else if (islower(text[i])) {
            text[i] = (char)(((text[i] - 'a' + shift) % 26) + 'a');
        }
    }
}

void decryptCaesar(char text[], int shift) {
    // Decryption is simply shifting by (26 - shift)
    encryptCaesar(text, 26 - (shift % 26));
}

int main() {
    char message[256];
    int shift;

    printf("Enter a message to encrypt: ");
    if (fgets(message, sizeof(message), stdin) != NULL) {
        // Remove trailing newline if present
        message[strcspn(message, "\n")] = '\0';
    }

    printf("Enter shift key (integer): ");
    scanf("%d", &shift);

    // Encrypt
    encryptCaesar(message, shift);
    printf("Encrypted message: %s\n", message);

    // Decrypt
    decryptCaesar(message, shift);
    printf("Decrypted message: %s\n", message);

    return 0;
}