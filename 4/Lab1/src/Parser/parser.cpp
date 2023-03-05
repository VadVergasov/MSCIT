#include "parser.h"

std::tuple<std::vector<std::pair<std::string, int>>,
           std::vector<std::pair<std::string, int>>, int, int, int, int, int, int,
           int, int, int>
ParserHalstead::parse(int argc, char const *argv[]) {
    clang::tooling::CommonOptionsParser optionsParser(argc, argv, myToolCategory);

    clang::tooling::ClangTool tool(optionsParser.getCompilations(),
                                   optionsParser.getSourcePathList());

    MyRecursiveASTVisitor::sourcepathlist = optionsParser.getSourcePathList();

    int res = tool.run(
        clang::tooling::newFrontendActionFactory<MetricsCalculatorAction>().get());

    // Operators
    // std::cout << "Operators:" << std::endl;
    std::vector<std::pair<std::string, int>> operators;
    for (auto const &op : MyRecursiveASTVisitor::operators) {
        // std::cout << '(' << op.first << ", " << op.second << ") ";
        operators.push_back({op.first, op.second});
    }
    // std::cout << std::endl << std::endl;

    // Operands
    // std::cout << "Operands:" << std::endl;
    std::vector<std::pair<std::string, int>> operands;
    for (auto const &op : MyRecursiveASTVisitor::operands) {
        // std::cout << '(' << op.first << ", " << op.second << ") ";
        operands.push_back({op.first, op.second});
    }
    // std::cout << std::endl << std::endl;

    // Metrics
    // std::cout << "Metrics:" << std::endl;

    unsigned int N1, N2, n1, n2, N, n, E;
    float D, V;

    N1 = 0;
    for (auto op : MyRecursiveASTVisitor::operators) {
        N1 += op.second;
    }

    N2 = 0;
    for (auto op : MyRecursiveASTVisitor::operands) {
        N2 += op.second;
    }

    n1 = MyRecursiveASTVisitor::operators.size();
    n2 = MyRecursiveASTVisitor::operands.size();

    N = N1 + N2;
    n = n1 + n2;
    V = N * std::log2f(n);
    D = (n1 / 2.f) * (N2 / (float)n2);
    E = V * D;

    // std::cout << "N1=\t" << N1 << std::endl;
    // std::cout << "N2=\t" << N2 << std::endl;
    // std::cout << "n1=\t" << n1 << std::endl;
    // std::cout << "n2=\t" << n2 << std::endl;
    // std::cout << "N=\t" << N << std::endl;
    // std::cout << "n=\t" << n << std::endl;
    // std::cout << "V=\t" << V << std::endl;
    // std::cout << "D=\t" << D << std::endl;
    // std::cout << "E=\t" << E << std::endl;

    return {operators, operands, N1, N2, n1, n2, N, n, V, D, E};
}
