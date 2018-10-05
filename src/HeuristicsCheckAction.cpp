#include "HeuristicsCheckAction.hpp"
#include "Heuristics.hpp"

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

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

void writeToJson(
    std::ofstream& file,
    const clang::SourceManager& sm,
    const Heuristic& heuristic,
    const FailedChecks& failedChecks,
    bool onlyUserCode)
{
    for (const auto& fc : failedChecks) {
        if (!onlyUserCode || sm.getFileCharacteristic(fc.loc()) == clang::SrcMgr::C_User) {
            const std::string filename = sm.getFilename(fc.loc());
            file << "  {\n";
            file << "    \"file\": \"" << filename << "\",\n";
            file << "    \"line\": " << sm.getSpellingLineNumber(fc.loc()) << ",\n";
            file << "    \"heuristic\": \"" << heuristic.id() << "\",\n";
            file << "    \"message\": \"" << fc.msg() << "\"\n";
            file << "  },\n";
        }
    }
}

}


HeuristicsCheckAction::HeuristicsCheckAction(Options options)
    : m_options(options)
{
    m_heuristics.push_back(createHeuristic_KM_1());

    if (boost::get<bool>(m_options["json"].value())) {
        const auto filePath = fs::current_path() / "cpp_api_check_results.json";
        m_file = std::make_unique<std::ofstream>(filePath.c_str());

        if (m_file->is_open()) {
            *m_file << "{\n";
        }
    }
}


HeuristicsCheckAction::~HeuristicsCheckAction()
{
    if (m_file->is_open()) {
        *m_file << "}\n";
    }
}


void HeuristicsCheckAction::run(const MatchFinder::MatchResult& result)
{
    if (const auto functionDecl = result.Nodes.getNodeAs<clang::FunctionDecl>("func_decl")) {
        for (const auto& heuristic : m_heuristics) {
            const auto failedChecks = heuristic.check(*functionDecl, m_options);

            if (!failedChecks.empty()) {
                if (m_file && m_file->is_open()) {
                    writeToJson(*m_file, *result.SourceManager, heuristic, failedChecks, true);
                } else {
                    printToConsole(*result.SourceManager, heuristic, failedChecks, true);
                }
            }
        }
    }
}
