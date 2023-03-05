#include "parser.h"

std::tuple<std::vector<std::pair<std::string, int>>,
           std::vector<std::pair<std::string, int>>, int, int, int, int, int, int,
           double>
ParserHalstead::parse(int argc, char const *argv[]) {
    MyRecursiveASTVisitor::operators.clear();
    MyRecursiveASTVisitor::operands.clear();

    clang::tooling::CommonOptionsParser optionsParser(argc, argv, myToolCategory);

    clang::tooling::ClangTool tool(optionsParser.getCompilations(),
                                   optionsParser.getSourcePathList());

    MyRecursiveASTVisitor::sourcepathlist = optionsParser.getSourcePathList();

    int res = tool.run(
        clang::tooling::newFrontendActionFactory<MetricsCalculatorAction>().get());

    std::vector<std::pair<std::string, int>> operators;
    for (auto const &op : MyRecursiveASTVisitor::operators) {
        operators.push_back({op.first, op.second});
    }

    std::vector<std::pair<std::string, int>> operands;
    for (auto const &op : MyRecursiveASTVisitor::operands) {
        operands.push_back({op.first, op.second});
    }
    unsigned int N1 = 0, N2 = 0, n1, n2, N, n;
    double V;
    for (auto op : MyRecursiveASTVisitor::operators) {
        N1 += op.second;
    }
    for (auto op : MyRecursiveASTVisitor::operands) {
        N2 += op.second;
    }
    n1 = MyRecursiveASTVisitor::operators.size();
    n2 = MyRecursiveASTVisitor::operands.size();
    N = N1 + N2;
    n = n1 + n2;
    V = N * std::log2f(n);

    return {operators, operands, N1, N2, n1, n2, N, n, V};
}
