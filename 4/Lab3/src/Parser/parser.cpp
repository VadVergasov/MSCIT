#include "parser.h"

std::unordered_map<clang::Stmt **, record> Parser::parse(int argc,
                                                         char const *argv[]) {
    MyRecursiveASTVisitor::variables.clear();
    clang::tooling::CommonOptionsParser optionsParser(argc, argv,
                                                      myToolCategory);

    clang::tooling::ClangTool tool(optionsParser.getCompilations(),
                                   optionsParser.getSourcePathList());

    MyRecursiveASTVisitor::sourcepathlist = optionsParser.getSourcePathList();

    int res = tool.run(
        clang::tooling::newFrontendActionFactory<MetricsCalculatorAction>()
            .get());

    if (res != 0) {
        throw CompileException(
            (std::string("Compilation error: ") + std::to_string(res)).c_str());
    }

    return MyRecursiveASTVisitor::variables;
}
