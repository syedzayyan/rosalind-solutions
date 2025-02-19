#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "hm.h"

// Hash Function - FNV-1a hash algorithm
#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

static uint64_t key_hasher (const char* key) {
    uint64_t hash = FNV_OFFSET;
    for (const char*p = key; *p; p++) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

// HashMap Guts

typedef struct {
    const char* key; 
    void* value; // void* here is a placeholder for type switchable stuff in C
    size_t pd; //prob distance;
} entries_in_hm;


struct hm {
    entries_in_hm* entries;
    size_t capacity;
    size_t len;
};

#define INITIAL_CAPACITY 16

hm* hm_create(void) {
    // Allocation of space for hash table with NULL Values
    hm* map = calloc(1, sizeof(hm));
    if (map == NULL) {
        return NULL;
    }
    map->len = 0;
    map->capacity = INITIAL_CAPACITY;
    map->entries = calloc(map->capacity, sizeof(hm));

    if (map->entries == NULL) {
        free(map);
        return NULL;
    }
    return map;
}

void ht_delete(hm* map) {
    for (size_t i = 0; i < map->capacity; i++) {
        free((void*)map->entries[i].key);
    }
    free(map->entries);
    free(map);
}

void hm_delete_kv(hm* map, const char* key) {
    uint64_t hash = key_hasher(key);
    size_t idx = (size_t)(hash & (uint64_t)(map->capacity - 1));
    size_t start_idx = idx; // To detect full loop

    // Step 1: Find the entry with the given key
    while (map->entries[idx].key != NULL) {
        if (strcmp(map->entries[idx].key, key) == 0) {
            // Step 2: Deallocate key and value
            free((char*)map->entries[idx].key);
            if (map->entries[idx].value != NULL) {
                free(map->entries[idx].value);  // Assuming the value was dynamically allocated
            }
            
            map->entries[idx].key = NULL;  // Mark the slot as empty
            
            // Step 3: Shift subsequent entries if necessary
            size_t next_idx = (idx + 1) & (map->capacity - 1);
            while (map->entries[next_idx].key != NULL) {
                uint64_t next_hash = key_hasher(map->entries[next_idx].key);
                size_t expected_idx = (size_t)(next_hash & (uint64_t)(map->capacity - 1));

                // If the current entry has a larger probe distance than the next, it's in the wrong spot
                // Move it to fill the gap
                if ((next_idx - expected_idx + map->capacity) % map->capacity < map->entries[next_idx].pd) {
                    break; // No need to move this entry, it's not part of the deleted probe chain
                }

                // Step 4: Shift the next entry into the current one
                map->entries[idx] = map->entries[next_idx];
                map->entries[next_idx].key = NULL;
                idx = next_idx;
                next_idx = (idx + 1) & (map->capacity - 1);
            }

            // Step 5: Reduce the length of the map after deletion
            map->len--;
            break;
        }

        // Linear probing as key wasn't in the slot.
        idx = (idx + 1) & (map->capacity - 1); // Wrap around using bitwise AND.
        if (idx == start_idx) {
            break; // Full loop detected; key not found.
        }
    }
}


void* hm_get(hm* map, const char* key) {
    uint64_t hash = key_hasher(key);
    size_t idx = (size_t)(hash & (uint64_t)(map->capacity - 1));
    size_t start_idx = idx; // To detect full loop

    while (map->entries[idx].key != NULL) {
        if (strcmp(map->entries[idx].key, key) == 0) {
            return map->entries[idx].value;
        }
        // Linear probing as key wasn't in the slot.
        idx = (idx + 1) & (map->capacity - 1); // Wrap around using bitwise AND.
        if (idx == start_idx) {
            break; // Full loop detected; key not found.
        }
    }
    return NULL;
}

static const char* hm_set_entry(entries_in_hm* entries, size_t capacity, const char* key, void* value, size_t* plen) {
    uint64_t hash = key_hasher(key);
    size_t idx = (size_t)(hash & (uint64_t)(capacity - 1));
    size_t pd = 0;

    while (entries[idx].key != NULL) {
        // Check if the k-v index slot is free
        if (strcmp(key, entries[idx].key) == 0) {
            entries[idx].value = value;
            return entries[idx].key;
        }
        
        // Update from hashmap in linear probing:
        if (entries[idx].pd < pd) { // The Robin Hood comparison of stealing from index items close to home.
            const char* temp_key = entries[idx].key;
            void* temp_value = entries[idx].value;
            size_t temp_pd = entries[idx].pd;

            entries[idx].key = key;
            entries[idx].value = value;
            entries[idx].pd = pd;

            key = temp_key;
            value = temp_value;
            pd = temp_pd;
        }

        idx = (idx + 1) % capacity;
        pd++;
        
        if (idx >= capacity) {
            idx = 0;
        }
    }

    if (plen != NULL) {
        key = strdup(key);
        if (key == NULL) {
            return NULL;
        }
        (*plen)++;
    }

    entries[idx].key = (char*)key;
    entries[idx].value = value;
    entries[idx].pd = pd;
    return key;
}

static bool hm_expand(hm* map) {
    size_t new_cap = map->capacity * 2;
    if (new_cap < map->capacity) {
        return false;
    }
    entries_in_hm* new_entries = calloc(new_cap, sizeof(entries_in_hm));

    if (new_entries == NULL) {
        return false;
    }

    for (size_t i = 0; i < map->capacity; i++) {
        entries_in_hm entry = map->entries[i];
        if (entry.key != NULL) {
            hm_set_entry(new_entries, new_cap, entry.key, entry.value, NULL);
        }
    }

    free(map->entries);
    map->entries = new_entries;
    map->capacity = new_cap;
    return true;
}

// Complicated with more inner functions since setting new value involves/
// moving around memory and stuffh


const char* hm_set(hm* map, const char* key, void* value) {
    assert(value != NULL);
    if (value == NULL) {
        return NULL;
    }

    if (map->len >= map->capacity / 2) {
        if (!hm_expand(map)) {
            return NULL;
        }
    }

    return hm_set_entry(map->entries, map->capacity, key, value, &map->len);
}

size_t hm_len(hm* map) {
    return map->len;
}

hmi ht_iter(hm* map) {
    hmi it;
    it._map = map;
    it._idx = 0;
    return it;
}

bool hm_nxt(hmi* im){
    hm* map = im->_map;
    while (im->_idx < map->capacity) {
        size_t i = im->_idx;
        im->_idx++;
        if (map->entries[i].key != NULL) {
            entries_in_hm entry = map->entries[i];
            im->key = entry.key;
            im->value = entry.value;
            return true;
        }
    }
    return false;
}