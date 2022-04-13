#pragma once

#include "FormulaLexer.h"
#include "common.h"

#include <forward_list>
#include <functional>
#include <stdexcept>


namespace ASTImpl {
class Expr;
}

class ParsingError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

template <typename T>
std::optional<T> SafeAdd(T a, T b) {
    double MAX_VALUE = std::numeric_limits<T>::max();
    double MIN_VALUE = - MAX_VALUE;
    if (a < 0 && b < 0) {
        if (a < MIN_VALUE - b) {
            return {};
        } else {
            return a + b;
        }
    } else if (a > 0 && b > 0){
        if (a >= MAX_VALUE - b) {
            return {};
        } else {
            return a + b;
        }
    } else {
        return a + b;
    }
}

template <typename T>
std::optional<T> SafeMultyplicate(T a, T b) {
    double MAX_VALUE = std::numeric_limits<T>::max();
    double MIN_VALUE = - MAX_VALUE;
    if (b == 0 || a == 0) {
        return 0.0;
    }
    if ((a < 0 && b < 0) || (a > 0 && b > 0)) {
        if (a >= MAX_VALUE / b) {
            return {};
        } else {
            return a * b;
        }
    } else {
        if (a < MIN_VALUE / b) {
            return {};
        } else {
            return a * b;
        }
    }
}

template <typename T>
std::optional<T> SafeDivide(T a, T b) {
    if (abs(b - 0) < 1e-6) {
        return {};
    } else {
        return a / b;
    }
}



class FormulaAST {
public:
    explicit FormulaAST(std::unique_ptr<ASTImpl::Expr> root_expr,
                        std::forward_list<Position> cells);
    FormulaAST(FormulaAST&&) = default;
    FormulaAST& operator=(FormulaAST&&) = default;
    ~FormulaAST();

    double Execute(std::function<std::variant<double, FormulaError>(Position)> func) const;
    void PrintCells(std::ostream& out) const;
    void Print(std::ostream& out) const;
    void PrintFormula(std::ostream& out) const;

    std::forward_list<Position>& GetCells() {
        return cells_;
    }

    const std::forward_list<Position>& GetCells() const {
        return cells_;
    }

private:
    std::unique_ptr<ASTImpl::Expr> root_expr_;

    // physically stores cells so that they can be
    // efficiently traversed without going through
    // the whole AST
    std::forward_list<Position> cells_;
};

FormulaAST ParseFormulaAST(std::istream& in);
FormulaAST ParseFormulaAST(const std::string& in_str);
