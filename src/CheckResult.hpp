#pragma once

#include <clang/Basic/SourceLocation.h>

/*! The result of a check operation
*/
class CheckResult
{
public:
    CheckResult() = default;

    //! Constructs a CheckResult object
    //! \param sourceLocation The location in the source code
    //!                       related to this check result
    //! \param message        A message describing the check result
    //! \param verboseMessage A more detailed message describing the check result
    CheckResult(
        clang::SourceLocation sourceLocation,
        std::string message,
        std::string verboseMessage
    )
        : m_sourceLocation(sourceLocation)
        , m_message(message)
        , m_verboseMessage(verboseMessage)
    {}

    CheckResult(const CheckResult& other, int guidelineId)
        : m_sourceLocation(other.sourceLocation())
        , m_message(other.message())
        , m_verboseMessage(other.verboseMessage())
        , m_guidelineId(guidelineId)
    {}

    //! Returns this check result's source location
    const clang::SourceLocation& sourceLocation() const
    {
        return m_sourceLocation;
    }

    //! Returns this check result's message
    const std::string& message() const
    {
        return m_message;
    }

    //! Returns this check result's detailed message
    const std::string& verboseMessage() const
    {
        return m_verboseMessage;
    }

    //! Returns the Guideline Id related to this check result
    int guidelineId() const
    {
        return m_guidelineId;
    }

    //! Returns \c true if this check result represents a failure
    //! and \c false otherwise
    bool failure() const
    {
        return !m_message.empty();
    }

private:
    clang::SourceLocation m_sourceLocation;
    std::string m_message;
    std::string m_verboseMessage;
    int m_guidelineId = 0;
};
