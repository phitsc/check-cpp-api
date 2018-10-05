#pragma once

#include "CheckResult.hpp"

#include <functional>
#include <string>

namespace clang {
    class FunctionDecl;
}

class Options;

using GuidelineCheckFunction = std::function<CheckResult(const clang::FunctionDecl&, const Options&)>;

class Guideline
{
public:
    Guideline(
        int id,
        const std::string& description,
        GuidelineCheckFunction checkFn)
        : m_id(id)
        , m_description(description)
        , m_check(checkFn)
    {}

    int id() const
    {
        return m_id;
    }

    CheckResult check(const clang::FunctionDecl& functionDecl, const Options& options) const
    {
        return m_check(functionDecl, options);
    }

private:
    int m_id;
    std::string m_description;
    GuidelineCheckFunction m_check;
};
