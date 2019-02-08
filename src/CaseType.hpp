//! \file CaseType.h
#pragma once

#include <string>

//! \addtogroup free_functions Free functions
//! \{

//! \private
enum CaseTypeOption : int
{
    camel,
    hungarian,
    pascal,
    snake
};

//! A type representing case types used in
//! common programming naming conventions
enum class CaseType : int
{
    //! camelCase
    Camel     = camel,
    //! nHungarianNotation
    Hungarian = hungarian,
    //! PascalCase
    Pascal    = pascal,
    //! snake_case
    Snake     = snake,
    //! None of the other case types
    Unknown
};

//! Converts from int to CaseType
inline CaseType caseTypeFromInt(int value)
{
    switch (value) {
        case 0: return CaseType::Camel;
        case 1: return CaseType::Hungarian;
        case 2: return CaseType::Pascal;
        case 3: return CaseType::Snake;
        default: return CaseType::Unknown;
    }
}

//! Converts from CaseType to std::string
inline std::string toString(CaseType caseType)
{
    switch (caseType) {
        case CaseType::Camel: return "camel";
        case CaseType::Hungarian: return "hungarian";
        case CaseType::Pascal: return "pascal";
        case CaseType::Snake: return "snake";
        default: return "unknown";
    }
}

//! \}
