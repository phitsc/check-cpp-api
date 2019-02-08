#pragma once

#include "Heuristic.hpp"
#include "Options.hpp"

#include "../nlohmann/json.hpp"

#include "clang/ASTMatchers/ASTMatchFinder.h"

#include <fstream>
#include <memory>
#include <vector>

template<typename T>
std::string toString(const T* t)
{
    std::string buffer;
    llvm::raw_string_ostream os(buffer);
    t->print(os, clang::PrintingPolicy(clang::LangOptions()));
    return os.str();
}

//! A MatchFinder callback for checking API usability heuristics
class HeuristicsCheckAction : public clang::ast_matchers::MatchFinder::MatchCallback
{
  public:
    //! Constructs a HeuristicsCheckAction object
    //! \param options An Options object holding the program arguments
    HeuristicsCheckAction(Options options);
    ~HeuristicsCheckAction();

    //! Returns the matcher declaration to be passed to
    //! MatchFinder::addMatcher()
    auto matcher() const
    {
        using namespace clang::ast_matchers;

        return functionDecl().bind("func_decl");
    }

    //! \private
    virtual void run(const clang::ast_matchers::MatchFinder::MatchResult& result) override;

private:
    std::vector<Heuristic> m_heuristics;
    Options m_options;
    nlohmann::json m_json;
    std::unique_ptr<std::ofstream> m_ofstream;
};
