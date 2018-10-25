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


} // ns

Heuristic createHeuristic_KC_1()
{
    return Heuristic(
        "KCE-1", "The API should be consistent with itself",
        {
            { 1, "Use consistent naming", &checkForConsistentNaming },
        }
    );
}
