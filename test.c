#include "common.h"
#include "string.h"

void test_hashmap_functions() {
    // Create a hashmap
    hashmap* map = hashmap_create();
    if (!map) {
        printf("Failed to create hashmap\n");
        return;
    }

    // Test case 1: Set and Get operations
    int value1 = 42;
    int value2 = 84;
    const char* key1 = "key1";
    const char* key2 = "key2";

    hashmap_set(map, key1, &value1, sizeof(value1));
    hashmap_set(map, key2, &value2, sizeof(value2));

    // Retrieve values
    int* retrieved_value1 = (int*)hashmap_get(map, key1);
    int* retrieved_value2 = (int*)hashmap_get(map, key2);

    if (retrieved_value1 && *retrieved_value1 == value1) {
        printf("Get test passed for key1: %d\n", *retrieved_value1);
    } else {
        printf("Get test failed for key1\n");
    }

    if (retrieved_value2 && *retrieved_value2 == value2) {
        printf("Get test passed for key2: %d\n", *retrieved_value2);
    } else {
        printf("Get test failed for key2\n");
    }

    // Test case 2: Delete operation
    hashamp_delete_kv(map, key1);
    retrieved_value1 = (int*)hashmap_get(map, key1);

    if (retrieved_value1 == NULL) {
        printf("Delete test passed for key1\n");
    } else {
        printf("Delete test failed for key1\n");
    }

    retrieved_value2 = (int*)hashmap_get(map, key2);
    if (retrieved_value2 && *retrieved_value2 == value2) {
        printf("Key2 remains intact after delete test\n");
    } else {
        printf("Key2 failed integrity test after deletion\n");
    }

    // Test case 3: Check length
    size_t length = hashmap_len(map);
    if (length == 1) {
        printf("Length test passed, length: %zu\n", length);
    } else {
        printf("Length test failed, length: %zu\n", length);
    }

    // Clean up
    hashmap_delete(map);
}

int main() {
    test_hashmap_functions();
    return 0;
}
