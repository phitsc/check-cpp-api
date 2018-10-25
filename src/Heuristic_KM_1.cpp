#include "Helpers.hpp"
#include "Heuristics.hpp"
#include "Options.hpp"

#include <clang/AST/Decl.h>
#include <clang/AST/DeclCXX.h>

#include <boost/optional.hpp>

namespace
{


CheckResult checkFunctionNameLength(const clang::FunctionDecl& functionDecl, const Options& options)
{
    const auto limit = options["km-1-1-limit"].as<int>();

    const auto name = getFunctionName(functionDecl);

    // ignore template parameter names
    const auto pos = name.find('<');
    const auto len = pos == std::string::npos ? name.length() : pos;

    // ignore operators
    if ((int)len > limit && name.find("operator ") == std::string::npos) {
        auto methodDecl = clang::dyn_cast<clang::CXXMethodDecl>(&functionDecl);

        // also ignore generated methods
        if (!methodDecl || methodDecl->isUserProvided()) {
            return {
                 loc(functionDecl),
                 "long function name",
                 "the name of function '" + name + "' is " + std::to_string(len) +
                 " characters long, which is more than " + std::to_string(limit) + " characters"
             };
        }
    }

    return {};
}


CheckResult checkParamCount(const clang::FunctionDecl& functionDecl, const Options& options)
{
    const auto limit = options["km-1-2-limit"].as<int>();

    if ((int)functionDecl.parameters().size() > limit) {
        return {
            loc(functionDecl),
             "too many parameters",
             "function '" + getFunctionName(functionDecl) + "' has " + std::to_string(functionDecl.parameters().size()) +
             " parameters, which are more than " + std::to_string(limit) + " parameters"
        };
    } else {
        return {};
    }
}


CheckResult checkConsecutiveParams(const clang::FunctionDecl& functionDecl, const Options& options)
{
    const auto limit = options["km-1-3-limit"].as<int>();

    auto check = [limit](const auto& params) -> boost::optional<std::string> {
        if ((int)params.size() > limit) {
            return std::to_string(params.size()) + " consecutive parameters of type " + params.back()->getOriginalType().getAsString();
        } else {
            return {};
        }
    };

    std::string msg;

    std::vector<clang::ParmVarDecl*> params;

    for (const auto param : functionDecl.parameters()) {

        if (!params.empty()) {
            if (params.back()->getOriginalType() != param->getOriginalType()) {
                if (auto res = check(params)) {
                    append(msg, *res, " and ");
                }

                params.clear();
            }
        }

        params.push_back(param);
    }

    if (auto res = check(params)) {
        append(msg, *res, " and ");
    }

    if (!msg.empty()) {
        msg += " (only " + std::to_string(limit) + " consecutive parameters of same type allowed)";

        return {
            loc(functionDecl),
            "too many consecutive parameters of same type",
            "function '" + getFunctionName(functionDecl) + "' has " + msg
        };
    } else {
        return {};
    }

}


} // ns


Heuristic createHeuristic_KM_1()
{
    return Heuristic(
        "KM-1", "The API should be easy to remember",
        {
            { 1, "Prefer short function names", &checkFunctionNameLength },
            { 2, "Avoid functions with many parameters", &checkParamCount },
            { 3, "Avoid functions with many consecutive parameters of the same type", &checkConsecutiveParams }
        }
    );
}
