#pragma once

#include "CaseType.hpp"

#include <boost/algorithm/string.hpp>

#include <array>
#include <string>


bool isCamelCase(const std::string& name)
{
    if (name.length() > 1) {
        if (islower(name[0])) {
            for (auto n = 1u; n < name.length(); ++n) {
                if (name[n] == '_') {
                    return false;
                } else if (isupper(name[n])) {
                    return true;
                }
            }
        }
    }

    return false;
}


bool isPascalCase(const std::string& name)
{
    auto hasLower = false;

    if (name.length() > 1) {
        if (isupper(name[0])) {
            for (auto n = 1u; n < name.length(); ++n) {
                if (name[n] == '_') {
                    return false;
                } else if (islower(name[n])) {
                    hasLower = true;
                } else if (isupper(name[n]) && hasLower) {
                    return true;
                }
            }
        }
    }

    return false;
}


bool isSnakeCase(const std::string& name)
{
    if (name.length() > 2) {
        const auto pos = name.find("_", 1); // ignore leading _, if there is one

        if (pos != std::string::npos && pos != 0 && pos != (name.length() - 1)) {
            return isalnum(name[pos-1]) && isalnum(name[pos+1]);
        }
    }

    return false;
}


constexpr const std::array<const char*, 14> HungarianPrefixes = { {
    "b", "ch", "d", "dw", "i", "l", "lpsz", "n", "p", "psz", "str", "sz", "u16", "u32"
} };

bool isHungarian(const std::string& name)
{
    for (const auto& prefix : HungarianPrefixes) {
        const auto len = strlen(prefix);
        if (name.length() > len &&
            boost::starts_with(name, prefix) &&
            isupper(name[len]))
        {
            return true;
        }
    }

    return false;
}




CaseType determineCaseType(const std::string name)
{
    if (isHungarian(name)) {
        return CaseType::Hungarian;
    } else if (isCamelCase(name)) {
        return CaseType::Camel;
    } else if(isPascalCase(name)) {
        return CaseType::Pascal;
    } else if(isSnakeCase(name)) {
        return CaseType::Snake;
    }

    return CaseType::Unknown;
}