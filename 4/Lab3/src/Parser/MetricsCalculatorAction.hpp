#pragma once

#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>

#include "MetricsCalculatorASTConsumer.hpp"

class MetricsCalculatorAction : public clang::ASTFrontendAction {
   public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance& compiler, llvm::StringRef inFile) {
        return std::unique_ptr<clang::ASTConsumer>(
            new MetricsCalculatorASTConsumer(&compiler.getASTContext()));
    }
};
