#include <assembler.h>
#include <direction.h>


using namespace std::chrono_literals;

int main(int argc, char* argv[])
{
    assert(argc == 2);

    auto assembly_input = std::ifstream(argv[1], std::ios::binary);
    auto [ast, settings] = Assembler::build_ast(assembly_input);

    {
        auto output = std::fstream("multiplication.bin", std::ios::binary | std::ios::out);

        Assembler::assemble(ast, settings, output);
    }

    auto binary_input = std::fstream("multiplication.bin", std::ios::binary | std::ios::in);

    auto cpu = Assembler::load_binary(binary_input);
    auto answer = static_cast<int>(cpu.start());
    std::cout << "answer: " << answer << std::endl;
}
