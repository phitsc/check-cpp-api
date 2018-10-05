#include "HeuristicsCheckAction.hpp"
#include "Heuristics.hpp"

namespace
{

void printToConsole(
    const clang::SourceManager& sm,
    const Heuristic& heuristic,
    const FailedChecks& failedChecks,
    bool onlyUserCode)
{
    for (const auto& fc : failedChecks) {
        if (!onlyUserCode || sm.getFileCharacteristic(fc.loc()) == clang::SrcMgr::C_User) {
            llvm::outs()
                << sm.getFilename(fc.loc()) << ":" << sm.getSpellingLineNumber(fc.loc()) << ": "
                << heuristic.id() << ": " << fc.msg() << "\n";
        }
    }
}

}

HeuristicsCheckAction::HeuristicsCheckAction(Options options)
    : m_options(options)
{
    m_heuristics.push_back(createHeuristic_KM_1());
}

void HeuristicsCheckAction::run(const MatchFinder::MatchResult& result)
{
    if (const auto functionDecl = result.Nodes.getNodeAs<clang::FunctionDecl>("func_decl")) {

        for (const auto& heuristic : m_heuristics) {
            const auto failedChecks = heuristic.check(*functionDecl, m_options);

            if (!failedChecks.empty()) {
                printToConsole(*result.SourceManager, heuristic, failedChecks, true);
            }
        }
    }
}
