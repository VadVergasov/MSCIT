#pragma once

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>

class MyRecursiveASTVisitor
    : public clang::RecursiveASTVisitor<MyRecursiveASTVisitor> {
   public:
    inline static unsigned long long operators_count = 0;
    inline static unsigned long long branches_count = 0;
    inline static unsigned long long max_depth = 0;
    inline static std::vector<std::string> sourcepathlist;
    std::map<std::pair<clang::SourceLocation, clang::SourceLocation>,
             unsigned long long>
        depthes;
    std::set<std::pair<clang::SourceLocation, clang::SourceLocation>> operators;

   private:
    clang::ASTContext *p_astContext;

   public:
    MyRecursiveASTVisitor(clang::ASTContext *p_astContext)
        : p_astContext(p_astContext) {
        branches_count = 0;
        operators_count = 0;
        max_depth = 0;
    }

    /**
     * Override the TraverseDecl method to avoid traversing declarations from
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
            operators.insert({p_decl->getBeginLoc(), p_decl->getEndLoc()});
            operators_count = operators.size();
        }
        return true;
    }

    /**
     * Function calls.
     */
    bool VisitCallExpr(clang::CallExpr *p_expr) {
        operators.insert({p_expr->getBeginLoc(), p_expr->getEndLoc()});
        operators_count = operators.size();
        return true;
    }

    /**
     * OPERATORS
     */

    /**
     * Unary operator.
     */
    bool VisitUnaryOperator(clang::UnaryOperator *p_op) {
        operators.insert({p_op->getBeginLoc(), p_op->getEndLoc()});
        operators_count = operators.size();
        return true;
    }

    /**
     * Binary operator.
     */
    bool VisitBinaryOperator(clang::BinaryOperator *p_op) {
        operators.insert({p_op->getBeginLoc(), p_op->getEndLoc()});
        operators_count = operators.size();
        return true;
    }

    /**
     * CONTROL STATEMENTS.
     */

    /**
     * If
     */
    bool VisitIfStmt(clang::IfStmt *p_stmt) {
        auto current_start = p_stmt->getBeginLoc();
        auto current_end = p_stmt->getEndLoc();
        if (depthes.find({current_start, current_end}) != depthes.end()) {
            return true;
        }
        TraverseStmt(p_stmt->getThen());
        TraverseStmt(p_stmt->getElse());
        depthes[{current_start, current_end}] = 0;
        for (const auto &[f, s] : depthes) {
            const auto &[start, end] = f;
            if (p_astContext->getSourceManager().getFileOffset(start) >
                    p_astContext->getSourceManager().getFileOffset(current_start) &&
                p_astContext->getSourceManager().getFileOffset(
                    end) <=  // if .. else if .. else case
                    p_astContext->getSourceManager().getFileOffset(current_end)) {
                depthes[{current_start, current_end}] =
                    std::max(depthes[{current_start, current_end}], s + 1);
            }
            max_depth =
                std::max({max_depth, s, depthes[{current_start, current_end}]});
        }
        branches_count = depthes.size();
        return true;
    }

    /**
     * Switch
     */
    bool VisitSwitchStmt(clang::SwitchStmt *s_stmt) {
        clang::SwitchCase *p_stmt = s_stmt->getSwitchCaseList();
        size_t count = 0;
        while (p_stmt) {
            if (llvm::dyn_cast<clang::DefaultStmt>(p_stmt) != nullptr) {
                p_stmt = p_stmt->getNextSwitchCase();
                continue;
            }
            auto cstart = p_stmt->getBeginLoc();
            auto cend = p_stmt->getEndLoc();
            if (depthes.find({cstart, cend}) != depthes.end()) {
                branches_count = depthes.size();
                return true;
            }
            TraverseStmt(p_stmt->getSubStmt());
            depthes[{cstart, cend}] = count;
            for (const auto &[f, s] : depthes) {
                const auto &[start, end] = f;
                if (p_astContext->getSourceManager().getFileOffset(start) >
                        p_astContext->getSourceManager().getFileOffset(cstart) &&
                    p_astContext->getSourceManager().getFileOffset(end) <
                        p_astContext->getSourceManager().getFileOffset(cend)) {
                    depthes[{cstart, cend}] =
                        std::max(depthes[{cstart, cend}], s + 1 + count);
                }
                max_depth = std::max({max_depth, s, depthes[{cstart, cend}]});
            }
            p_stmt = p_stmt->getNextSwitchCase();
            count++;
        }
        branches_count = depthes.size();
        return true;
    }

    /**
     * For
     */
    bool VisitForStmt(clang::ForStmt *p_stmt) {
        auto current_start = p_stmt->getBeginLoc();
        auto current_end = p_stmt->getEndLoc();
        if (depthes.find({current_start, current_end}) != depthes.end()) {
            return true;
        }
        TraverseStmt(p_stmt->getBody());
        depthes[{current_start, current_end}] = 0;
        for (const auto &[f, s] : depthes) {
            const auto &[start, end] = f;
            if (p_astContext->getSourceManager().getFileOffset(start) >
                    p_astContext->getSourceManager().getFileOffset(current_start) &&
                p_astContext->getSourceManager().getFileOffset(end) <
                    p_astContext->getSourceManager().getFileOffset(current_end)) {
                depthes[{current_start, current_end}] =
                    std::max(depthes[{current_start, current_end}], s + 1);
            }
            max_depth =
                std::max({max_depth, s, depthes[{current_start, current_end}]});
        }
        branches_count = depthes.size();
        return true;
    }

    /**
     * While
     */
    bool VisitWhileStmt(clang::WhileStmt *p_stmt) {
        auto current_start = p_stmt->getBeginLoc();
        auto current_end = p_stmt->getEndLoc();
        if (depthes.find({current_start, current_end}) != depthes.end()) {
            return true;
        }
        TraverseStmt(p_stmt->getBody());
        depthes[{current_start, current_end}] = 0;
        for (const auto &[f, s] : depthes) {
            const auto &[start, end] = f;
            if (p_astContext->getSourceManager().getFileOffset(start) >
                    p_astContext->getSourceManager().getFileOffset(current_start) &&
                p_astContext->getSourceManager().getFileOffset(end) <
                    p_astContext->getSourceManager().getFileOffset(current_end)) {
                depthes[{current_start, current_end}] =
                    std::max(depthes[{current_start, current_end}], s + 1);
            }
            max_depth =
                std::max({max_depth, s, depthes[{current_start, current_end}]});
        }
        branches_count = depthes.size();
        return true;
    }

    /**
     * Do while
     */
    bool VisitDoStmt(clang::DoStmt *p_stmt) {
        auto current_start = p_stmt->getBeginLoc();
        auto current_end = p_stmt->getEndLoc();
        if (depthes.find({current_start, current_end}) != depthes.end()) {
            return true;
        }
        TraverseStmt(p_stmt->getBody());
        depthes[{current_start, current_end}] = 0;
        for (const auto &[f, s] : depthes) {
            const auto &[start, end] = f;
            if (p_astContext->getSourceManager().getFileOffset(start) >
                    p_astContext->getSourceManager().getFileOffset(current_start) &&
                p_astContext->getSourceManager().getFileOffset(end) <
                    p_astContext->getSourceManager().getFileOffset(current_end)) {
                depthes[{current_start, current_end}] =
                    std::max(depthes[{current_start, current_end}], s + 1);
            }
            max_depth =
                std::max({max_depth, s, depthes[{current_start, current_end}]});
        }
        branches_count = depthes.size();
        return true;
    }
};
