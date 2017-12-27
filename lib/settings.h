#ifndef PROJECT_SETTINGS_H
#define PROJECT_SETTINGS_H

#include <cstddef>
#include <initializer_list>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iterator>

struct Settings
{
    std::array<uint16_t, 3> dimensions = {10, 10, 10};
    uint32_t bank_number = 16u;
    uint32_t bank_size = 256u;
    // TODO: Add core_to_mem_map here ?


    static Settings from_ast(const std::unordered_map<std::string, std::vector<uint8_t>>& ast)
    {
        Settings settings;

        assert(ast.at("cores").size() == 3);
        settings.dimensions.at(0) = ast.at("cores").at(0);
        settings.dimensions.at(1) = ast.at("cores").at(1);
        settings.dimensions.at(2) = ast.at("cores").at(2);
        settings.bank_number = ast.at("mem_number").at(0);
        settings.bank_size = ast.at("mem_size").at(0);

        return settings;
    }

    std::unordered_map<std::string, std::vector<long unsigned>> to_ast()
    {
        std::unordered_map<std::string, std::vector<long unsigned>> ast;

        ast["cores"].assign(std::cbegin(dimensions), std::cend(dimensions));
        ast["mem_number"] = {bank_number};
        ast["mem_size"] = {bank_size};

        return ast;
    }

    // TODO: Use operator<< and operator>>
    void dump(std::ostream& output)
    {
        std::copy_n(reinterpret_cast<char*>(this), sizeof(*this), std::ostreambuf_iterator(output));
    }

    static Settings load(std::istream& raw)
    {
        Settings settings;

        std::array<char, sizeof(settings)> buffer;
        auto& status = raw.read(buffer.data(), buffer.size());

        assert(status);

        std::copy_n(buffer.data(), sizeof(settings), reinterpret_cast<uint8_t*>(&settings));

        return settings;
    }
};


#endif //PROJECT_SETTINGS_H
