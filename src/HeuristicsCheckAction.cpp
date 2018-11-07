#include "HeuristicsCheckAction.hpp"
#include "Heuristics.hpp"

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
        const auto ploc = sm.getPresumedLoc(fc.loc());

        if (ploc.isValid() &&
            (!onlyUserCode || sm.getFileCharacteristic(fc.loc()) == clang::SrcMgr::C_User)) {
            llvm::outs()
                << ploc.getFilename() << ":" << ploc.getLine() << ": "
                << heuristic.id() << "-" << fc.guidelineId() << ": " << fc.message() << "\n";

            if (beVerbose) {
                llvm::outs()
                    << ": " << fc.verboseMessage() << "\n";
            }
        }
    }
}


void writeToJson(
    std::ofstream& file,
    const clang::SourceManager& sm,
    const Heuristic& heuristic,
    const FailedChecks& failedChecks,
    bool onlyUserCode,
    bool& isFirstRecord)
{
    for (const auto& fc : failedChecks) {
        if (!onlyUserCode || sm.getFileCharacteristic(fc.loc()) == clang::SrcMgr::C_User) {
            if (isFirstRecord) {
                isFirstRecord = false;
            } else {
                file << ",\n";
            }

            const std::string filename = sm.getFilename(fc.loc());
            file << "  {\n";
            file << "    \"file\": \"" << filename << "\",\n";
            file << "    \"line\": " << sm.getSpellingLineNumber(fc.loc()) << ",\n";
            file << "    \"heuristic\": \"" << heuristic.id() << "\",\n";
            file << "    \"guideline\": " << fc.guidelineId() << ",\n";
            file << "    \"message\": \"" << fc.message() << "\"\n";
            file << "  }";
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
        m_file = std::make_unique<std::ofstream>(filePath.c_str());

        if (m_file && m_file->is_open()) {
            *m_file << "{\n";
        } else {
            llvm::errs() << "Error: failed to create " << filePath << "\n";
        }
    }
}


HeuristicsCheckAction::~HeuristicsCheckAction()
{
    if (m_file && m_file->is_open()) {
        *m_file << "\n}";
    }
}


void HeuristicsCheckAction::run(const MatchFinder::MatchResult& result)
{
    if (const auto functionDecl = result.Nodes.getNodeAs<clang::FunctionDecl>("func_decl")) {
        for (const auto& heuristic : m_heuristics) {
            const auto failedChecks = heuristic.check(*functionDecl, m_options);

            if (!failedChecks.empty()) {
                if (m_file && m_file->is_open()) {
                    writeToJson(
                        *m_file,
                        *result.SourceManager,
                        heuristic,
                        failedChecks,
                        true,
                        m_isFirstRecord);
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
