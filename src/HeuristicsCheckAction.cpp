#include "HeuristicsCheckAction.hpp"
#include "Heuristics.hpp"

#include <iomanip>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

namespace
{

// sole purpose of this function is to keep the
// respective code at the top of this file
// so new heurstics are easier to add
std::vector<Heuristic> createHeuristics()
{
    return {
        createHeuristic_KC_1(),
        createHeuristic_KCE_1(),
        createHeuristic_KCE_2(),
        createHeuristic_KM_1(),
        createHeuristic_RU_2()
    };
}


void printToConsole(
    const clang::SourceManager& sm,
    const Heuristic& heuristic,
    const FailedChecks& failedChecks,
    bool onlyUserCode,
    bool beVerbose)
{
    for (const auto& fc : failedChecks) {
        const auto ploc = sm.getPresumedLoc(fc.sourceLocation());

        if (ploc.isValid() &&
            (!onlyUserCode || sm.getFileCharacteristic(fc.sourceLocation()) == clang::SrcMgr::C_User)) {
            llvm::outs()
                << ploc.getFilename() << ":" << ploc.getLine() << ": "
                << heuristic.id() << "-" << fc.guidelineId() << ": " << fc.message() << "\n";

            if (beVerbose) {
                llvm::outs()
                    << " : " << fc.verboseMessage() << "\n";
            }
        }
    }
}


void writeToJson(
    nlohmann::json& json,
    const clang::SourceManager& sm,
    const Heuristic& heuristic,
    const FailedChecks& failedChecks,
    bool onlyUserCode)
{
    for (const auto& fc : failedChecks) {
        if (!onlyUserCode || sm.getFileCharacteristic(fc.sourceLocation()) == clang::SrcMgr::C_User) {
            const std::string filename = sm.getFilename(fc.sourceLocation());
            json += {
                { "file", filename },
                { "guideline", fc.guidelineId() },
                { "heuristic", heuristic.id() },
                { "line", sm.getSpellingLineNumber(fc.sourceLocation()) },
                { "message", fc.message() }
            };
        }
    }
}

} // ns

HeuristicsCheckAction::HeuristicsCheckAction(Options options)
    : m_heuristics(createHeuristics())
    , m_options(options)
{
    const auto filePath = m_options["json"].as<std::string>();
    if (!filePath.empty()) {
        m_ofstream = std::make_unique<std::ofstream>(filePath.c_str());

        if (!m_ofstream->is_open()) {
            llvm::errs() << "Error: failed to create " << filePath << "\n";
        }
    }
}


HeuristicsCheckAction::~HeuristicsCheckAction()
{
    if (m_ofstream && m_ofstream->is_open()) {
        *m_ofstream << std::setw(2) << m_json << std::endl;
    }
}


void HeuristicsCheckAction::run(const clang::ast_matchers::MatchFinder::MatchResult& result)
{
    if (const auto functionDecl = result.Nodes.getNodeAs<clang::FunctionDecl>("func_decl")) {
        for (const auto& heuristic : m_heuristics) {
            const auto failedChecks = heuristic.check(*functionDecl, m_options);

            if (!failedChecks.empty()) {
                if (m_ofstream && m_ofstream->is_open()) {
                    writeToJson(
                        m_json,
                        *result.SourceManager,
                        heuristic,
                        failedChecks,
                        true);
                } else {
                    printToConsole(
                        *result.SourceManager,
                        heuristic,
                        failedChecks,
                        true, // only user code
                        m_options["verbose"].as<bool>());
                }
            }
        }
    }
}
