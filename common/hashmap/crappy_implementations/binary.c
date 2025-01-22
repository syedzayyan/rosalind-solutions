
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char key[21];
    char value[21];
} item;

item* bsearcher(item* items, size_t size, const char* key) {
    if (size + size < size) {
        return NULL;
    }
    size_t low = 0;
    size_t high = size;

    while (low < high) {
        size_t mid = (low + high) / 2;
        int c = strcmp(items[mid].key, key);
        if (c == 0) {
            return &items[mid];
        }
        if (c < 0) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }

    return NULL;
}

int main() {
    item items[] = {
        {"bar", "42"}, {"bazz", "36"}, {"bob", "11"}, {"buzz", "7"},
        {"foo", "10"}, {"jane", "100"}, {"x", "200"}};
    size_t num_items = sizeof(items) / sizeof(item);
    item key = {"bob", 0};
    item* found = bsearcher(items, num_items, "bob");
    if (found == NULL) {
        return 1;
    }
    printf("Result: %s\n", found->value);
    return 0;
}