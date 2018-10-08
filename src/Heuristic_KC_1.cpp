#include "DetermineCaseType.hpp"
#include "Helpers.hpp"
#include "Heuristics.hpp"
#include "Options.hpp"

#include <clang/AST/Decl.h>
#include <clang/AST/DeclCXX.h>

namespace
{


CheckResult checkForConsistentNaming(const clang::FunctionDecl& functionDecl, const Options& options)
{
    const auto expectedCaseType = boost::get<int>(options["kc-1-1-case-type"].value());

    const auto name = getFunctionName(functionDecl);

    if (static_cast<int>(determineCaseType(name)) != expectedCaseType) {
        return { loc(functionDecl), "invalid case type" };
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
