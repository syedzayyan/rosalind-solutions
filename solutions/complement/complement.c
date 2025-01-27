#include <stdio.h>
#include <string.h>
#include <common.h>

// Function to complement the DNA sequence
// Function to complement the DNA sequence
void complement_dna(char *str) {
    while (*str) {
        switch (*str) {
            case 'A': *str = 'T'; break;
            case 'C': *str = 'G'; break;
            case 'G': *str = 'C'; break;
            case 'T': *str = 'A'; break;
        }
        str++;
    }
}

// https://www.geeksforgeeks.org/reverse-string-in-c/
void reverse_string(char *str) {
    int length = strlen(str);
    int start = 0;
    int end = length - 1;

    while (start < end) {
        // Swap characters
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;

        // Move towards the middle
        start++;
        end--;
    }
}

int main() {
    char *str = file_to_char("rosalind_revc.txt");

    printf("Original string: %s\n", str);

    // Complement the DNA sequence
    complement_dna(str);

    // Reverse the complemented string
    reverse_string(str);

    printf("Complemented and reversed string: %s\n", str);

    return 0;
}
