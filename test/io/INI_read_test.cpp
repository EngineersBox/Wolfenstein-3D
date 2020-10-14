#pragma once

#include <string>

#include "../../src/io/INIReader.hpp"
#include "../framework/catch.hpp"

#define DIR_PREFIX string("test/resources/")
#define VALID_INI DIR_PREFIX + string("valid_test.ini")
#define INVALID_INI DIR_PREFIX + string("invalid_test.ini")
#define NON_EXISTANT_INI DIR_PREFIX + string("non_existant_test.ini")

using namespace std;

INIReader validIniRes;
INIReader invalidIniRes;

TEST_CASE("3.1: Can load valid INI file successfully", "[multi-file:3]") {
    validIniRes = INIReader(VALID_INI);
    REQUIRE(validIniRes.ParseError() >= 0);
}

TEST_CASE("3.2: Fails to parse invalid INI file", "[multi-file:3]"){
    invalidIniRes = INIReader(INVALID_INI);
    REQUIRE(invalidIniRes.ParseError() == 0);
}

TEST_CASE("3.3: Fails to load non-existant INI file", "[multi-file:3]") {
    invalidIniRes = INIReader(NON_EXISTANT_INI);
    REQUIRE(invalidIniRes.ParseError() < 0);
}

TEST_CASE("3.4: Can read in sections", "[multi-file:3]") {
    SECTION("3.4.1 Ensure all sections are read in") {
        REQUIRE(validIniRes.Sections().size() == 2);
    }
    set<string> sections = validIniRes.Sections();
    SECTION("3.4.2 Ensure all sections exist") {
        REQUIRE(sections.find("section1") != sections.end());
        REQUIRE(sections.find("section2") != sections.end());
    }
}

TEST_CASE("3.5: Can read base types from INI resource", "[multi-file:3") {
    SECTION("3.5.1 Can read string and int types") {
        REQUIRE(validIniRes.Get("section1", "some_string", "DOES NOT EXIST") == "string value");
        REQUIRE(validIniRes.GetInteger("section1", "some_int", 0) == 352545);
    }
    SECTION("3.5.2: Can read float and boolean types") {
        REQUIRE(validIniRes.GetFloat("section2", "some_float", 0.0) == 3.3523f);
        REQUIRE(validIniRes.GetBoolean("section2", "some_boolean", false) == true);
    }
}