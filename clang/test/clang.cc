#include <stdio.h>
#include <iostream>
#include <memory>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/CommandLine.h>
#include <clang/Tooling/JSONCompilationDatabase.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>

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

class BrowserASTVisitor : public clang::RecursiveASTVisitor<BrowserASTVisitor> {
 public:
  typedef clang::RecursiveASTVisitor<BrowserASTVisitor> Base;

  BrowserASTVisitor() = default;

  bool VisitTypedefNameDecl(clang::TypedefNameDecl *d) {
    std::cout << __func__ << std::endl;
    return true;
  }

  bool VisitTagDecl(clang::TagDecl *d) {
    std::cout << __func__ << std::endl;
    return true;
  }

  bool VisitNamespaceDecl(clang::NamespaceDecl *d) {
    std::cout << __func__ << std::endl;
    return true;
  }

  bool VisitNamespaceAliasDecl(clang::NamespaceAliasDecl *d) {
    std::cout << __func__ << std::endl;
    return true;
  }

  bool VisitFunctionDecl(clang::FunctionDecl *d) {
    std::cout << __func__ << std::endl;
    return true;
  }

  bool VisitEnumConstantDecl(clang::EnumConstantDecl *d) {
    std::cout << __func__ << std::endl;
    return true;
  }

  bool VisitVarDecl(clang::VarDecl *d) {
    std::cout << __func__ << std::endl;
    return true;
  }

  bool VisitFieldDecl(clang::FieldDecl *d) {
    std::cout << __func__ << std::endl;
    return true;
  }

  bool VisitMemberExpr(clang::MemberExpr *e) {
    std::cout << __func__ << std::endl;
    return true;
  }

  bool VisitDeclRefExpr(clang::DeclRefExpr *e) {
    std::cout << __func__ << std::endl;
    return true;
  }

  bool VisitDesignatedInitExpr(clang::DesignatedInitExpr *e) {
    std::cout << __func__ << std::endl;
    return true;
  }

  bool VisitTypedefTypeLoc(clang::TypedefTypeLoc TL) {
    std::cout << __func__ << std::endl;
    return true;
  }

  bool VisitTagTypeLoc(clang::TagTypeLoc TL) {
    std::cout << __func__ << std::endl;
    return true;
  }

  bool VisitTemplateSpecializationTypeLoc(clang::TemplateSpecializationTypeLoc TL) {
    std::cout << __func__ << std::endl;
    return true;
  }

  bool TraverseNestedNameSpecifierLoc(clang::NestedNameSpecifierLoc NNS) {
    std::cout << __func__ << std::endl;
    return Base::TraverseNestedNameSpecifierLoc(NNS);
  }

  bool TraverseUsingDirectiveDecl(clang::UsingDirectiveDecl *d) {
    std::cout << __func__ << std::endl;
    return true;
  }

  bool TraverseConstructorInitializer(clang::CXXCtorInitializer *Init) {
    std::cout << __func__ << std::endl;
    return true;
  }

  bool VisitCXXConstructExpr(clang::CXXConstructExpr *ctr) {
    std::cout << __func__ << std::endl;
    return true;
  }

  bool VisitCallExpr(clang::CallExpr *e) {
    std::cout << __func__ << std::endl;
    return true;
  }
  bool VisitGotoStmt(clang::GotoStmt *stm) {
    std::cout << __func__ << std::endl;
    return true;
  }
  bool VisitLabelStmt(clang::LabelStmt *stm) {
    std::cout << __func__ << std::endl;
    return true;
  }
  bool TraverseDecl(clang::Decl *d) {
    std::cout << __func__ << std::endl;
    if (!d)
      return true;
    Base::TraverseDecl(d);
    return true;
  }
  bool TraverseStmt(clang::Stmt *s) {
    std::cout << __func__ << std::endl;
    return true;
  }
  bool TraverseDeclarationNameInfo(clang::DeclarationNameInfo NameInfo) {
    std::cout << __func__ << std::endl;
    return true;
  }
};

class BrowserASTConsumer : public clang::ASTConsumer {
 public:
  BrowserASTConsumer() = default;
  ~BrowserASTConsumer() override = default;

  void HandleTranslationUnit(clang::ASTContext& ctx) override {
    BrowserASTVisitor visitor;
    visitor.TraverseDecl(ctx.getTranslationUnitDecl());
  }
};

class BrowserAction : public clang::ASTFrontendAction {
 public:
  BrowserAction() = default;
  ~BrowserAction() override = default;

  std::unique_ptr<clang::ASTConsumer>
      CreateASTConsumer(clang::CompilerInstance &CI,
                        llvm::StringRef InFile) override {
        auto consumer = std::make_unique<BrowserASTConsumer>();
        return consumer;
  }
  bool hasCodeCompletionSupport() const override { return true; }
};


void process_file(std::vector<std::string> command, const std::string file) {
  clang::FileManager file_manager({"."});
  file_manager.Retain();
  command = clang::tooling::getClangSyntaxOnlyAdjuster()(command, file);
  command = clang::tooling::getClangStripOutputAdjuster()(command, file);

  clang::tooling::ToolInvocation inv(command, new BrowserAction(), &file_manager);
  bool result = inv.run();

  for (const auto& arg: command) {
    std::cout << arg << " ";
  }
  std::cout << std::endl <<  "result=" << result << std::endl;;
}


int main(int argc, char** argv) {
  cl::HideUnrelatedOptions(test_category);
  cl::ParseCommandLineOptions(argc, argv);

  std::string error_message;
  auto compilations = clang::tooling::CompilationDatabase::loadFromDirectory(
      "..", error_message);
  if (!compilations) {
    std::cerr << error_message << std::endl;
    return EXIT_FAILURE;
  }

  auto files = compilations->getAllFiles();
  for (const auto& file : files) {
    auto command = compilations->getCompileCommands(file);
    process_file(command.front().CommandLine, file);
  }
  return 0;
}
