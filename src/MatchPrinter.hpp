#pragma once

#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang;
using namespace clang::ast_matchers;

class MatchPrinter : public MatchFinder::MatchCallback
{
  public:
    virtual void run(const MatchFinder::MatchResult& result) override
    {
        for (const auto& node : result.Nodes.getMap()) {
            std::string buffer;
            llvm::raw_string_ostream os(buffer);
            node.second.print(os, PrintingPolicy(LangOptions()));
            llvm::outs() << node.first << ": " << os.str() << "\n";
        }
    }
};
