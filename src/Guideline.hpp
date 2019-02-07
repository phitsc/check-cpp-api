//! \file Guideline.hpp
#pragma once

#include "CheckResult.hpp"

#include <functional>
#include <string>

namespace clang {
    class FunctionDecl;
}

class Options;

//! A function that checks guidelines
using GuidelineCheckFunction = std::function<CheckResult(const clang::FunctionDecl&, const Options&)>;

/*! A API usability guideline

    A guideline's check() function calls the GuidelineCheckFunction
    with which this Guideline object was initialized.
*/
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

    //! Returns the guideline's identifier
    int id() const
    {
        return m_id;
    }

    //! Check's the guideline by calling the object's GuidelineCheckFunction
    CheckResult check(const clang::FunctionDecl& functionDecl, const Options& options) const
    {
        return CheckResult(m_check(functionDecl, options), m_id);
    }

private:
    int m_id;
    std::string m_description;
    GuidelineCheckFunction m_check;
};
