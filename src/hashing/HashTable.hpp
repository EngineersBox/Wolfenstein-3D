#pragma once

#include <string>
#include <vector>

#include "../exceptions/hashing/BucketIndexOccupied.hpp"
#include "../exceptions/hashing/HashTableCapacity.hpp"
#include "../exceptions/hashing/InvalidKeySize.hpp"
#include "../raytracer/Globals.hpp"

using namespace std;

#define HASH_TABLE_MAX_SIZE 1024
#define HASH_TABLE_DEFAULT_SIZE 16

#define FNV_PRIME_MOD 22801763489 // Closest prime to 1e9 - 9
#define FNV_PRIME_32 16777619
#define FNV_OFFSET_32 2166136261

template <typename V>
class HMEntry {
    public:
        HMEntry(const string& key, V& value);
        ~HMEntry();

        string key;
        V value;
        HMEntry *next = nullptr;
};

template <typename V>
HMEntry<V>::HMEntry(const string& key, V& value){
    this->key = key;
    this->value = value;
    this->next = nullptr;
};

template<typename V>
HMEntry<V>::~HMEntry(){};

template <typename V>
class HashTable {
   public:
        explicit HashTable();
        explicit HashTable(int size);
        ~HashTable();

        bool get(const string& key, V& value);
        void insert(const string& key, V value);
        void remove(const string& key);

        size_t size() const noexcept;
        size_t hashFunc(const string& key) const;
   private:
        size_t element_count;
        int table_size;
        vector<HMEntry<V>*> buckets;

        inline void getSequentialNonNull(HMEntry<V>* prev, HMEntry<V>* entry, const string& key);
};

template <typename V>
HashTable<V>::HashTable(int size) {
    if (size > HASH_TABLE_MAX_SIZE) {
        throw HashTableCapacity(size);
    }
    this->table_size = size;
    this->buckets = vector<HMEntry<V>*>(size);
    this->element_count = 0;
    fill(this->buckets.begin(), this->buckets.end(), nullptr);
};

template <typename V>
HashTable<V>::HashTable() {
    this->table_size = HASH_TABLE_DEFAULT_SIZE;
    this->buckets = vector<HMEntry<V>*>(HASH_TABLE_DEFAULT_SIZE);
    this->element_count = 0;
    fill(this->buckets.begin(), this->buckets.end(), nullptr);
};

template <typename V>
HashTable<V>::~HashTable() {
    for (int i = table_size - 1; i != -1; i--) {
        HMEntry<V> *entry = buckets[i];
        while (entry != nullptr) {
            HMEntry<V> *current = entry;
            entry = entry->next;
            delete current;
        }
        buckets[i] = nullptr;
    }
};

template <typename V>
bool HashTable<V>::get(const string& key, V& value) {
    if (this->element_count == 0) {
        throw HashTableCapacity(this->element_count, "No elements in table. Size is: ");
    }
    size_t hashValue = hashFunc(key);
    HMEntry<V>* entry = buckets[hashValue];
    while (entry != nullptr) {
        if (entry->key == key) {
            value = entry->value;
            return true;
        }
        entry = entry->next;
    }
    return false;
};

template <typename V>
inline void HashTable<V>::getSequentialNonNull(HMEntry<V>* prev, HMEntry<V>* entry, const string& key) {
    while (entry != nullptr && entry->key != key) {
        prev = entry; 
        entry = entry->next;
    }
};

template <typename V>
void HashTable<V>::insert(const string& key, V value) {
    size_t hashValue = hashFunc(key);
    HMEntry<V>* prev = nullptr;
    HMEntry<V>* entry = buckets[hashValue];

    getSequentialNonNull(prev, entry, key);

    if (entry != nullptr) {
        entry->value = value;
        this->element_count++;
        return;
    }

    entry = new HMEntry<V>(key, value);
    if (prev == nullptr) {
        buckets[hashValue] = entry;
    } else {
        prev->next = entry;
    }
    this->element_count++;
};

template <typename V>
void HashTable<V>::remove(const string& key) {
    size_t hashValue = hashFunc(key);
    HMEntry<V>* prev = nullptr;
    HMEntry<V>* entry = buckets[hashValue];

    getSequentialNonNull(prev, entry, key);

    if (entry == nullptr) {
        return;
    }

    if (prev == nullptr) {
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
size_t HashTable<V>::hashFunc(const string& key) const {
    if (key.length() > 32) {
        throw InvalidKeySize(key);
    }
    size_t prime_power = 13;
    size_t hashVal = FNV_OFFSET_32;
    for (int i = key.length(); i != -1; i--) {
        hashVal = (hashVal + (key[i] ^ FNV_PRIME_32) * prime_power) % table_size;
        prime_power = (prime_power * FNV_PRIME_MOD) % table_size;
    }
    return hashVal;
};