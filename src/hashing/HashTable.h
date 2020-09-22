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

        const string& getKey() const;
        V* getValue() const;
        HMEntry *getNext() const;

        void setValue(V* value);
        void setNext(HMEntry *next);
    private:
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
const string& HMEntry<V>::getKey() const {
    return this->key;
};

template<typename V>
V* HMEntry<V>::getValue() const {
    return this->value;
};

template<typename V>
void HMEntry<V>::setValue(V* value) {
    this->value = value;
};

template<typename V>
HMEntry<V>* HMEntry<V>::getNext() const {
    return this->next;
};

template<typename V>
void HMEntry<V>::setNext(HMEntry<V>* next) {
    this->next = next;
};

template <typename V>
class HashTable {
   public:
        HashTable();
        HashTable(int size);
        ~HashTable();

        V* get(const string& key);
        void insert(const string& key, V* value);
        void remove(const string& key);

        size_t size();
   private:
        size_t element_count;
        int table_size = HASH_TABLE_DEFAULT_SIZE;
        HMEntry<V> **entries;

        inline void findNextNonNull(HMEntry<V>* prev, HMEntry<V>* entry, const string& key);
        unsigned long hashFunc(const string& key);
};

template <typename V>
HashTable<V>::HashTable() {
    this->entries = new HMEntry<V>*[table_size]();
}

template <typename V>
HashTable<V>::HashTable(int size) {
    this->table_size = size;
    this->entries = new HMEntry<V>*[size]();
}

template <typename V>
HashTable<V>::~HashTable() {
    for (int i = table_size - 1; i != -1; i--) {
        HMEntry<V>* entry = entries[i];
        while (entry != NULL) {
            HMEntry<V>* prev = entry;
            entry = entry->getNext();
            delete prev;
        }
        entries[i] = NULL;
    }
    delete[] entries;
}

template <typename V>
V* HashTable<V>::get(const string& key) {
    unsigned long hashValue = hashFunc(key);
    HMEntry<V>* entry = entries[hashValue];

    while (entry != NULL) {
        if (entry->getKey().c_str() == key) {
            return entry->getValue();
        }
        entry = entry->getNext();
    }
    return nullptr;
}

template <typename V>
inline void HashTable<V>::findNextNonNull(HMEntry<V>* prev, HMEntry<V>* entry, const string& key) {
    while (entry != NULL && entry->getKey() != key) {
        prev = entry;
        entry = entry->getNext();
    }
}

template <typename V>
void HashTable<V>::insert(const string& key, V* value) {
    unsigned long hashValue = hashFunc(key);
    HMEntry<V>* prev = NULL, entry = entries[hashValue];

    findNextNonNull(prev, entry, key);

    if (entry != NULL) {
        entry->setValue(value);
        this->element_count++;
        return;
    }

    entry = new HMEntry<V>(key, value);
    if (prev == NULL) {
        entries[hashValue] = entry;
    } else {
        prev->setNext(entry);
    }
    this->element_count++;
}

template <typename V>
void HashTable<V>::remove(const string& key) {
    unsigned long hashValue = hashFunc(key);
    HMEntry<V>* prev = NULL, entry = entries[hashValue];

    findNextNonNull(prev, entry, key);

    if (entry == NULL) {
        return;
    }
    if (prev == NULL) {
        entries[hashValue] = entry->getNext();
    } else {
        prev->setNext(entry->getNext());
    }
    delete entry;
    this->element_count--;
}

template <typename V>
size_t HashTable<V>::size() {
    return this->element_count;
};

template <typename V>
unsigned long HashTable<V>::hashFunc(const string& key) {
    unsigned long hashVal = 0;
    for (int i = key.length() - 1; i != -1; i--) {
        hashVal = (hashVal * PRIME_MOD) + key[i];
        hashVal %= table_size;
    }
    return hashVal;
};