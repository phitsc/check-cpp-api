
#include "MatchPrinter.hpp"
#include "HeuristicsCheckAction.hpp"
#include "Options.hpp"

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Rewrite/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Refactoring.h"
#include "clang/Tooling/ReplacementsYaml.h"

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/YAMLTraits.h"

#include <cstdlib>
#include <set>

using namespace clang::tooling;
using namespace llvm;

namespace boost
{
    void throw_exception(const std::exception& e)
    {
        llvm::errs() << "Error: " << e.what();
        std::exit(1);
    }
}

namespace {

static cl::OptionCategory optionCategory("check-cpp-api options");
// static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
// static cl::extrahelp MoreHelp("\nMore help text...\n");

// clang-format off
static cl::opt<bool> json("json", cl::desc(R"(
Export violations to JSON file
)"),
    cl::init(false),
    cl::cat(optionCategory));


static cl::opt<int> km13limit("km-1-3-limit", cl::desc(R"(
Consecutive parameter type limit
)"),
    cl::init(3),
    cl::cat(optionCategory));
// clang-format off

} // namespace


int main(int argc, const char** argv)
{
    CommonOptionsParser optionsParser(argc, argv, optionCategory);

    ClangTool tool(optionsParser.getCompilations(), optionsParser.getSourcePathList());

    // CLANG_BUILTIN_INCLUDES_DIR is defined in CMakeLists.txt
    // const auto clangBuiltinIncludesDir = std::string("-I") + CLANG_BUILTIN_INCLUDES_DIR;
    // tool.appendArgumentsAdjuster(getInsertArgumentAdjuster(clangBuiltinIncludesDir.c_str()));

    // tool.appendArgumentsAdjuster(getInsertArgumentAdjuster("-Wno-gnu-include-next"));

    Options options;
    options.add({ "json", "Export violations to JSON file", json });
    options.add({ "km-1-3-limit", "Consecutive parameter type limit", km13limit.getNumOccurrences() > 0 ? km13limit : 3 });

    MatchFinder matchFinder;

    // MatchPrinter matchPrinter;
    // matchFinder.addMatcher(statementMatcher, &matchPrinter);

    HeuristicsCheckAction heuristicsCheckAction(options);
    matchFinder.addMatcher(heuristicsCheckAction.matcher(), &heuristicsCheckAction);

    return tool.run(newFrontendActionFactory(&matchFinder).get());
}
