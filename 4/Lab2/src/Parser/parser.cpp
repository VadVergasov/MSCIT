#include "parser.h"

std::tuple<unsigned long long, double, unsigned long long> ParserJilb::parse(
    int argc, char const *argv[]) {
    clang::tooling::CommonOptionsParser optionsParser(argc, argv, myToolCategory);

    clang::tooling::ClangTool tool(optionsParser.getCompilations(),
                                   optionsParser.getSourcePathList());

    MyRecursiveASTVisitor::sourcepathlist = optionsParser.getSourcePathList();

    int res = tool.run(
        clang::tooling::newFrontendActionFactory<MetricsCalculatorAction>().get());

    if (res != 0) {
        throw CompileException(
            (std::string("Compilation error: ") + std::to_string(res)).c_str());
    }

    return {MyRecursiveASTVisitor::branches_count,
            static_cast<double>(MyRecursiveASTVisitor::branches_count) /
                static_cast<double>(MyRecursiveASTVisitor::operators_count),
            MyRecursiveASTVisitor::max_depth};
}
