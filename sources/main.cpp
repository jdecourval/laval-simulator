#include <assembler.h>

#include <externals/CLI11.hpp>


using namespace std::chrono_literals;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"
#pragma ide diagnostic ignored "CannotResolve"
int main(int argc, char* argv[])
{
    CLI::App app("LAVAL simulator");

    std::string file_path;
    app.add_option("file", file_path, "Assembly or binary program")->required()->check(CLI::ExistingFile);

    std::vector<uint8_t> args;
    app.add_option("args", args, "Program arguments");

    auto preprocess = false;
    auto preprocess_opt = app.add_flag("-E,--preprocess", preprocess, "Preprocess only, do not assemble or simulate");

    auto compile = false;
    app.add_flag("-c,--compile", compile, "Assemble only, do not compile or simulate");

    std::string output_path;
    app.add_option("-o,--output ", output_path, "Place the assembled or preprocessed output into <file>");

    auto simulate = false;
    app.add_flag("-s,--simulate ", simulate, "Simulate")->excludes(preprocess_opt);

    CLI11_PARSE(app, argc, argv);

    auto file = std::fstream(file_path, std::ios::binary | std::ios::in);
    assert(file);

    // TODO: Maybe an unique_ptr with custom cleanup function could be used
    std::ostream* binary = nullptr;

    if (preprocess || compile)
    {
        std::ostream* preprocessed = nullptr;

        if (preprocess)
        {

            if (output_path.empty())
            {
                preprocessed = &std::cout;
            }
            else
            {
                preprocessed = new std::fstream(output_path, std::ios::out | std::ios::binary);
            }
        }
        else
        {
            preprocessed = new std::stringstream;
        }

        Assembler::preprocess(file, *preprocessed);

        if (preprocess)
        {
            return 0;
        }


        auto preprocessed_input = dynamic_cast<std::istream*>(preprocessed);
        assert(preprocessed_input);
        preprocessed_input->seekg(0);
        auto [ast, settings, args] = Assembler::build_ast(*preprocessed_input);


        if (!output_path.empty())
        {
            binary = new std::fstream(output_path, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
            assert(*binary);
        }
        else
        {
            binary = new std::stringstream;
        }

        Assembler::assemble(ast, settings, args, *binary);
        binary->flush();
        preprocessed->flush();
    }
    else
    {
        binary = &file;
    }

    if (simulate)
    {
        auto binary_input = dynamic_cast<std::istream*>(binary);
        assert(binary_input);
        binary_input->seekg(0);
        auto cpu = Assembler::load_binary(*binary_input);
//    auto cpu = Assembler::load_binary(binary, args);  // Arguments should be passed like this

        auto answer = static_cast<int>(cpu.start(0s, args));
        std::cout << "answer: " << answer << std::endl;
    }

    return 0;
}

#pragma clang diagnostic pop
