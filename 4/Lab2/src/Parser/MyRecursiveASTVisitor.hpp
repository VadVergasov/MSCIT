#pragma once

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>

class MyRecursiveASTVisitor
    : public clang::RecursiveASTVisitor<MyRecursiveASTVisitor> {
   public:
    inline static unsigned long long operators_count = 0;
    inline static unsigned long long branches_count = 0;
    inline static unsigned long long max_depth = 0;
    inline static std::vector<std::string> sourcepathlist;

   private:
    clang::ASTContext *p_astContext;

   public:
    MyRecursiveASTVisitor(clang::ASTContext *p_astContext)
        : p_astContext(p_astContext) {
        branches_count = 0;
        operators_count = 0;
    }

    /**
     * Overide the TraverseDecl method to avoid traversing declarations from
     * included files.
     */
    bool TraverseDecl(clang::Decl *p_decl) {
        bool isdeclinsource =
            clang::isa<clang::TranslationUnitDecl>(p_decl) ||
            p_astContext->getSourceManager().isInMainFile(p_decl->getLocation());

        if (isdeclinsource) {
            clang::RecursiveASTVisitor<MyRecursiveASTVisitor>::TraverseDecl(p_decl);
        }
        return true;
    }

    /**
     * Variable declaration.
     */
    bool VisitVarDecl(clang::VarDecl *p_decl) {
        // type var = init ?
        if (p_decl->hasInit()) {
            operators_count++;
        }
        return true;
    }

    /**
     * Function calls.
     */
    bool VisitCallExpr(clang::CallExpr *p_expr) {
        operators_count++;
        return true;
    }

    /**
     * OPERATORS
     */

    /**
     * Unary operator.
     */
    bool VisitUnaryOperator(clang::UnaryOperator *p_op) {
        operators_count++;
        return true;
    }

    /**
     * Binary operator.
     */
    bool VisitBinaryOperator(clang::BinaryOperator *p_op) {
        operators_count++;
        return true;
    }

    /**
     * Case statement
     */
    bool VisitCaseStmt(clang::CaseStmt *c_stmt) {
        branches_count++;
        return true;
    }

    /**
     * Default statement
     */
    bool VisitDefaultStmt(clang::DefaultStmt *c_stmt) { return true; }

    /**
     * CONTROL STATEMENTS.
     */

    /**
     * If
     */
    bool VisitIfStmt(clang::IfStmt *p_stmt) {
        branches_count++;
        return true;
    }

    /**
     * Switch
     */
    bool VisitSwitchStmt(clang::SwitchStmt *s_stmt) { return true; }

    /**
     * For
     */
    bool VisitForStmt(clang::ForStmt *p_stmt) { return true; }

    /**
     * While
     */
    bool VisitWhileStmt(clang::WhileStmt *p_stmt) { return true; }

    /**
     * Do while
     */
    bool VisitDoStmt(clang::DoStmt *p_stmt) { return true; }
};
