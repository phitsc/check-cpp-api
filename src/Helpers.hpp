#pragma once

#include <clang/AST/Decl.h>
#include <clang/Basic/SourceLocation.h>

#include <string>


inline void append(std::string& dest, const std::string& src, const std::string& divider = ". ")
{
    if (!dest.empty()) {
        dest += divider;
    }

    dest += src;
}


inline clang::SourceLocation loc(const clang::FunctionDecl& functionDecl)
{
    return functionDecl.getSourceRange().getBegin();
}


inline std::string getFunctionName(const clang::FunctionDecl& functionDecl)
{
    return functionDecl.getNameInfo().getAsString();
}