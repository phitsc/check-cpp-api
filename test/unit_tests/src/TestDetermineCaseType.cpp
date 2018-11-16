#include "DetermineCaseType.hpp"

#include "../catch2/catch.hpp"


TEST_CASE("isCamelCase")
{
    REQUIRE(isCamelCase("helloWorld"));
    REQUIRE(isCamelCase("thisIsAcamelCaseFunction"));
    REQUIRE(isCamelCase("aFunctionThatLooksLikeHungarian"));

    REQUIRE(!isCamelCase("ThisIsPascalCase"));
    REQUIRE(!isCamelCase("this_is_snake_case"));
    REQUIRE(!isCamelCase("thisisalllowercase"));
    REQUIRE(!isCamelCase("THISISALLUPPERCASE"));
}


TEST_CASE("isPascalCase")
{
    REQUIRE(isPascalCase("HelloWorld"));
    REQUIRE(isPascalCase("ThisIsApascalCaseFunction"));

    REQUIRE(!isPascalCase("thisIsCamelCase"));
    REQUIRE(!isPascalCase("Thisisalsonotpascalcase"));
    REQUIRE(!isPascalCase("this_is_snake_case"));
    REQUIRE(!isPascalCase("thisisalllowercase"));
    REQUIRE(!isPascalCase("THISISALLUPPERCASE"));
}


TEST_CASE("isSnakeCase")
{
    REQUIRE(isSnakeCase("a_b"));
    REQUIRE(isSnakeCase("hello_world"));
    REQUIRE(isSnakeCase("Hello_World"));
    REQUIRE(isSnakeCase("can_be_more_than_two_words"));
    REQUIRE(isSnakeCase("_this_is_camel_case"));
    REQUIRE(isSnakeCase("this_is_camel_case_"));
    REQUIRE(isSnakeCase("_this_is_camel_case_"));

    REQUIRE(!isSnakeCase("_"));
    REQUIRE(!isSnakeCase("__"));
    REQUIRE(!isSnakeCase("___"));
    REQUIRE(!isSnakeCase("_a_"));
    REQUIRE(!isSnakeCase("thisIsCamelCase"));
    REQUIRE(!isSnakeCase("ThisIsPascalCase"));
    REQUIRE(!isSnakeCase("thisisalllowercase"));
    REQUIRE(!isSnakeCase("THISISALLUPPERCASE"));
    REQUIRE(!isSnakeCase("_thisIsNotCamelCase"));
    REQUIRE(!isSnakeCase("thisIsNotCamelCase_"));
}


TEST_CASE("isHungarian")
{
    REQUIRE(isHungarian("szText"));
    REQUIRE(isHungarian("pszAnotherText"));
    REQUIRE(isHungarian("nWordCount"));
    REQUIRE(isHungarian("bIsUpper"));
    REQUIRE(isHungarian("iCalcRect"));

    REQUIRE(!isHungarian("thisIsCamelCase"));
    REQUIRE(!isHungarian("ThisIsPascalCase"));
    REQUIRE(!isHungarian("thisisalllowercase"));
    REQUIRE(!isHungarian("THISISALLUPPERCASE"));
    REQUIRE(!isHungarian("_thisIsNotCamelCase"));
    REQUIRE(!isHungarian("isUpper"));
    REQUIRE(!isHungarian("notHungarian"));
    REQUIRE(!isHungarian("doneTesting"));
}


TEST_CASE("determineCaseType")
{
    REQUIRE(determineCaseType("camelCase") == CaseType::Camel);
    REQUIRE(determineCaseType("PascalCase") == CaseType::Pascal);
    REQUIRE(determineCaseType("snake_case") == CaseType::Snake);
    REQUIRE(determineCaseType("pszHungarian") == CaseType::Hungarian);
    REQUIRE(determineCaseType("n") == CaseType::Unknown);
}