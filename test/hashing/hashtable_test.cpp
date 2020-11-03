#pragma once

#include <string>

#include "../../src/logic/hashing/HashTable.hpp"
#include "../../src/exceptions/hashing/HashTableCapacity.hpp"
#include "../framework/catch.hpp"

using namespace std;

struct TestPointToObject {
    float float_value;
    bool operator==(const TestPointToObject& other) const {
        return this->float_value == other.float_value;
    }
};

struct TestHashableObject {
    int int_value;
    string string_value;
    TestPointToObject* test_point_obj;
    bool operator==(const TestHashableObject& other) const {
        return (this->int_value == other.int_value
            &&  this->string_value == other.string_value
            &&  this->test_point_obj == other.test_point_obj);
    }
};

HashTable<TestHashableObject> testTable;

TestHashableObject obj1{46, "string value 1", new TestPointToObject{6.34232}};
string obj1_key = "some object name";
TestHashableObject obj2{642, "string value 2", new TestPointToObject{3.8453}};
string obj2_key = "another key name";
TestHashableObject obj3{343, "string value 3", new TestPointToObject{2.69463}};
string obj3_key = "the final key string";

TEST_CASE("4.1: Create new hash table of size", "[multi-file:4]") {
    SECTION("4.1.1: Table of invalid size: " + to_string(HASH_TABLE_MAX_SIZE + 1)) {
        REQUIRE_THROWS_AS(HashTable<TestHashableObject>(HASH_TABLE_MAX_SIZE + 1), HashTableCapacity);
    }
    SECTION("4.1.2: Table of valid size: " + to_string(HASH_TABLE_DEFAULT_SIZE)) {
        REQUIRE_NOTHROW(testTable = HashTable<TestHashableObject>());
        REQUIRE(testTable.size() == 0);
    }
}

TEST_CASE("4.2: Insert objects into table", "[multi-file:4]") {
    TestHashableObject testobj = TestHashableObject{0, "", nullptr};
    SECTION("4.2.1: Insert object 1") {
        testobj = TestHashableObject{0,"",nullptr};
        REQUIRE_THROWS_AS(testTable.get(obj1_key, testobj), HashTableCapacity);
        REQUIRE(testobj.test_point_obj == nullptr);
        REQUIRE_NOTHROW(testTable.insert(obj1_key, obj1));
        REQUIRE(testTable.size() == 1);
    }
    SECTION("4.2.2: Insert object 2") {
        testobj = TestHashableObject{0,"",nullptr};
        REQUIRE_NOTHROW(testTable.get(obj2_key, testobj));
        REQUIRE(testobj.test_point_obj == nullptr);
        REQUIRE_NOTHROW(testTable.insert(obj2_key, obj2));
        REQUIRE(testTable.size() == 2);
    }
    SECTION("4.2.3: Insert object 3") {
        testobj = TestHashableObject{0,"",nullptr};
        REQUIRE_NOTHROW(testTable.get(obj3_key, testobj));
        REQUIRE(testobj.test_point_obj == nullptr);
        REQUIRE_NOTHROW(testTable.insert(obj3_key, obj3));
        REQUIRE(testTable.size() == 3);
    }
}

TEST_CASE("4.3: Get object and ensure they have correct structure", "[multi-file:4]") {
    TestHashableObject testobj;
    SECTION("4.3.1: Get object 1") {
        testobj = TestHashableObject{0, "", nullptr};
        REQUIRE_NOTHROW(testTable.get(obj1_key, testobj));
        REQUIRE(obj1 == testobj);
        REQUIRE(testTable.size() == 3);
    }
    SECTION("4.3.2: Get object 2") {
        testobj = TestHashableObject{0, "", nullptr};
        REQUIRE_NOTHROW(testTable.get(obj2_key, testobj));
        REQUIRE(obj2 == testobj);
        REQUIRE(testTable.size() == 3);
    }
    SECTION("4.3.3: Get object 3") {
        testobj = TestHashableObject{0, "", nullptr};
        REQUIRE_NOTHROW(testTable.get(obj3_key, testobj));
        REQUIRE(obj3 == testobj);
        REQUIRE(testTable.size() == 3);
    }
}

TEST_CASE("4.4: Remove objects from table", "[multi-file:4]") {
    TestHashableObject testobj = TestHashableObject{0, "", nullptr};
    SECTION("4.4.1: Remove object 1") {
        testobj = TestHashableObject{0, "", nullptr};
        REQUIRE_NOTHROW(testTable.remove(obj1_key));
        REQUIRE_NOTHROW(testTable.get(obj1_key, testobj));
        REQUIRE(testobj.int_value == 0);
        REQUIRE(testobj.string_value == "");
        REQUIRE(testobj.test_point_obj == nullptr);
        REQUIRE(testTable.size() == 2);
    }
    SECTION("4.4.2: Remove object 2") {
        testobj = TestHashableObject{0, "", nullptr};
        REQUIRE_NOTHROW(testTable.remove(obj2_key));
        REQUIRE_NOTHROW(testTable.get(obj2_key, testobj));
        REQUIRE(testobj.int_value == 0);
        REQUIRE(testobj.string_value == "");
        REQUIRE(testobj.test_point_obj == nullptr);
        REQUIRE(testTable.size() == 1);
    }
    SECTION("4.4.3: Remove object 3") {
        testobj = TestHashableObject{0, "", nullptr};
        REQUIRE_NOTHROW(testTable.remove(obj3_key));
        REQUIRE_THROWS_AS(testTable.get(obj3_key, testobj), HashTableCapacity);
        REQUIRE(testobj.int_value == 0);
        REQUIRE(testobj.string_value == "");
        REQUIRE(testobj.test_point_obj == nullptr);
        REQUIRE(testTable.size() == 0);
    }
}