#pragma once

#include <clang/Basic/SourceLocation.h>

class CheckResult
{
public:
    CheckResult() = default;

    CheckResult(clang::SourceLocation loc, std::string msg)
        : m_loc(loc)
        , m_msg(msg)
    {}

    CheckResult(const CheckResult& other, int guidelineId)
        : m_loc(other.loc())
        , m_msg(other.msg())
        , m_guidelineId(guidelineId)
    {}

    const clang::SourceLocation& loc() const
    {
        return m_loc;
    }

    const std::string& msg() const
    {
        return m_msg;
    }

    int guidelineId() const
    {
        return m_guidelineId;
    }

    bool failure() const
    {
        return !m_msg.empty();
    }

private:
    clang::SourceLocation m_loc;
    std::string m_msg;
    int m_guidelineId = 0;
};
