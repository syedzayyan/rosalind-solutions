
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char key[21];
    char value[21];
} simpmap;



int main() {

    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    int num_items;    
    scanf("%i", &num_items);
    
    simpmap phone_book[num_items]; 
    
    // Read key-value pairs
    for (int i = 0; i < num_items; i++) {
        char temp_key[2000];
        scanf("%[^\n]", temp_key);
        
        char *token = strtok(temp_key, " ");
        if (token != NULL) {
            strcpy(phone_book[i].key, token);
            token = strtok(NULL, " ");
            if (token != NULL) {
                strcpy(phone_book[i].value, token);
            } else {
                strcpy(phone_book[i].value, "");
            }
        }
    }
    
    // Simplistic Linear Search Algorithm
    char query[21];
    while (scanf("%s", query) != EOF) {
        int found = 0;
        for (int i = 0; i < num_items; i++) {
            if (strcmp(phone_book[i].key, query) == 0) {
                printf("%s=%s\n", phone_book[i].key, phone_book[i].value);
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("Not found\n");
        }
    }
    
    return 0;
}