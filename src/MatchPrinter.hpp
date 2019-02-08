#pragma once

#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang;
using namespace clang::ast_matchers;

//! A MatchFinder callback that prints every match to stdout
class MatchPrinter : public MatchFinder::MatchCallback
{
  public:
    //! \private
    virtual void run(const MatchFinder::MatchResult& result) override
    {
        for (const auto& node : result.Nodes.getMap()) {
            std::string buffer;
            llvm::raw_string_ostream os(buffer);
            node.second.print(os, clang::PrintingPolicy(clang::LangOptions()));
            llvm::outs() << node.first << ": " << os.str() << "\n";
        }
    }
};
