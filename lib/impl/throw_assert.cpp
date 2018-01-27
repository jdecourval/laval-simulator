#include "throw_assert.h"


CpuException::StreamFormatter::operator std::string() const
{
    return stream.str();
}

CpuException::CpuException(const char* expression, const char* file, int line, const std::string& message)
        : expression(expression)
          , file(file)
          , line(line)
          , message(message)
{
    std::ostringstream outputStream;

    if (!message.empty())
    {
        outputStream << message << ": ";
    }

    std::string expressionString = expression;

    if (expressionString == "false" || expressionString == "0" || expressionString == "FALSE")
    {
        outputStream << "Unreachable code assertion";
    }
    else
    {
        outputStream << "Assertion '" << expression << "'";
    }

    outputStream << " failed in file '" << file << "' line " << line << "\n";
    report = outputStream.str();
}

const char* CpuException::what() const noexcept
{
    return report.c_str();
}

const char* CpuException::Expression() const noexcept
{
    return expression;
}

const char* CpuException::File() const noexcept
{
    return file;
}

int CpuException::Line() const noexcept
{
    return line;
}

const char* CpuException::Message() const noexcept
{
    return message.c_str();
}

void CpuException::add_registers(const Registers& registers)
{
    assert(!this->registers);   // Registers should not have been already set

    this->registers = registers;

    std::ostringstream output;
    output << report << "\n";
    output << "Registers:" << "\n";
    output << registers;
    report =  output.str();

    add_line_infos(registers.status2.membank, registers.pc + 1);
}

void CpuException::add_line_infos(int membank, int instruction)
{
    assert(!this->line_infos);   // Line infos should not have been already set

    this->line_infos = std::make_pair(membank, instruction);

    std::ostringstream output;
    output << report << "\n";
    output << "In membank " << line_infos->first << " at its instruction #" << line_infos->second << "\n";
    report =  output.str();
}
