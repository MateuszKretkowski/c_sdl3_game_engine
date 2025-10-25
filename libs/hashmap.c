#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

// ========== FUNKCJE WEWNĘTRZNE ==========

/**
 * Powiększa pojemność hashmapy i przenosi wszystkie elementy
 */
void hashmap_rehash(HashMap *hm) {
    if (hm == NULL) {
        return;
    }
    
    int old_capacity = hm->capacity;
    hm->capacity = hm->capacity * 2;
    struct Node **newBuckets = calloc(hm->capacity, sizeof(struct Node*));
    
    if (newBuckets == NULL) {
        fprintf(stderr, "Błąd alokacji pamięci w Rehash\n");
        hm->capacity = old_capacity;  // przywróć starą pojemność
        return;
    }
    
    // Iteruj po wszystkich bucketach
    for (int i = 0; i < old_capacity; i++) {
        struct Node *currNode = hm->buckets[i];
        
        // Przenieś wszystkie węzły z łańcucha
        while (currNode != NULL) {
            struct Node *next = currNode->next;
            
            // Przelicz hash dla nowej pojemności
            int index = hm->hash_func(currNode->key) % hm->capacity;
            
            // Wstaw na początek łańcucha w nowym buckecie
            currNode->next = newBuckets[index];
            newBuckets[index] = currNode;
            
            currNode = next;
        }
    }
    
    free(hm->buckets);
    hm->buckets = newBuckets;
}

// ========== FUNKCJE PUBLICZNE ==========

/**
 * Tworzy nową hashmapę
 */
HashMap* hashmap_create(int (*hash_func)(void*), int (*compare_func)(void*, void*)) {
    if (hash_func == NULL || compare_func == NULL) {
        fprintf(stderr, "Błąd: funkcje hash_func i compare_func nie mogą być NULL\n");
        return NULL;
    }
    
    HashMap* newHm = malloc(sizeof(HashMap));
    if (newHm == NULL) {
        fprintf(stderr, "Błąd alokacji pamięci dla HashMap\n");
        return NULL;
    }
    
    newHm->capacity = 8;  // początkowa pojemność
    newHm->size = 0;
    newHm->hash_func = hash_func;
    newHm->compare_func = compare_func;
    newHm->buckets = calloc(newHm->capacity, sizeof(struct Node*));
    
    if (newHm->buckets == NULL) {
        fprintf(stderr, "Błąd alokacji pamięci dla buckets\n");
        free(newHm);
        return NULL;
    }
    
    return newHm;
}

/**
 * Wstawia parę klucz-wartość do hashmapy
 */
void hashmap_insert(HashMap *hm, void *key, void *value) {
    if (hm == NULL || key == NULL) {
        return;
    }

    // Sprawdź czy trzeba powiększyć
    if (hm->size >= hm->capacity * 0.75) {
        hashmap_rehash(hm);
    }
    
    int index = hm->hash_func(key) % hm->capacity;
    
    // Sprawdź czy klucz już istnieje
    struct Node *curr = hm->buckets[index];
    while (curr != NULL) {
        if (hm->compare_func(curr->key, key) == 0) {
            // Klucz istnieje - aktualizuj wartość
            curr->value = value;
            return;
        }
        curr = curr->next;
    }
    
    // Dodaj nowy węzeł
    struct Node *newNode = malloc(sizeof(struct Node));
    if (newNode == NULL) {
        fprintf(stderr, "Błąd alokacji pamięci dla Node\n");
        return;
    }
    
    newNode->key = key;
    newNode->value = value;
    newNode->next = hm->buckets[index];
    hm->buckets[index] = newNode;
    hm->size++;
}

/**
 * Usuwa element o podanym kluczu
 */
void hashmap_remove(HashMap *hm, void *key) {
    if (hm == NULL || key == NULL) {
        return;
    }
    
    int index = hm->hash_func(key) % hm->capacity;
    
    struct Node *curr = hm->buckets[index];
    struct Node *prev = NULL;
    
    while (curr != NULL) {
        if (hm->compare_func(curr->key, key) == 0) {
            // Znaleziono węzeł do usunięcia
            if (prev == NULL) {
                // Usuwamy pierwszy węzeł w łańcuchu
                hm->buckets[index] = curr->next;
            } else {
                // Usuwamy węzeł ze środka/końca łańcucha
                prev->next = curr->next;
            }
            free(curr);
            hm->size--;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

/**
 * Pobiera wartość dla danego klucza
 */
void* hashmap_get(HashMap *hm, void *key) {
    if (hm == NULL || key == NULL) {
        return NULL;
    }

    int index = hm->hash_func(key) % hm->capacity;
    fprintf(stderr, "[hashmap_get] key=%s, index=%d, buckets[%d]=%p\n",
            (char*)key, index, index, (void*)hm->buckets[index]);
    fflush(stderr);
    struct Node *curr = hm->buckets[index];

    while (curr != NULL) {
        fprintf(stderr, "[hashmap_get] curr=%p, curr->key=%p\n", (void*)curr, curr->key);
        fflush(stderr);
        if (hm->compare_func(curr->key, key) == 0) {
            return curr->value;
        }
        curr = curr->next;
    }

    return NULL;  // klucz nie znaleziony
}

/**
 * Sprawdza czy klucz istnieje w hashmapie
 */
bool hashmap_contains(HashMap *hm, void *key) {
    if (hm == NULL || key == NULL) {
        return false;
    }

    return hashmap_get(hm, key) != NULL;
}

/**
 * Zwraca aktualną liczbę elementów w hashmapie
 */
int hashmap_size(HashMap *hm) {
    if (hm == NULL) {
        return 0;
    }
    
    return hm->size;
}

/**
 * Sprawdza czy hashmapa jest pusta
 */
bool hashmap_is_empty(HashMap *hm) {
    if (hm == NULL) {
        return true;  // NULL jest "pusty"
    }

    return hm->size == 0;
}

/**
 * Czyści całą hashmapę (usuwa wszystkie elementy)
 */
void hashmap_clear(HashMap *hm) {
    if (hm == NULL) {
        return;
    }
    
    // Zwolnij wszystkie węzły w każdym buckecie
    for (int i = 0; i < hm->capacity; i++) {
        struct Node *curr = hm->buckets[i];
        while (curr != NULL) {
            struct Node *next = curr->next;
            free(curr);
            curr = next;
        }
        hm->buckets[i] = NULL;  // wyczyść bucket
    }
    
    hm->size = 0;
}

/**
 * Zwalnia pamięć zajmowaną przez hashmapę
 */
void hashmap_free(HashMap *hm) {
    if (hm == NULL) {
        return;
    }

    // Najpierw wyczyść wszystkie elementy
    hashmap_clear(hm);
    
    // Zwolnij tablicę bucketów
    free(hm->buckets);
    
    // Zwolnij strukturę hashmapy
    free(hm);
}

/**
 * Wyświetla zawartość hashmapy (debug)
 */
void hashmap_print(HashMap *hm) {
    if (hm == NULL) {
        printf("HashMap jest NULL\n");
        return;
    }
    
    printf("=== HashMap Debug ===\n");
    printf("Size: %d, Capacity: %d, Load Factor: %.2f\n", 
           hm->size, hm->capacity, (double)hm->size / hm->capacity);
    
    for (int i = 0; i < hm->capacity; i++) {
        if (hm->buckets[i] != NULL) {
            printf("Bucket[%d]: ", i);
            struct Node *curr = hm->buckets[i];
            int count = 0;
            while (curr != NULL) {
                count++;
                curr = curr->next;
            }
            printf("%d elementów\n", count);
        }
    }
    printf("====================\n");
}

/**
 * Wyświetla statystyki hashmapy
 */
void hashmap_print_stats(HashMap *hm) {
    if (hm == NULL) {
        printf("HashMap jest NULL\n");
        return;
    }
    
    int used_buckets = 0;
    int max_chain_length = 0;
    int total_chain_length = 0;
    
    for (int i = 0; i < hm->capacity; i++) {
        if (hm->buckets[i] != NULL) {
            used_buckets++;
            int chain_length = 0;
            struct Node *curr = hm->buckets[i];
            while (curr != NULL) {
                chain_length++;
                curr = curr->next;
            }
            total_chain_length += chain_length;
            if (chain_length > max_chain_length) {
                max_chain_length = chain_length;
            }
        }
    }
    
    printf("=== HashMap Statistics ===\n");
    printf("Elementy: %d\n", hm->size);
    printf("Pojemność: %d\n", hm->capacity);
    printf("Load Factor: %.2f%%\n", (double)hm->size / hm->capacity * 100);
    printf("Użyte buckety: %d / %d (%.2f%%)\n", 
           used_buckets, hm->capacity, 
           (double)used_buckets / hm->capacity * 100);
    printf("Najdłuższy łańcuch: %d\n", max_chain_length);
    printf("Średnia długość łańcucha: %.2f\n", 
           used_buckets > 0 ? (double)total_chain_length / used_buckets : 0.0);
    printf("==========================\n");
}

// ===== FUNKCJE HASHUJĄCE DLA STRINGÓW =====

/**
 * Funkcja hashująca dla stringów (algorytm djb2 variant)
 */
int hash_string(void *key) {
    char *str = (char*)key;
    unsigned int hash = 0;
    while (*str) {
        hash = hash*31+(*str);
        str++;
    }
    // Ensure positive value by masking sign bit
    return (int)(hash & 0x7FFFFFFF);
}

/**
 * Funkcja porównująca dla stringów
 */
int compare_string(void *key1, void *key2) {
    return strcmp((char*)key1, (char*)key2);
}