#ifndef __BROwSER_AST_CONSUMER_H_
#define __BROwSER_AST_CONSUMER_H_

#include <clang/AST/ASTConsumer.h>
#include <string>

namespace clang {
class ASTContext;
}

class BrowserASTConsumer : public clang::ASTConsumer {
public:
  BrowserASTConsumer(std::string in_file);
  ~BrowserASTConsumer() override;

  void Initialize(clang::ASTContext &ctx) override;
  void HandleTranslationUnit(clang::ASTContext &ctx) override;

private:
  std::string in_file_;
};

#endif // __BROwSER_AST_CONSUMER_H_
