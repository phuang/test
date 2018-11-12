
#include "browser_ast_consumer.h"

#include <memory>
#include <clang/AST/ASTContext.h>

#include "browser_ast_visitor.h"

BrowserASTConsumer::BrowserASTConsumer(std::string in_file)
      : in_file_(std::move(in_file)) {}

BrowserASTConsumer:: ~BrowserASTConsumer() = default;

void BrowserASTConsumer::Initialize(clang::ASTContext& ctx) {}

void BrowserASTConsumer::HandleTranslationUnit(clang::ASTContext& ctx) {
  BrowserASTVisitor visitor(in_file_, &ctx.getSourceManager());
  visitor.TraverseDecl(ctx.getTranslationUnitDecl());
}
