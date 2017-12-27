#ifndef PROJECT_SETTINGS_H
#define PROJECT_SETTINGS_H

#include <unordered_map>
#include <vector>


struct Settings
{
    std::array<uint16_t, 3> dimensions = {10, 10, 10};
    uint32_t bank_number = 16u;
    uint32_t bank_size = 256u;
    // TODO: Add core_to_mem_map here ?


    static Settings from_ast(const std::unordered_map<std::string, std::vector<uint8_t>>& ast);

    static Settings load(std::istream& raw);

    // TODO: Use operator<< and operator>>
    void dump(std::ostream& output);
};


#endif //PROJECT_SETTINGS_H
