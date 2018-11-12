#ifndef __BROWSER_AST_VISITOZR_H_
#define __BROWSER_AST_VISITOZR_H_

// #include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>

namespace clang {
class ASTContext;
class SourceManager;
class FunctionDecl;
}

class BrowserASTVisitor : public clang::RecursiveASTVisitor<BrowserASTVisitor> {
 public:
  typedef clang::RecursiveASTVisitor<BrowserASTVisitor> Base;

  BrowserASTVisitor(std::string in_file,
                    const clang::SourceManager* source_manager);

#if 0
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
#endif

  bool VisitFunctionDecl(clang::FunctionDecl *d);

#if 0
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
#endif

 private:
  std::string in_file_;
  const clang::SourceManager* const source_manager_;
};

#endif  // __BROWSER_AST_VISITOZR_H_
