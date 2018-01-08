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
    using BlockId = int;

    /// An instruction with its optional arguments for use in the AST
    using Node = std::pair<std::string, std::vector<uint8_t>>;

    /// Abstract Syntax Tree
    ///
    /// Represents the complete program structure
    /// This is not really a tree since assembly language does not need one
    using Ast = std::unordered_map<BlockId, std::vector<Node>>;

    /// Parsed settings, part of the AST
    using SettingMap = std::unordered_map<std::string, std::vector<uint8_t>>;

    /// Preprocess input into output
    void preprocess(std::istream& input, std::ostream& output);

    /// Build an AST from an assembly file
    std::tuple<Ast, SettingMap, std::vector<std::vector<std::pair<BlockId, int>>>> build_ast(std::istream& input);

    /// Assemble an AST to binary
    // TODO: Standardize variable/parameter/arg naming
    void assemble(const Ast&, const SettingMap&, std::vector<std::vector<std::pair<BlockId, int>>> variables, std::ostream& output);

    /// Configure a CPU instance with a binary input
    Cpu load_binary(std::istream& input);
};


#endif //PROJECT_ASSEMBLER_H
