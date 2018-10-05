#pragma once

#include "CheckResult.hpp"
#include "Guideline.hpp"

#include <string>
#include <tuple>
#include <vector>

namespace clang {
    class FunctionDecl;
}

class Options;

using Guidelines = std::vector<Guideline>;
using FailedChecks = std::vector<CheckResult>;

class Heuristic
{
public:
    Heuristic(const std::string& id, const std::string& description, Guidelines guidelines)
        : m_id(id)
        , m_description(description)
        , m_guidelines(guidelines)
    {}

    const std::string& id() const
    {
        return m_id;
    }

    const std::string& description() const
    {
        return m_description;
    }

    FailedChecks check(const clang::FunctionDecl& functionDecl, const Options& options) const
    {
        FailedChecks failedChecks;

        for (const auto& guideline : m_guidelines) {
            const auto result = guideline.check(functionDecl, options);

            if (result.failure()) {
                failedChecks.push_back(result);
            }
        }

        return failedChecks;
    }

private:
    std::string m_id;
    std::string m_description;
    Guidelines m_guidelines;
};
