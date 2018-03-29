#ifndef THROW_ASSERT_H
#define THROW_ASSERT_H

#include "registers.h"

#include <cassert>
#include <exception>
#include <iostream>
#include <string>
#include <sstream>

// Greatly inspired by https://github.com/Softwariness/ThrowAssert


/// Exception type for assertion failures
class CpuException final : public std::exception
{
public:
    /// Helper class for formatting assertion message
    class StreamFormatter
    {
    public:
        operator std::string() const;  // NOLINT: Not marked as explicit since this function rely on implicit conversions.

        template<typename T>
        StreamFormatter& operator<<(const T& value)
        {
            stream << value;
            return *this;
        }

    private:
        std::ostringstream stream;
    };


    /// Construct an assertion failure exception
    CpuException(const char* expression, const char* file, int line, const std::string& message);

    /// The assertion message
    const char* what() const noexcept override final;

    /// The expression which was asserted to be true
    const char* Expression() const noexcept;

    /// Source file
    const char* File() const noexcept;

    /// Source line
    int Line() const noexcept;

    /// Description of failure
    const char* Message() const noexcept;

    void add_registers(const Registers& registers);

    void add_line_infos(int membank, int instruction);


private:
    const char* expression;
    const char* file;
    int line;
    std::string message;
    std::string report;
    std::optional<Registers> registers;
    std::optional<std::pair<int, int>> line_infos;
};


/// Assert that EXPRESSION evaluates to true, otherwise rise CpuException with associated MESSAGE (which may use C++ stream-style message formatting)
#define cpu_assert(EXPRESSION, MESSAGE) if(!(EXPRESSION)) { throw CpuException(#EXPRESSION, __FILE__, __LINE__, (CpuException::StreamFormatter() << MESSAGE)); }


#endif //THROW_ASSERT_H
