#include "settings.h"

#include "throw_assert.h"

#include <cstring>


Settings Settings::from_ast(const std::unordered_map<std::string, std::vector<uint16_t>>& ast)
{
    Settings settings;

    cpu_assert(ast.at("cores").size() == 3, "Wrong number of dimensions");
    settings.dimensions.at(0) = ast.at("cores").at(0);
    settings.dimensions.at(1) = ast.at("cores").at(1);
    settings.dimensions.at(2) = ast.at("cores").at(2);
    settings.bank_number = ast.at("mem_number").at(0);
    settings.bank_size = ast.at("mem_size").at(0);
    std::copy(std::cbegin(ast.at("core_to_mem")), std::cend(ast.at("core_to_mem")), std::back_inserter(settings.core_to_mem));

    if (ast.count("in"))
    {
        std::copy(std::cbegin(ast.at("in")), std::cend(ast.at("in")), std::back_inserter(settings.inputs));
    }

    if (ast.count("out"))
    {
        std::copy(std::cbegin(ast.at("out")), std::cend(ast.at("out")), std::back_inserter(settings.outputs));
    }

    return settings;
}
