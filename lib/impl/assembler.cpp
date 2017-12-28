#include "assembler.h"

#include <regex>


namespace
{
    /// trim from start (in place)
    void ltrim(std::string& s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch)
        {
            return !std::isspace(ch);
        }));
    }

    /// trim from end (in place)
    void rtrim(std::string& s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch)
        {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    /// trim from both ends (in place)
    void trim(std::string& s)
    {
        ltrim(s);
        rtrim(s);
    }

    /// Split s at every delim into a vector of string
    std::vector<std::string> split(const std::string& s, char delim)
    {
        std::vector<std::string> elems;
        std::stringstream ss(s);
        std::string item;

        while (std::getline(ss, item, delim))
        {
            elems.push_back(item);
        }

        return elems;
    }
}

namespace Assembler
{
    std::pair<Ast, SettingMap> build_ast(std::istream& input)
    {
        assert(input);

        // Regex
        std::regex find_setting(R"(\.(\w+) ([\d, ]*))");
        std::regex find_block(R"((\d+):)");
        std::regex find_instruction(R"((\w{3})( -?\d+(?:, ?-?\d+)*)?)");

        // Result
        Ast blocks;
        SettingMap settings;

        // State machine
        auto setting_done = false;
        auto current_block = std::optional < int > ();
        std::smatch base_match;

        for (std::string line; getline(input, line);)
        {
            trim(line);

            if (line.empty() || line.at(0) == ';')
            {
                continue;
            }
            else if (!setting_done && std::regex_match(line, base_match, find_setting))
            {
                assert(base_match.size() == 3);

                auto name = base_match[1];
                auto args = split(base_match[2], ',');

                std::vector<uint8_t> args_int;
                std::transform(std::cbegin(args), std::cend(args), std::back_inserter(args_int), [](auto& arg)
                {
                    auto arg_int = std::stol(arg);
                    assert(arg_int <= 0xff);
                    assert(arg_int >= 0);
                    return arg_int;
                });

                settings.emplace(name, args_int);
            }
            else if (current_block && std::regex_match(line, base_match, find_instruction))
            {
                assert(base_match.size() == 3);  // All + OPCode + args

                auto instruction = base_match[1];
                auto args = split(base_match[2], ',');

                std::vector<uint8_t> args_int;
                std::transform(std::cbegin(args), std::cend(args), std::back_inserter(args_int), [](auto& arg)
                {
                    auto arg_int = std::stol(arg);
                    assert(arg_int <= 0xff);
                    return arg_int;
                });

                blocks[*current_block].emplace_back(instruction, args_int);
            }
            else if (std::regex_match(line, base_match, find_block))
            {
                assert(base_match.size() == 2);  // All + id
                setting_done = true;
                current_block = std::stoi(base_match[1]);
            }
            else
            {
                std::cout << "error" << std::endl;
            }
        }

        return {blocks, settings};
    }

    void assemble(const Ast& ast, const SettingMap& setting_map, std::ostream& output)
    {
        auto settings = Settings::from_ast(setting_map);
        settings.dump(output);

        auto& core_to_mem_map = setting_map.at("mem_map");
        assert(core_to_mem_map.size() <= 0xff);
        output.put(static_cast<uint8_t>(core_to_mem_map.size()));
        assert(sizeof(core_to_mem_map[0]) == 1);
        output.write(reinterpret_cast<const char*>(core_to_mem_map.data()), core_to_mem_map.size());

        // TODO: Remove core requirement
        Core core;
        auto& factory = core.get_factory();

        for (auto& [bank_id, instructions]: ast)
        {
            assert(bank_id <= 0xff);
            output.put(static_cast<uint8_t>(bank_id));

            assert(instructions.size() <= 0xff);
            output.put(static_cast<uint8_t>(instructions.size()));

            for (auto& instruction_ast: instructions)
            {
                auto instruction = factory.create(instruction_ast);
                auto instruction_raw = factory.dump(*instruction);
                output.put(instruction_raw);
            }
        }
    }

    Cpu load_binary(std::istream& input)
    {
        auto settings = Settings::load(input);
        auto memory = Memory(settings);

        auto core_to_mem_map_size = char{};
        input.get(core_to_mem_map_size);
        assert(input);

        auto core_to_mem_map = std::vector<uint8_t>(core_to_mem_map_size);

        input.read(reinterpret_cast<char*>(core_to_mem_map.data()), core_to_mem_map.capacity());
        assert(input);

        while (true)
        {
            auto membank_id = char{};
            input.get(membank_id);
            if (!input)
            {
                break;
            }

            auto membank_len = char{};
            input.get(membank_len);
            assert(input);
            assert(memory.banks_size() >= membank_len);

            input.read(reinterpret_cast<char*>(memory.at(membank_id).data()), membank_len);
            assert(input);
        }

        return Cpu(settings, std::move(memory), std::move(core_to_mem_map));
    }
}
