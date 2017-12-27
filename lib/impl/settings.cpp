#include "settings.h"

#include <algorithm>
#include <cassert>
#include <istream>


Settings Settings::from_ast(const std::unordered_map<std::string, std::vector<uint8_t>>& ast)
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

Settings Settings::load(std::istream& raw)
{
    Settings settings;

    std::array<char, sizeof(settings)> buffer;
    auto& status = raw.read(buffer.data(), buffer.size());

    assert(status);

    std::copy_n(buffer.data(), sizeof(settings), reinterpret_cast<uint8_t*>(&settings));

    return settings;
}

void Settings::dump(std::ostream& output)
{
    std::copy_n(reinterpret_cast<char*>(this), sizeof(*this), std::ostreambuf_iterator(output));
}
