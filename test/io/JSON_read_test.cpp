#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../framework/catch.hpp"
#include "../../src/io/resource_management/JSONParser.hpp"

#define DIR_PREFIX string("test/resources/")
#define VALID_JSON DIR_PREFIX + string("valid_test.json")
#define INVALID_JSON DIR_PREFIX + string("invalid_test.json")
#define NON_EXISTANT_JSON DIR_PREFIX + string("non_existant_test.json") 

using namespace std;

RSJresource validJsonRes;
RSJresource invalidJsonRes;

void getResource(const string& filename, RSJresource& res) {
    filebuf fileBuffer;
    if (fileBuffer.open(filename.c_str(), ios::in)) {
        istream is(&fileBuffer);
        res = RSJresource(is);
        fileBuffer.close();
    } else {
        throw runtime_error("Unable to read JSON resource");
    }
}

TEST_CASE("2.1: Can load valid JSON file successfully", "[multi-file:2]") {
    REQUIRE_NOTHROW(getResource(VALID_JSON, validJsonRes));
    REQUIRE(validJsonRes.exists() == true);
    REQUIRE(validJsonRes.is_parsed() == false);
}

TEST_CASE("2.2: Fails to parse invalid JSON file", "[multi-file:2]") {
    getResource(INVALID_JSON, invalidJsonRes);
    REQUIRE(invalidJsonRes.exists() == true);
    REQUIRE(invalidJsonRes.is_parsed() == false);
}

TEST_CASE("2.3: Fails to load non-existant JSON file", "[multi-file:2]") {
    REQUIRE_THROWS_AS(
        getResource(NON_EXISTANT_JSON, invalidJsonRes),
        runtime_error
    );
}

TEST_CASE("2.3: Can read base types from JSON resource", "[multi-file:2]") {
    REQUIRE(validJsonRes["some_string"].as<string>() == "string value");
    REQUIRE(validJsonRes["some_int"].as<int>() == 13531);
    REQUIRE(validJsonRes["some_float"].as<double>() == 5.35235);
    REQUIRE(validJsonRes["some_boolean"].as<bool>() == true);
}

TEST_CASE("2.4: Can read object types", "[multi-file:2]") {
    RSJobject rsjTestObj = validJsonRes["some_object"].as_object();
    SECTION("2.4.1 Object key count is correct") {
        REQUIRE(rsjTestObj.size() == 4);
    }
    SECTION("2.4.2 Object field values are correct") {
        REQUIRE(rsjTestObj["some_string"].as<string>() == "another string value");
        REQUIRE(rsjTestObj["some_int"].as<int>() == 634285);
        REQUIRE(rsjTestObj["some_float"].as<double>() == 3.14159);
        REQUIRE(rsjTestObj["some_boolean"].as<bool>() == false);
    }
}

TEST_CASE("2.5: Can read array types", "[multi-file:2]") {
    RSJarray rsjTestArray = validJsonRes["some_array"].as_array();
    SECTION("2.5.1 Array size is correct") {
        REQUIRE(rsjTestArray.size() == 5);
    }
    SECTION("2.5.2 Array base type fields are correct") {
        REQUIRE(rsjTestArray[0].as<string>() == "more strings");
        REQUIRE(rsjTestArray[1].as<int>() == 8953134);
        REQUIRE(rsjTestArray[2].as<double>() == 8.595736);
        REQUIRE(rsjTestArray[3].as<bool>() == true);
    }
    RSJobject rsjTestArrObj = rsjTestArray[4].as_object();
    SECTION("2.5.3 Object in array has correct key count") {
        REQUIRE(rsjTestArrObj.size() == 4);
    }
    SECTION("2.5.4 Object in array has correct field values") {
        REQUIRE(rsjTestArrObj["some_string"].as<string>() == "yet more string values");
        REQUIRE(rsjTestArrObj["some_int"].as<int>() == 4895837);
        REQUIRE(rsjTestArrObj["some_float"].as<double>() == 2.69382);
        REQUIRE(rsjTestArrObj["some_boolean"].as<bool>() == false);
    }
}