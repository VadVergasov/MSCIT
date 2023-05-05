#pragma once

#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/CommandLine.h>

#include <string>
#include <utility>
#include <vector>

#include "MetricsCalculatorAction.hpp"

class CompileException : std::runtime_error {
   public:
    CompileException(const char* what) : std::runtime_error(what) {}
};

class Parser {
    llvm::cl::OptionCategory myToolCategory{"my-tool options"};

   public:
    std::unordered_map<clang::Stmt **, record> parse(
        int argc, char const* argv[]);
};