#pragma once

#include "Heuristic.hpp"
#include "Options.hpp"

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


class HeuristicsCheckAction : public clang::ast_matchers::MatchFinder::MatchCallback
{
  public:
    HeuristicsCheckAction(Options options);
    ~HeuristicsCheckAction();

    auto matcher() const
    {
        using namespace clang::ast_matchers;

        return functionDecl().bind("func_decl");
    }

    virtual void run(const clang::ast_matchers::MatchFinder::MatchResult& result) override;

private:
    std::vector<Heuristic> m_heuristics;
    Options m_options;

    std::unique_ptr<std::ofstream> m_file;
    bool m_isFirstRecord = true;
};
