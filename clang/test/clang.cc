
#include <iostream>
#include <memory>

#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/JSONCompilationDatabase.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/CommandLine.h>

#include "browser_ast_consumer.h"

namespace cl = llvm::cl;

cl::OptionCategory test_category("test options", "test related options.");

cl::opt<std::string> build_path("b", cl::value_desc("compile_commands.json"),
                                cl::desc("Path to the compilation database."),
                                cl::cat(test_category), cl::Optional);

cl::extrahelp extra(
    R"(
Examples:
)");

class BrowserAction : public clang::ASTFrontendAction {
public:
  BrowserAction() = default;
  ~BrowserAction() override = default;

  std::unique_ptr<clang::ASTConsumer>
  CreateASTConsumer(clang::CompilerInstance &CI,
                    llvm::StringRef in_file) override {
    auto consumer = std::make_unique<BrowserASTConsumer>(in_file);
    return consumer;
  }
  bool hasCodeCompletionSupport() const override { return true; }
};

bool ProcessFile(std::vector<std::string> command, const std::string file) {
  clang::FileManager file_manager({"."});
  file_manager.Retain();
  command = clang::tooling::getClangSyntaxOnlyAdjuster()(command, file);
  command = clang::tooling::getClangStripOutputAdjuster()(command, file);

  auto action = std::make_unique<BrowserAction>();
  clang::tooling::ToolInvocation invocation(command, action.release(),
                                            &file_manager);
  return invocation.run();
}

int main(int argc, char **argv) {
  cl::HideUnrelatedOptions(test_category);
  cl::ParseCommandLineOptions(argc, argv);

  std::string error_message;
  auto compilations = clang::tooling::CompilationDatabase::loadFromDirectory(
      ".", error_message);

  if (!compilations) {
    std::cerr << error_message << std::endl;
    return EXIT_FAILURE;
  }

  auto files = compilations->getAllFiles();
  for (const auto &file : files) {
    auto command = compilations->getCompileCommands(file);
    if (!ProcessFile(command.front().CommandLine, file))
      return EXIT_FAILURE;
  }
  return 0;
}
