#include "Heuristics.hpp"
#include "Options.hpp"

#include <clang/AST/Decl.h>
#include <clang/AST/DeclCXX.h>

#include <boost/optional.hpp>

namespace
{


void append(std::string& dest, const std::string& src, const std::string& divider = ". ")
{
    if (!dest.empty()) {
        dest += divider;
    }

    dest += src;
}


clang::SourceLocation loc(const clang::FunctionDecl& functionDecl)
{
    return functionDecl.getSourceRange().getBegin();
}


CheckResult checkFunctionNameLength(const clang::FunctionDecl& functionDecl, const Options& options)
{
    const auto limit = boost::get<int>(options["km-1-1-limit"].value());

    const auto name = functionDecl.getNameInfo().getAsString();

        // ignore operators
    if ((int)name.length() > limit && name.find("operator ") == std::string::npos) {
        auto methodDecl = clang::dyn_cast<clang::CXXMethodDecl>(&functionDecl);

            // also ignore generated methods
        if (!methodDecl || methodDecl->isUserProvided()) {
            return { loc(functionDecl), "long function name" };
        }
    }

    return {};
}


CheckResult checkParamCount(const clang::FunctionDecl& functionDecl, const Options& options)
{
    const auto limit = boost::get<int>(options["km-1-2-limit"].value());

    if ((int)functionDecl.parameters().size() > limit) {
        return { loc(functionDecl), std::to_string(functionDecl.parameters().size()) + " parameters" };
    } else {
        return {};
    }
}


CheckResult checkConsecutiveParams(const clang::FunctionDecl& functionDecl, const Options& options)
{
    const auto limit = boost::get<int>(options["km-1-3-limit"].value());

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
                    append(msg, *res);
                }

                params.clear();
            }
        }

        params.push_back(param);
    }

    if (auto res = check(params)) {
        append(msg, *res);
    }

    return {
        !msg.empty() ? loc(functionDecl) : clang::SourceLocation(),
        msg
    };
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
