#include "Helpers.hpp"
#include "Heuristics.hpp"
#include "Options.hpp"

#include <clang/AST/Decl.h>
#include <clang/AST/DeclCXX.h>

#include <boost/algorithm/string.hpp>

#include <array>

namespace
{


constexpr const std::array<const char*, 5> CommonAbbreviations = { {
    "calc", "desc", "loc", "msg", "rect"
} };


CheckResult checkForAbbreviation(const clang::FunctionDecl& functionDecl, const Options& options)
{
    const auto name = boost::to_lower_copy(getFunctionName(functionDecl));

    for (const auto term : CommonAbbreviations) {
        if (boost::ends_with(name, term)) {
            return {
                loc(functionDecl),
                "abbreviation",
                "function '" + name + "' ends with an abbreviation"
            };
        }
    }

    return {};
}


} // ns

Heuristic createHeuristic_KCE_1()
{
    return Heuristic(
        "KCE-1", "Names should be self-explanatory",
        {
            { 1, "Avoid cryptographic names and abbreviations", &checkForAbbreviation },
        }
    );
}
