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

    const clang::SourceLocation& loc() const
    {
        return m_loc;
    }

    const std::string& msg() const
    {
        return m_msg;
    }

    bool failure() const
    {
        return !m_msg.empty();
    }

private:
    clang::SourceLocation m_loc;
    std::string m_msg;
};
