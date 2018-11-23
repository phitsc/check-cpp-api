#include "Helpers.hpp"
#include "Heuristics.hpp"
#include "Options.hpp"

#include <clang/AST/Decl.h>
#include <clang/AST/DeclCXX.h>

namespace
{


CheckResult checkForBooleanReturnAndOutParams(const clang::FunctionDecl& functionDecl, const Options& options)
{
    if (functionDecl.getReturnType()->isBooleanType()) {
        for (const auto param : functionDecl.parameters()) {
            if (getInOutType(param->getOriginalType()) == InOutType::Out) {
                return {
                    loc(functionDecl),
                    "omission to use optional?",
                    "function '" + getFunctionName(functionDecl) +
                    "' combines out parameter '" + param->getName().str() + "' with a boolean return type"
                };
            }
        }
    }

    return {};
}


} // ns


Heuristic createHeuristic_RU_2()
{
    return Heuristic(
        "RU-2", "The API should facilitate managing non-common but correct situations without generating exceptions or forcing users to catch them",
        {
            { 1, "Prefer standard over exceptional processing", &checkForBooleanReturnAndOutParams },
        }
    );
}
