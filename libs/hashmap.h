#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>

// Struktura węzła w łańcuchu (lista jednokierunkowa)
struct Node {
    void *key;
    void *value;
    struct Node *next;
};

// Struktura HashMap
typedef struct HashMap {
    struct Node **buckets;           // tablica wskaźników na węzły
    int size;                        // liczba elementów w hashmapie
    int capacity;                    // pojemność (liczba bucketów)
    int (*hash_func)(void *key);     // wskaźnik na funkcję hashującą
    int (*compare_func)(void *key1, void *key2);  // wskaźnik na funkcję porównującą
} HashMap;

/**
 * Tworzy nową hashmapę
 *
 * @param hash_func - funkcja hashująca, zwraca int dla danego klucza
 * @param compare_func - funkcja porównująca dwa klucze, zwraca:
 *                       0 jeśli równe, <0 jeśli key1<key2, >0 jeśli key1>key2
 * @return wskaźnik na nową hashmapę
 */
HashMap* hashmap_create(int (*hash_func)(void*), int (*compare_func)(void*, void*));

/**
 * Wstawia parę klucz-wartość do hashmapy
 * Jeśli klucz już istnieje, aktualizuje wartość
 *
 * @param hm - wskaźnik na hashmapę
 * @param key - wskaźnik na klucz
 * @param value - wskaźnik na wartość
 */
void hashmap_insert(HashMap *hm, void *key, void *value);

/**
 * Usuwa element o podanym kluczu z hashmapy
 *
 * @param hm - wskaźnik na hashmapę
 * @param key - wskaźnik na klucz do usunięcia
 */
void hashmap_remove(HashMap *hm, void *key);

/**
 * Pobiera wartość dla danego klucza
 *
 * @param hm - wskaźnik na hashmapę
 * @param key - wskaźnik na klucz
 * @return wskaźnik na wartość lub NULL jeśli klucz nie istnieje
 */
void* hashmap_get(HashMap *hm, void *key);

/**
 * Powiększa pojemność hashmapy i przenosi wszystkie elementy
 * (funkcja pomocnicza, wywoływana automatycznie przez hashmap_insert)
 *
 * @param hm - wskaźnik na hashmapę
 */
void hashmap_rehash(HashMap *hm);

/**
 * Zwalnia pamięć zajmowaną przez hashmapę
 * UWAGA: nie zwalnia pamięci kluczy i wartości - to odpowiedzialność użytkownika!
 *
 * @param hm - wskaźnik na hashmapę do zwolnienia
 */
void hashmap_free(HashMap *hm);

// ===== FUNKCJE POMOCNICZE =====

/**
 * Sprawdza czy klucz istnieje w hashmapie
 * @return true jeśli klucz istnieje, false w przeciwnym przypadku
 */
bool hashmap_contains(HashMap *hm, void *key);

/**
 * Zwraca aktualną liczbę elementów w hashmapie
 */
int hashmap_size(HashMap *hm);

/**
 * Sprawdza czy hashmapa jest pusta
 * @return true jeśli pusta, false w przeciwnym przypadku
 */
bool hashmap_is_empty(HashMap *hm);

/**
 * Czyści całą hashmapę (usuwa wszystkie elementy)
 */
void hashmap_clear(HashMap *hm);

// ===== FUNKCJE DEBUG =====

/**
 * Wyświetla zawartość hashmapy (debug)
 */
void hashmap_print(HashMap *hm);

/**
 * Wyświetla statystyki hashmapy (debug)
 */
void hashmap_print_stats(HashMap *hm);

// ===== FUNKCJE HASHUJĄCE DLA STRINGÓW =====

/**
 * Funkcja hashująca dla kluczy typu string
 */
int hash_string(void *key);

/**
 * Funkcja porównująca dla kluczy typu string
 * @return 0 jeśli równe, <0 jeśli key1<key2, >0 jeśli key1>key2
 */
int compare_string(void *key1, void *key2);

#endif // HASHMAP_H