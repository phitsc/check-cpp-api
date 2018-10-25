#include "CaseType.hpp"
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

void printVersion(llvm::raw_ostream& ostream)
{
    ostream << "check-cpp-api version 0.1.0\n";

    std::exit(0);
}

static cl::OptionCategory optionCategory("check-cpp-api Options");

// clang-format off
static cl::opt<bool> json("json",
    cl::desc("Export violations to JSON file"),
    cl::init(false),
    cl::cat(optionCategory)
);

static cl::opt<bool> verbose("verbose",
    cl::desc("Be verbose when reporting violations"),
    cl::init(false),
    cl::cat(optionCategory)
);

static cl::alias verboseAlias("v",
    cl::desc("Same as -verbose"),
    cl::aliasopt(verbose)
);

constexpr const auto kc_1_1_init = camel;
cl::opt<CaseTypeOption> kc_1_1_case_type("kc-1-1-case-type",
    cl::desc("The case type used by the API (default: camel)"),
    cl::values(
        clEnumVal(camel,     "camelCase"),
        clEnumVal(pascal,    "PascalCase"),
        clEnumVal(snake,     "snake_case"),
        clEnumVal(hungarian, "szHungarianNotation")
    ),
    cl::cat(optionCategory)
);

constexpr const int km_1_1_init = 30;
static cl::opt<int> km_1_1_limit("km-1-1-limit",
    cl::desc("Function name length limit (default: 30)"),
    cl::init(km_1_1_init),
    cl::cat(optionCategory)
);

constexpr const int km_1_2_init = 6;
static cl::opt<int> km_1_2_limit("km-1-2-limit",
    cl::desc("Parameter count limit (default: 6)"),
    cl::init(km_1_2_init),
    cl::cat(optionCategory)
);

constexpr const int km_1_3_init = 3;
static cl::opt<int> km_1_3_limit("km-1-3-limit",
    cl::desc("Consecutive parameter type limit (default: 3)"),
    cl::init(km_1_3_init),
    cl::cat(optionCategory)
);
// clang-format off

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
// static cl::extrahelp MoreHelp("\nMore help text...\n");

template<typename T>
void addOpt(Options& opts, const T& opt, int optInit = -1)
{
    if (optInit == -1) {
        opts.add({ opt.ArgStr, opt.HelpStr, opt });
    } else {
        opts.add({ opt.ArgStr, opt.HelpStr, opt.getNumOccurrences() > 0 ? opt : optInit });
    }
}

} // namespace


int main(int argc, const char** argv)
{
    cl::SetVersionPrinter(&printVersion);

    CommonOptionsParser optionsParser(argc, argv, optionCategory);

    ClangTool tool(optionsParser.getCompilations(), optionsParser.getSourcePathList());

    // CLANG_BUILTIN_INCLUDES_DIR is defined in CMakeLists.txt
    // const auto clangBuiltinIncludesDir = std::string("-I") + CLANG_BUILTIN_INCLUDES_DIR;
    // tool.appendArgumentsAdjuster(getInsertArgumentAdjuster(clangBuiltinIncludesDir.c_str()));

    // tool.appendArgumentsAdjuster(getInsertArgumentAdjuster("-Wno-gnu-include-next"));

    Options options;
    addOpt(options, json);
    addOpt(options, verbose);
    addOpt(options, kc_1_1_case_type, kc_1_1_init);
    addOpt(options, km_1_1_limit, km_1_1_init);
    addOpt(options, km_1_2_limit, km_1_2_init);
    addOpt(options, km_1_3_limit, km_1_3_init);

    MatchFinder matchFinder;

    // MatchPrinter matchPrinter;
    // matchFinder.addMatcher(statementMatcher, &matchPrinter);

    HeuristicsCheckAction heuristicsCheckAction(options);
    matchFinder.addMatcher(heuristicsCheckAction.matcher(), &heuristicsCheckAction);

    return tool.run(newFrontendActionFactory(&matchFinder).get());
}
