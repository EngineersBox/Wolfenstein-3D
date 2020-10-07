#pragma once

#include <random>
#include <algorithm>
#include <vector>
#include <unordered_set>

#include "../../src/exceptions/hashing/InvalidKeySize.hpp"
#include "../../src/hashing/HashTable.hpp"
#include "../framework/catch.hh"

using namespace std;

random_device randomDevice;
mt19937 gen(randomDevice());

inline size_t getRandInRange(size_t min_value, size_t max_value_inc) {
    uniform_int_distribution<size_t> uniformDistribution(min_value, max_value_inc);
    return uniformDistribution(gen);
}

string random_string(size_t length) {
    auto randchar = []() -> char {
        const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        return charset[getRandInRange(0, sizeof(charset) - 1)];
    };
    std::string str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}

void generateRandStringArray(vector<string>& arr) {
    for (int i = arr.size() - 1; i != -1; i--) {
        arr[i] = random_string(32);
    }
}

HashTable<string> hashTestTable(HASH_TABLE_MAX_SIZE);

TEST_CASE("5.1: Can generate hash value", "[multi-file:5]") {
    size_t hashVal;
    REQUIRE_NOTHROW(hashVal = hashTestTable.hashFunc(random_string(15)));
    REQUIRE(hashVal >= 0);
    REQUIRE(hashVal < HASH_TABLE_MAX_SIZE);
}

TEST_CASE("5.2: Throws on invalid size keys", "[multi-file:5]") {
    size_t hashVal;
    REQUIRE_THROWS_AS(hashVal = hashTestTable.hashFunc(random_string(40)), InvalidKeySize);
}

TEST_CASE("5.3: Ensure hash collisions are minimal", "[multi-file:5]") {
    vector<string> testStrings(HASH_TABLE_MAX_SIZE);
    generateRandStringArray(testStrings);
    int collisions = 0;
    SECTION("5.3.2: All generated hashes are valid") {
        unordered_set<size_t> seen_before;
        seen_before.reserve(HASH_TABLE_MAX_SIZE);
        for (int i = testStrings.size() - 1; i != -1; i--) {
            size_t hashVal = hashTestTable.hashFunc(testStrings[i]);
            REQUIRE(hashVal >= 0);
            REQUIRE(hashVal < HASH_TABLE_MAX_SIZE);
            auto idx = seen_before.find(hashVal);
            if (idx == seen_before.end()) {
                seen_before.emplace_hint(idx, hashVal);
            } else {
                collisions++;
            }
        }
    }
    INFO("Number of collisions is: " + to_string(collisions));
    INFO("Collision percentage is: " + to_string((((float)collisions) / HASH_TABLE_MAX_SIZE) * 100));
}