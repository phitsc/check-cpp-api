#pragma once

#include <string>

enum CaseTypeOption : int
{
    camel,
    hungarian,
    pascal,
    snake
};

enum class CaseType : int
{
    Camel     = camel,
    Hungarian = hungarian,
    Pascal    = pascal,
    Snake     = snake,
    Unknown
};

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