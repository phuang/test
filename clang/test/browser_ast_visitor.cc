#include "browser_ast_visitor.h"

#include <iostream>

BrowserASTVisitor::BrowserASTVisitor(std::string in_file,
                                     const clang::SourceManager *source_manager)
    : in_file_(in_file), source_manager_(source_manager) {}

bool BrowserASTVisitor::VisitFunctionDecl(clang::FunctionDecl *d) {
  auto location = d->getLocation();
  auto fixed = source_manager_->getPresumedLoc(location);
  if (!fixed.isValid())
    return true;

  if (in_file_ != fixed.getFilename())
    return true;

  std::string text;
  llvm::raw_string_ostream textString(text);
  std::cout << "function=" << d->getQualifiedNameAsString() << std::endl;
  return true;
}
