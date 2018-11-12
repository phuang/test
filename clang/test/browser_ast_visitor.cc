#include "browser_ast_visitor.h"

#include <iostream>

BrowserASTVisitor::BrowserASTVisitor(std::string in_file,
                                     const clang::SourceManager *source_manager)
    : in_file_(in_file), source_manager_(source_manager) {}

bool BrowserASTVisitor::VisitTypedefNameDecl(clang::TypedefNameDecl *d) {
  if (IsInFile(d->getLocation())) {
    std::string text;
    llvm::raw_string_ostream textString(text);
    std::cout << "TypedefName=" << d->getQualifiedNameAsString() << std::endl;
  }
  return true;
}

bool BrowserASTVisitor::VisitFunctionDecl(clang::FunctionDecl *d) {
  if (IsInFile(d->getLocation())) {
    std::string text;
    llvm::raw_string_ostream textString(text);
    std::cout << "Function=" << d->getQualifiedNameAsString() << std::endl;
  }
  return true;
}

bool BrowserASTVisitor::VisitMemberExpr(clang::MemberExpr *e) {
#if 0
  if (IsInFile(e->getMemberLoc()) {
    std::string text;
    llvm::raw_string_ostream textString(text);
    std::cout << "MumberExpr=" << e->getQualifiedNameAsString() << std::endl;
  }
  std::cout << "MumberExpr=" << e->getQualifiedNameAsString() << std::endl;
#endif
  return true;
}

bool BrowserASTVisitor::IsInFile(clang::SourceLocation location) {
  auto fixed = source_manager_->getPresumedLoc(location);
  if (!fixed.isValid())
    return false;

  return in_file_ == fixed.getFilename();
}
