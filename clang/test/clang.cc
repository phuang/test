#include <stdio.h>
#include <llvm/Support/CommandLine.h>


namespace cl = llvm::cl;

cl::OptionCategory test_category(
    "test options",
    "test related options.");

cl::opt<std::string> build_path(
    "b",
    cl::value_desc("compile_commands.json"),
    cl::desc("Path to the compilation database."),
    cl::cat(test_category),
    cl::Optional);

cl::extrahelp extra(
R"(
Examples:
)");


int main(int argc, char** argv) {
  cl::HideUnrelatedOptions(test_category);
  cl::ParseCommandLineOptions(argc, argv);
  return 0;
}
