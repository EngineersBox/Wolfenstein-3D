#pragma once

#include <string>
#include <type_traits>
#include <vector>

#include "../exceptions/hashing/BucketIndexOccupied.h"
#include "../exceptions/hashing/HashTableCapacity.h"
#include "../exceptions/hashing/NullClassWithInheritance.h"

using namespace std;

#define HASH_TABLE_MAX_SIZE 1024
#define HASH_TABLE_DEFAULT_SIZE 16

#define PRIME_MOD 1e9 - 9
#define PRIME_BASE 113

template <typename V>
class HMEntry {
    public:
        HMEntry(const string& key, V* value);
        ~HMEntry();

        const string key;
        V* value;
        HMEntry *next;
};

template <typename V>
HMEntry<V>::HMEntry(const string& key, V* value):
    key(key),
    value(value),
    next(NULL)
{};

template<typename V>
HMEntry<V>::~HMEntry(){};

template <typename V>
class HashTable {
   public:
        HashTable();
        explicit HashTable(int size);
        ~HashTable();

        V* get(const string& key);
        void insert(const string& key, V* value);
        void remove(const string& key);

        size_t size() noexcept;
   private:
        size_t element_count;
        int table_size;
        HMEntry<V> **entries;

        inline void findNextNonNull(HMEntry<V>* prev, HMEntry<V>* entry, const string& key);
        unsigned long hashFunc(const string& key) const;
};

template <typename V>
HashTable<V>::HashTable(int size) {
    if (size > HASH_TABLE_MAX_SIZE) {
        throw HashTableCapacity(HASH_TABLE_MAX_SIZE, "HashTable size cannot exceed max size limit.");
    }
    this->table_size = size;
    this->entries = new HMEntry<V>*[size]();
};

template <typename V>
HashTable<V>::HashTable() : HashTable<V>(HASH_TABLE_DEFAULT_SIZE){};

template <typename V>
HashTable<V>::~HashTable() {
    for (int i = table_size - 1; i != -1; i--) {
        HMEntry<V>* entry = entries[i];
        while (entry != NULL) {
            HMEntry<V>* prev = entry;
            entry = entry->next;
            delete prev;
        }
        entries[i] = NULL;
    }
    delete[] entries;
};

template <typename V>
V* HashTable<V>::get(const string& key) {
    unsigned long hashValue = hashFunc(key);
    HMEntry<V>* entry = entries[hashValue];

    while (entry != NULL) {
        if (entry->key == key) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
};

template <typename V>
inline void HashTable<V>::findNextNonNull(HMEntry<V>* prev, HMEntry<V>* entry, const string& key) {
    while (entry != NULL && entry->key != key) {
        prev = entry;
        entry = entry->next;
    }
};

template <typename V>
void HashTable<V>::insert(const string& key, V* value) {
    unsigned long hashValue = hashFunc(key);
    HMEntry<V>* prev = NULL;
    HMEntry<V>* entry = entries[hashValue];

    findNextNonNull(prev, entry, key);

    if (entry != NULL) {
        entry->value = value;
        this->element_count++;
        return;
    }

    entry = new HMEntry<V>(key, value);
    if (prev == NULL) {
        entries[hashValue] = entry;
    } else {
        prev->next = entry;
    }
    this->element_count++;
};

template <typename V>
void HashTable<V>::remove(const string& key) {
    unsigned long hashValue = hashFunc(key);
    HMEntry<V>* prev = NULL;
    HMEntry<V>* entry = entries[hashValue];

    findNextNonNull(prev, entry, key);

    if (entry == NULL) {
        return;
    }
    if (prev == NULL) {
        entries[hashValue] = entry->next;
    } else {
        prev->next = entry->next;
    }
    delete entry;
    this->element_count--;
};

template <typename V>
size_t HashTable<V>::size() noexcept {
    return this->element_count;
};

template <typename V>
unsigned long HashTable<V>::hashFunc(const string& key) const {
    // Hashing using rolling polynomial method
    unsigned long hashVal = 0;
    for (int i = key.length() - 1; i != -1; i--) {
        hashVal = (hashVal * PRIME_MOD) + key[i];
        hashVal %= table_size;
    }
    return hashVal;
};