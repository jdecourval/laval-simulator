#include <assembler.h>

#include <externals/CLI11.hpp>


using namespace std::chrono_literals;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"
#pragma ide diagnostic ignored "CannotResolve"
int main(int argc, char* argv[])
{
    std::ios_base::sync_with_stdio(false);

    CLI::App app("LAVAL simulator");

    std::string file_path;
    app.add_option("file", file_path, "Assembly or binary program")->required()->check(CLI::ExistingFile);

    std::string args_path;
    app.add_option("ARGUMENTS", args_path, "Arguments file. With no ARGUMENTS, or when ARGUMENTS is -, read standard input");

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
    cpu_assert(file, std::strerror(errno));

    // TODO: Maybe an unique_ptr with custom cleanup function could be used
    std::ostream* binary = nullptr;

    try
    {
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
                cpu_assert(*preprocessed, std::strerror(errno));
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
        cpu_assert(preprocessed_input, std::strerror(errno));
        preprocessed_input->seekg(0);
        auto[ast, settings] = Assembler::parse(*preprocessed_input);


        if (!output_path.empty())
        {
            binary = new std::fstream(output_path, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
            cpu_assert(*binary, std::strerror(errno));
        }
        else
        {
            binary = new std::stringstream;
        }

        Assembler::assemble(ast, settings, *binary);
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
        cpu_assert(binary_input, std::strerror(errno));
        binary_input->seekg(0);
        auto cpu = Assembler::load_binary(*binary_input);

        binary_input->clear();
        std::cerr << "Program size: " << binary_input->tellg() << "\n";

        std::cerr << cpu << std::endl;

        int answer = 0;

        if (args_path.empty() || args_path == "-")
        {
            answer = static_cast<int>(cpu.start(std::cin, std::cout));
        }
        else
        {
            auto args_file = std::ifstream(args_path, std::ios::in);
            cpu_assert(args_file, std::strerror(errno));
            answer = static_cast<int>(cpu.start(args_file, std::cout));
        }

        std::cout << "answer: " << answer << std::endl;
    }
}
    catch (const CpuException& exception)
    {
        std::cerr << exception.what() << std::endl;
    }

    return 0;
}

#pragma clang diagnostic pop
