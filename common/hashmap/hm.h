#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

// HashMap Structures
typedef struct hm hm;

typedef struct {
    const char* key;
    void* value;
    hm* _map;
    size_t _idx;
} hmi;

// Public API
hm* hm_create(void);
void ht_delete(hm* map);
void* hm_get(hm* map, const char* key);
const char* hm_set(hm* map, const char* key, void* value);
size_t hm_len(hm* map);

hmi ht_iter(hm* map);
bool hm_nxt(hmi* im);