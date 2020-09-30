#pragma once

#include <string>
#include <vector>

#include "../exceptions/hashing/BucketIndexOccupied.h"
#include "../exceptions/hashing/HashTableCapacity.h"
#include "../exceptions/hashing/InvalidKeySize.h"
#include "../raytracer/Globals.h"

using namespace std;

#define HASH_TABLE_MAX_SIZE 1024
#define HASH_TABLE_DEFAULT_SIZE 16

#define PRIME_MOD 22801763489 // Closest prime to 1e9 - 9
#define PRIME_BASE 11

template <typename V>
class HMEntry {
    public:
        HMEntry(const string& key, V value);
        ~HMEntry();

        const string key;
        V value;
        HMEntry *next;
};

template <typename V>
HMEntry<V>::HMEntry(const string& key, V value):
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

        void get(const string& key, V& value);
        void insert(const string& key, V value);
        void remove(const string& key);

        size_t size() const noexcept;
   private:
        size_t element_count;
        int table_size;
        HMEntry<V> **buckets;

        inline void findNextNonNull(HMEntry<V>* prev, HMEntry<V>* entry, const string& key);
        unsigned long hashFunc(const string& key) const;
};

template <typename V>
HashTable<V>::HashTable(int size) {
    if (size > HASH_TABLE_MAX_SIZE) {
        throw HashTableCapacity(HASH_TABLE_MAX_SIZE, "HashTable size cannot exceed max size limit.");
    }
    this->table_size = size;
    this->buckets = new HMEntry<V>*[size]();
};

template <typename V>
HashTable<V>::HashTable() {
    this->table_size = HASH_TABLE_DEFAULT_SIZE;
    this->buckets = new HMEntry<V>*[HASH_TABLE_DEFAULT_SIZE]();
};

template <typename V>
HashTable<V>::~HashTable() {
    for (int i = table_size - 1; i != -1; i--) {
        HMEntry<V>* entry = buckets[i];
        while (entry != NULL) {
            HMEntry<V>* prev = entry;
            entry = entry->next;
            delete prev;
        }
        buckets[i] = NULL;
    }
    delete[] buckets;
};

template <typename V>
void HashTable<V>::get(const string& key, V& value) {
    unsigned long hashValue = hashFunc(key);
    HMEntry<V>* entry = buckets[hashValue];

    while (entry != NULL) {
        if (entry->key == key) {
            value = entry->value;
            return;
        }
        entry = entry->next;
    }
};

template <typename V>
inline void HashTable<V>::findNextNonNull(HMEntry<V>* prev, HMEntry<V>* entry, const string& key) {
    while (entry != NULL && entry->key != key) {
        prev = entry;
        entry = entry->next;
    }
};

template <typename V>
void HashTable<V>::insert(const string& key, V value) {
    unsigned long hashValue = hashFunc(key);
    HMEntry<V>* prev = NULL;
    HMEntry<V>* entry = buckets[hashValue];

    findNextNonNull(prev, entry, key);

    if (entry != NULL) {
        entry->value = value;
        this->element_count++;
        return;
    }

    entry = new HMEntry<V>(key, value);
    if (prev == NULL) {
        buckets[hashValue] = entry;
    } else {
        prev->next = entry;
    }
    this->element_count++;
};

template <typename V>
void HashTable<V>::remove(const string& key) {
    unsigned long hashValue = hashFunc(key);
    HMEntry<V>* prev = NULL;
    HMEntry<V>* entry = buckets[hashValue];

    findNextNonNull(prev, entry, key);

    if (entry == NULL) {
        return;
    }
    if (prev == NULL) {
        buckets[hashValue] = entry->next;
    } else {
        prev->next = entry->next;
    }
    delete entry;
    this->element_count--;
};

template <typename V>
size_t HashTable<V>::size() const noexcept {
    return this->element_count;
};

template <typename V>
unsigned long HashTable<V>::hashFunc(const string& key) const {
    if (key.length() > 32) {
        throw InvalidKeySize(key);
    }
    unsigned long hashVal = PRIME_BASE;
    for (int i = 31; i != -1; i--) {
        hashVal = (hashVal ^ PRIME_MOD) * key[i];
        hashVal %= table_size;
    }
    return hashVal;
};