#ifndef PROJECT_ASSEMBLER_H
#define PROJECT_ASSEMBLER_H

#if __GNUC__ < 8
#include <experimental/filesystem>
using path = std::experimental::filesystem::path;
#else
#include <filesystem>
using path = std::filesystem::path;
#endif

#include <unordered_map>
#include <utility>
#include <vector>
#include "cpu.h"

namespace Assembler
{
    /// An instruction with its optional arguments for use in the AST
    using Node = std::pair<std::string, std::vector<uint8_t>>;

    /// Abstract Syntax Tree
    ///
    /// Represents the complete program structure
    /// This is not really a tree since assembly language does not need one
    using Ast = std::unordered_map<int, std::vector<Node>>;

    /// Parsed settings, part of the AST
    using SettingMap = std::unordered_map<std::string, std::vector<uint8_t>>;

    /// Build an AST from an assembly file
    std::pair<Ast, SettingMap> build_ast(std::istream& path);

    /// Assemble an AST to binary
    void assemble(const Ast&, const SettingMap&, std::ostream& output);

    /// Configure a CPU instance with a binary input
    Cpu load_binary(std::istream& input);
};


#endif //PROJECT_ASSEMBLER_H
