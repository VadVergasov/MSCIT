#pragma once

#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/CommandLine.h>

#include <string>
#include <utility>
#include <vector>

#include "MetricsCalculatorAction.hpp"

class ParserHalstead {
    llvm::cl::OptionCategory myToolCategory{"my-tool options"};

   public:
    std::tuple<std::vector<std::pair<std::string, int>>,
               std::vector<std::pair<std::string, int>>, int, int, int, int, int,
               int, int, int, int>
    parse(int argc, char const *argv[]);
};