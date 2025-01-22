#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

// Defining Stuff that'll move to hashmap.h

typedef struct hm hm;
typedef struct {
    const char* key;
    void* value;      
    hm* _map; 
    size_t _idx;

} hmi;

hmi ht_iterator(hm* map);
bool ht_next(hmi* im);

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
    void* value;
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

void* hm_get (hm* map, const char* key) {
    // Hash Module for index setting
    uint64_t hash = key_hasher(key);
    size_t idx = (size_t)(hash & (uint64_t)(map->capacity - 1));

    while (map->entries[idx].key != NULL) {
        if (strcmp(key, map->entries[idx].key) == 0) {
            return map->entries[idx].value;
        }
        // Linear probing as key wasn't in the slot;
        idx++;
        idx = (idx + 1) % map->capacity;
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

int main(){
    char* ex_string = "Hello World";
    uint64_t hashedkey = key_hasher(ex_string);
    printf("Hashed Key: %llu \n", hashedkey);

    hm* map = hm_create();
    if (map == NULL) {
        return 1;
    }

    printf("Hash map created with capacity: %zu\n", map->capacity);

    hm_set(map, "Bob", "Ross");
    hm_set(map, "Harry", "Potter");
    hm_set(map, "Percy", "Jackson");

    char* value_test = (char*)hm_get(map, "Bob");
    printf("Test: %s\n", value_test);

    size_t len = hm_len(map);
    printf("Hash Map Length: %zu\n", len);

    hmi iter = ht_iter(map);
    while (hm_nxt(&iter)) {
        printf("Key: %s, Value: %s\n", iter.key, (char*)iter.value);
    }

    ht_delete(map);
    printf("Done\n");
    
    return 0;
}