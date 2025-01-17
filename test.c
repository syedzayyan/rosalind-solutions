#include "common.h"
#include "string.h"

int main()
{
    // Hashmap Testing
    hm *map = hm_create();
    if (!map)
    {
        printf("Failed to create hash map\n");
        return 1;
    }

    hm_set(map, "Alice", "Wonderland");
    hm_set(map, "Bob", "Builder");

    printf("Alice: %s\n", (char *)hm_get(map, "Alice"));
    printf("Bob: %s\n", (char *)hm_get(map, "Bob"));

    hmi iter = ht_iter(map);
    while (hm_nxt(&iter))
    {
        printf("Key: %s, Value: %s\n", iter.key, (char *)iter.value);
    }

    ht_delete(map);
}