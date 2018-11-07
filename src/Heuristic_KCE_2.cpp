#include "Helpers.hpp"
#include "Heuristics.hpp"
#include "Options.hpp"

#include <clang/AST/Decl.h>
#include <clang/AST/DeclCXX.h>

namespace
{


CheckResult checkForBooleanParams(const clang::FunctionDecl& functionDecl, const Options& options)
{
    for (const auto param : functionDecl.parameters()) {

        // llvm::outs() << param->getOriginalType().getAsString() << "\n";
        if (param->getOriginalType()->isBooleanType()) {
            return {
                loc(functionDecl),
                "boolean parameter",
                "function '" + getFunctionName(functionDecl) +
                "' has boolean parameter '" + param->getName().str() + "'"
            };
        }
    }

    return {};
}


} // ns


Heuristic createHeuristic_KCE_2()
{
    return Heuristic(
        "KCE-2", "Data types should be as specific as possible to make the code more readable",
        {
            { 2, "Use enumerations instead of booleans for options", &checkForBooleanParams },
        }
    );
}
