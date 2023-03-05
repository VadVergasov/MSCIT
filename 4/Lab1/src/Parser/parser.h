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

class ParserHalstead {
    llvm::cl::OptionCategory myToolCategory{"my-tool options"};

   public:
    std::tuple<std::vector<std::pair<std::string, int>>,
               std::vector<std::pair<std::string, int>>, int, int, int, int, int,
               int, double>
    parse(int argc, char const* argv[]);
};