#include "DetermineCaseType.hpp"
#include "Helpers.hpp"
#include "Heuristics.hpp"
#include "Options.hpp"

#include <clang/AST/Decl.h>
#include <clang/AST/DeclCXX.h>
#include <llvm/Support/Casting.h>

namespace
{


CheckResult checkForConsistentNaming(const clang::FunctionDecl& functionDecl, const Options& options)
{
    if (llvm::isa<clang::CXXConstructorDecl>(&functionDecl)) {
        // ignore constructors. they need to follow
        // the naming convention of their class.
        return {};
    }

    const auto expectedCaseType = caseTypeFromInt(options["kc-1-1-case-type"].as<int>());

    const auto name = getFunctionName(functionDecl);
    const auto caseType = determineCaseType(name);

    if (caseType != expectedCaseType && caseType != CaseType::Unknown) {
        return {
            loc(functionDecl),
            "invalid case type",
            "function '" + name + "' uses case type '" + toString(caseType) + "'"
            " instead of '" + toString(expectedCaseType) + "'"
        };
    }

    return {};
}

// substitute for QualType::isConstQualified() which does not
// seem to work properly for const references.
bool isConst(const clang::QualType& type)
{
    return type.getAsString().find("const ") == 0;
}


CheckResult checkForConsistentParameterOrdering(const clang::FunctionDecl& functionDecl, const Options& options)
{
    // check for mixture of in/out params
    enum class InOutType
    {
        Uninitialized,
        In,
        Out
    } inOutType = InOutType::Uninitialized;
    auto inOutTypeChanged = false;

    const auto getInOutType = [](const auto& type) {
        return (!type.getTypePtr()->isLValueReferenceType() || isConst(type)) ? InOutType::In : InOutType::Out;
    };

    for (const auto param : functionDecl.parameters()) {
        const auto& type = param->getOriginalType();

        const auto currentInOutType = getInOutType(type);

        if (inOutType == InOutType::Uninitialized) {
            inOutType = currentInOutType;
        } else if (currentInOutType != inOutType) {
            if (inOutTypeChanged) {
                return {
                    loc(functionDecl),
                    "inconsistent ordering of in and out parameters",
                    "function '" + getFunctionName(functionDecl) +
                    "' has inconsistent in and out parameter ordering"
                };
            } else {
                inOutType = currentInOutType;
                inOutTypeChanged = true;
            }
        }
    }

    return {};
}


} // ns

Heuristic createHeuristic_KC_1()
{
    return Heuristic(
        "KCE-1", "The API should be consistent with itself",
        {
            { 1, "Use consistent naming", &checkForConsistentNaming },
            { 2, "Use consistent parameter ordering", &checkForConsistentParameterOrdering },
        }
    );
}
