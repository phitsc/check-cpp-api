//! \file Heuristic.hpp
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

//!  A collection of Guideline objects
using Guidelines = std::vector<Guideline>;

//! A collection of CheckResult objects
using FailedChecks = std::vector<CheckResult>;

/*! A API usability heuristic

    A Heuristic has a collection of \ref Guideline objects
*/
class Heuristic
{
public:
    //! Constructs a Heuristic object
    //! \param id The heuristic's identifier
    //! \param description The heuristic's description
    //! \param guidelines The collection of Guideline objects to store in this heuristic
    Heuristic(const std::string& id, const std::string& description, Guidelines guidelines)
        : m_id(id)
        , m_description(description)
        , m_guidelines(guidelines)
    {}

    //! Returns this heuristic's identifier
    const std::string& id() const
    {
        return m_id;
    }

    //! Returns this heuristic's description
    const std::string& description() const
    {
        return m_description;
    }

    //! Runs this heuristic's checks
    //! \returns A FailedChecks object containing information about the failed checks.
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
