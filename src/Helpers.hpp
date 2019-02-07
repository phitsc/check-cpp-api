//! \file Helpers.hpp
#pragma once

#include <clang/AST/Decl.h>
#include <clang/Basic/SourceLocation.h>

#include <string>

//! \addtogroup free_functions Free functions
//! \{

//! Appends \c source to \c destination, first appending \c divider
//! if \c destination is not empty
//
//! \param destination The string to append to
//! \param source The string to append
//! \param divider The string to prepend to \c source in case \c destination is not empty
inline void append(std::string& destination, const std::string& source, const std::string& divider)
{
    if (!destination.empty()) {
        destination += divider;
    }

    destination += source;
}


//! Returns \c functionDecl's source location
inline clang::SourceLocation loc(const clang::FunctionDecl& functionDecl)
{
    return functionDecl.getSourceRange().getBegin();
}


//! Returns \c functionDecl's function name
inline std::string getFunctionName(const clang::FunctionDecl& functionDecl)
{
    return functionDecl.getNameInfo().getAsString();
}


//! Returns \c true if type represents a const-qualified type
//! and \c false otherwise
//
// substitute for QualType::isConstQualified() which does not
// seem to work properly for const references.
inline bool isConst(const clang::QualType& type)
{
    return type.getAsString().find("const ") == 0;
}

//! A type used to differentiate between \a in and \a out parameters
enum class InOutType
{
    Uninitialized, //! Neither an \a in nor an \a out parameters
    In, //! An \a in parameter
    Out //! An \a out parameter
};


//! Returns \c InOutType::In if type represents an in parameter
//! and \c InOutType::Out otherwise
inline InOutType getInOutType(const clang::QualType& type)
{
    return (!type->isLValueReferenceType() || isConst(type)) ? InOutType::In : InOutType::Out;
};

//! \}
