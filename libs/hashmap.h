#ifndef HASHMAP_H
#define HASHMAP_H

// Struktura węzła w łańcuchu (lista jednokierunkowa)
struct Node {
    void *key;
    void *value;
    struct Node *next;
};

// Struktura HashMap
struct HashMap {
    struct Node **buckets;           // tablica wskaźników na węzły
    int size;                        // liczba elementów w hashmapie
    int capacity;                    // pojemność (liczba bucketów)
    int (*hash_func)(void *key);     // wskaźnik na funkcję hashującą
    int (*compare_func)(void *key1, void *key2);  // wskaźnik na funkcję porównującą
};

/**
 * Tworzy nową hashmapę
 * 
 * @param hash_func - funkcja hashująca, zwraca int dla danego klucza
 * @param compare_func - funkcja porównująca dwa klucze, zwraca:
 *                       0 jeśli równe, <0 jeśli key1<key2, >0 jeśli key1>key2
 * @return wskaźnik na nową hashmapę
 */
struct HashMap* CreateHashMap(int (*hash_func)(void*), int (*compare_func)(void*, void*));

/**
 * Wstawia parę klucz-wartość do hashmapy
 * Jeśli klucz już istnieje, aktualizuje wartość
 * 
 * @param hm - wskaźnik na hashmapę
 * @param key - wskaźnik na klucz
 * @param value - wskaźnik na wartość
 */
void Insert(struct HashMap *hm, void *key, void *value);

/**
 * Usuwa element o podanym kluczu z hashmapy
 * 
 * @param hm - wskaźnik na hashmapę
 * @param key - wskaźnik na klucz do usunięcia
 */
void Remove(struct HashMap *hm, void *key);

/**
 * Pobiera wartość dla danego klucza
 * 
 * @param hm - wskaźnik na hashmapę
 * @param key - wskaźnik na klucz
 * @return wskaźnik na wartość lub NULL jeśli klucz nie istnieje
 */
void* Get(struct HashMap *hm, void *key);

/**
 * Powiększa pojemność hashmapy i przenosi wszystkie elementy
 * (funkcja pomocnicza, wywoływana automatycznie przez Insert)
 * 
 * @param hm - wskaźnik na hashmapę
 */
void Rehash(struct HashMap *hm);

/**
 * Zwalnia pamięć zajmowaną przez hashmapę
 * UWAGA: nie zwalnia pamięci kluczy i wartości - to odpowiedzialność użytkownika!
 * 
 * @param hm - wskaźnik na hashmapę do zwolnienia
 */
void FreeHashMap(struct HashMap *hm);

// ===== PRZYKŁADOWE FUNKCJE POMOCNICZE DLA POPULARNYCH TYPÓW =====

/**
 * Sprawdza czy klucz istnieje w hashmapie
 */
int Contains(struct HashMap *hm, void *key);

/**
 * Zwraca aktualną liczbę elementów w hashmapie
 */
int Size(struct HashMap *hm);

/**
 * Sprawdza czy hashmapa jest pusta
 */
int IsEmpty(struct HashMap *hm);

/**
 * Czyści całą hashmapę (usuwa wszystkie elementy)
 */
void Clear(struct HashMap *hm);

#endif // HASHMAP_H