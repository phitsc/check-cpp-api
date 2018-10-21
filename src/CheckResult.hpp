#pragma once

#include <clang/Basic/SourceLocation.h>

class CheckResult
{
public:
    CheckResult() = default;

    CheckResult(
        clang::SourceLocation loc,
        std::string message,
        std::string verboseMessage
    )
        : m_loc(loc)
        , m_message(message)
        , m_verboseMessage(verboseMessage)
    {}

    CheckResult(const CheckResult& other, int guidelineId)
        : m_loc(other.loc())
        , m_message(other.message())
        , m_verboseMessage(other.verboseMessage())
        , m_guidelineId(guidelineId)
    {}

    const clang::SourceLocation& loc() const
    {
        return m_loc;
    }

    const std::string& message() const
    {
        return m_message;
    }

    const std::string& verboseMessage() const
    {
        return m_verboseMessage;
    }

    int guidelineId() const
    {
        return m_guidelineId;
    }

    bool failure() const
    {
        return !m_message.empty();
    }

private:
    clang::SourceLocation m_loc;
    std::string m_message;
    std::string m_verboseMessage;
    int m_guidelineId = 0;
};
