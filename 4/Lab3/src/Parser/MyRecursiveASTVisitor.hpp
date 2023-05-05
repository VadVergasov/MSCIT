#pragma once

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>

#include <iostream>
#include <unordered_map>
#include <unordered_set>

struct record {
    enum class Type { INPUT, MODIFY, CONTROL, JUNK };

    Type type;
    std::string name;
    std::set<std::pair<clang::SourceLocation, clang::SourceLocation>> locations;
};

class MyRecursiveASTVisitor
    : public clang::RecursiveASTVisitor<MyRecursiveASTVisitor> {
   public:
    inline static std::unordered_map<clang::Stmt **, record> variables;
    inline static std::vector<std::string> sourcepathlist;
    bool isControl = false;
    using Type = record::Type;

   private:
    clang::ASTContext *p_astContext;

   public:
    MyRecursiveASTVisitor(clang::ASTContext *p_astContext)
        : p_astContext(p_astContext) {
        variables.clear();
    }

    /**
     * Override the TraverseDecl method to avoid traversing declarations from
     * included files.
     */
    bool TraverseDecl(clang::Decl *p_decl) {
        bool isdeclinsource = clang::isa<clang::TranslationUnitDecl>(p_decl) ||
                              p_astContext->getSourceManager().isInMainFile(
                                  p_decl->getLocation());

        if (isdeclinsource) {
            clang::RecursiveASTVisitor<MyRecursiveASTVisitor>::TraverseDecl(
                p_decl);
        }
        return true;
    }

    /**
     * Variable declaration.
     */
    bool VisitVarDecl(clang::VarDecl *p_decl) {
        std::cout << "VarDecl " << p_decl->getNameAsString()
                  << " init address: " << p_decl->getInitAddress() << std::endl;
        variables[p_decl->getInitAddress()] = {
            Type::JUNK,
            p_decl->getNameAsString(),
            {{p_decl->getBeginLoc(), p_decl->getEndLoc()}}};
        return true;
    }

    bool VisitDeclRefExpr(clang::DeclRefExpr *p_decl) {
        if (clang::VarDecl *vd =
                clang::dyn_cast<clang::VarDecl>(p_decl->getDecl())) {
            if (p_decl->getDecl()->getType().getAsString().substr(0, 13) ==
                    "std::istream" ||
                p_decl->getDecl()->getType().getAsString().substr(0, 13) ==
                    "std::ostream") {
                return true;
            }
            std::cout << "DeclRefExpr init address: " << vd->getInitAddress()
                      << std::endl;
            if (!isControl &&
                variables[vd->getInitAddress()].type == Type::JUNK) {
                variables[vd->getInitAddress()].type = Type::MODIFY;
            } else if (isControl) {
                variables[vd->getInitAddress()].type = Type::CONTROL;
            }
            variables[vd->getInitAddress()].locations.insert(
                {p_decl->getBeginLoc(), p_decl->getEndLoc()});
        } else {
            std::cout << "Something not parsable: "
                      << p_decl->getDecl()->getType().getAsString()
                      << " Is a CXXOperatorCallExpr "
                      << clang::isa<clang::CXXOperatorCallExpr>(p_decl)
                      << std::endl;
            if (p_decl->getDecl()->getType().getAsString().substr(0, 43) ==
                "std::basic_istream<char>::__istream_type &") {
            }
        }
        return true;
    }

    bool VisitCXXOperatorCallExpr(clang::CXXOperatorCallExpr *p_expr) {
        for (size_t i = 0; i < p_expr->getNumArgs(); i++) {
            if (clang::DeclRefExpr *dr =
                    clang::dyn_cast<clang::DeclRefExpr>(p_expr->getArgs()[i])) {
                if (clang::VarDecl *vd =
                        clang::dyn_cast_or_null<clang::VarDecl>(
                            dr->getDecl())) {
                    std::cout << "New arg: " << std::endl;
                    p_expr->getArgs()[i]->dumpPretty(*p_astContext);
                    std::cout << std::endl;
                    if (variables.find(vd->getInitAddress()) !=
                        variables.end()) {
                        variables[vd->getInitAddress()].type = Type::INPUT;
                    }
                }
            }
        }
        return true;
    }

    /**
     * CONTROL STATEMENTS.
     */

    /**
     * If
     */
    bool VisitIfStmt(clang::IfStmt *p_stmt) {
        std::cout << "If cond: " << std::endl;
        p_stmt->getCond()->dumpPretty(*p_astContext);
        std::cout << std::endl;
        isControl = true;
        TraverseStmt(p_stmt->getCond());
        isControl = false;
        return true;
    }

    /**
     * Switch
     */
    bool VisitSwitchStmt(clang::SwitchStmt *s_stmt) {
        std::cout << "Switch cond: " << std::endl;
        s_stmt->getCond()->dumpPretty(*p_astContext);
        std::cout << std::endl;
        isControl = true;
        TraverseStmt(s_stmt->getCond());
        isControl = false;
        return true;
    }

    /**
     * For
     */
    bool VisitForStmt(clang::ForStmt *p_stmt) {
        std::cout << "For cond: " << std::endl;
        p_stmt->getCond()->dumpPretty(*p_astContext);
        std::cout << std::endl;
        isControl = true;
        TraverseStmt(p_stmt->getCond());
        isControl = false;
        return true;
    }

    /**
     * While
     */
    bool VisitWhileStmt(clang::WhileStmt *p_stmt) {
        std::cout << "While cond: " << std::endl;
        p_stmt->getCond()->dumpPretty(*p_astContext);
        std::cout << std::endl;
        isControl = true;
        TraverseStmt(p_stmt->getCond());
        isControl = false;
        return true;
    }

    /**
     * Do while
     */
    bool VisitDoStmt(clang::DoStmt *p_stmt) {
        std::cout << "Do cond: " << std::endl;
        p_stmt->getCond()->dumpPretty(*p_astContext);
        std::cout << std::endl;
        isControl = true;
        TraverseStmt(p_stmt->getCond());
        isControl = false;
        return true;
    }
};
