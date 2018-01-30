#include "assembler.h"

#include <cereal/archives/binary.hpp>

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
    void preprocess(std::istream& input, std::ostream& output)
    {
        for(std::string line; getline(input, line);)
        {
            using namespace Direction;

            line = std::regex_replace(line, std::regex("BEFORE"), std::to_string(BEFORE));
            line = std::regex_replace(line, std::regex("CURRENT"), std::to_string(CURRENT));
            line = std::regex_replace(line, std::regex("AFTER"), std::to_string(AFTER));
            line = std::regex_replace(line, std::regex("PC"), std::to_string(static_cast<int>(SpecialDirection::PC)));
            line = std::regex_replace(line, std::regex("MEMBANK"), std::to_string(static_cast<int>(SpecialDirection::MEMBANK)));

            output << line << std::endl;
        }
    }

    std::tuple<Ast, SettingMap> build_ast(std::istream& input)
    {
        cpu_assert(input, "Invalid input");

        // Regex
        std::regex find_setting(R"(\.(\w+) ([\d, ]*)\s*(?:;.*)?)");
        std::regex find_block(R"((\d+):\s*(?:;.*)?)");
        std::regex find_instruction(R"((\w{3})( -?\d+(?:, ?\d+)*)?\s*(?:;.*)?)");

        // Result
        Ast blocks;
        SettingMap settings;

        // State machine
        auto setting_done = false;
        auto current_block = std::optional<int>();
        std::smatch base_match;

        for (auto [line, line_number] = std::make_pair(std::string(), 1); getline(input, line); line_number++)
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
                std::transform(std::cbegin(args), std::cend(args), std::back_inserter(args_int), [&line_number](auto& arg)
                {
                    auto arg_int = std::stol(arg);
                    cpu_assert(arg_int <= 0xff, "Too large setting at line " << line_number);
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
                std::transform(std::cbegin(args), std::cend(args), std::back_inserter(args_int), [&line_number](auto& arg)
                {
                    auto arg_int = std::stol(arg);
                    cpu_assert(arg_int < 0xff, "Too large argument at line " << line_number);
                    assert(arg_int >= 0);
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
                cpu_assert(false, "Unrecognized expression at line " << line_number);
            }
        }

        return {blocks, settings};
    }

    void assemble(const Ast& ast, const SettingMap& setting_map, std::ostream& output)
    {
        // Settings
        try
        {
            auto settings = Settings::from_ast(setting_map);
            {
                cereal::BinaryOutputArchive oarchive(output);

                oarchive(settings);
            }
        }
        catch (const std::out_of_range&)
        {
            cpu_assert(false, "Mandatory setting missing");
        }

        // Instructions
        // TODO: Remove core requirement
        Core core;
        auto& factory = core.get_factory();

        for (auto& [bank_id, instructions]: ast)
        {
            cpu_assert(bank_id <= 0xff, "This implementation supports a maximum of 256 memory banks");
            output.put(static_cast<uint8_t>(bank_id));

            cpu_assert(instructions.size() <= 0xff, "This implementation supports only a maximum of 256 instructions per bank");
            output.put(static_cast<uint8_t>(instructions.size()));

            auto instruction_line = 1;
            for (auto& instruction_ast: instructions)
            {
                try
                {
                    auto instruction = factory.create(instruction_ast);
                    auto instruction_raw = factory.dump(*instruction);
                    output.put(instruction_raw);
                }
                catch(CpuException& exception)
                {
                    exception.add_line_infos(bank_id, instruction_line);
                    throw;
                }

                instruction_line++;
            }
        }
    }

    Cpu load_binary(std::istream& input)
    {
        // Settings
        Settings settings;
        {
            cereal::BinaryInputArchive iarchive(input);

            iarchive(settings);
        }

        auto memory = Memory(settings);

        // Instructions
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
            cpu_assert(memory.banks_size() >= membank_len,
                    "Using " << static_cast<int>(membank_len) << " instructions out of a maximum of "
                             << static_cast<int>(memory.banks_size()) << " in membank "
                             << static_cast<int>(membank_id));

            cpu_assert(membank_id < memory.banks_number(), "More membank than specified in settings");
            input.read(reinterpret_cast<char*>(memory.at(membank_id).data()), membank_len);
            assert(input);
        }

        // TODO: How to handle arguments:
        // 1. By restarting the CPU
        //   1.a. Additionally return a vector of pointers to the memory cell having the arguments
        //   1.b. Somehow store this vector inside Cpu and add a set_argument method
        // 2. At runtime
        //   2.a.
        return Cpu(settings, std::move(memory));
    }
}
