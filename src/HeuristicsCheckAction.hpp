#pragma once

#include "Heuristic.hpp"
#include "Options.hpp"

#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang;
using namespace clang::ast_matchers;

#include <vector>

template<typename T>
std::string toString(const T* t)
{
    std::string buffer;
    llvm::raw_string_ostream os(buffer);
    t->print(os, PrintingPolicy(LangOptions()));
    return os.str();
}


class HeuristicsCheckAction : public MatchFinder::MatchCallback
{
  public:
    HeuristicsCheckAction(Options options);

    auto matcher() const
    {
        return functionDecl().bind("func_decl");
    }

    virtual void run(const MatchFinder::MatchResult& result) override;

private:
    std::vector<Heuristic> m_heuristics;
    Options m_options;
};
