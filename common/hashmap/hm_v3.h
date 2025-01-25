#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

// HashMap Structures
typedef struct hashmap hashmap;

typedef struct {
    const char* key;
    void* value;
    hashmap* _map;
    size_t _idx;
} hashmap_iti;

// Public API
hashmap* hashmap_create(void);
void hashmap_delete(hashmap* map);
void* hashmap_get(hashmap* map, const char* key);
const char* hashmap_set(hashmap* map, const char* key, void* value, size_t value_size);
void hashamp_delete_kv(hashmap* map, const char* key);
size_t hashmap_len(hashmap* map);

hashmap_iti hashmap_iter(hashmap* map);
bool hashmap_nxt(hashmap_iti* im);