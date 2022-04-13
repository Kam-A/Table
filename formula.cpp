#include "formula.h"

#include "FormulaAST.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <sstream>

using namespace std::literals;

std::ostream& operator<<(std::ostream& output, FormulaError fe) {
    return output << "#DIV/0!";
}

FormulaInterface::Value SafeStringToDouble(std::string value) {
    if (value.size() == 0) {
        return 0.0;
    }
    try {
        size_t num_of_process = 0;
        auto value_from_str = std::stod(value, &num_of_process);
        if (num_of_process != value.size()) {
            return FormulaError(FormulaError::Category::Value);
        }
        return value_from_str;
    } catch (...) {
        return FormulaError(FormulaError::Category::Value);
    }
}

namespace {
class Formula : public FormulaInterface {
public:
// Реализуйте следующие методы:
    explicit Formula(std::string expression) : ast_(ParseFormulaAST(expression)) {
    }
    
    Value Evaluate(const SheetInterface& sheet) const override {
        try {
            auto cellexpr_func = [&sheet] (Position pos) -> std::variant<double, FormulaError> {
                if (pos < Position{sheet.GetPrintableSize().rows, sheet.GetPrintableSize().cols}) {
                    const CellInterface* cell = sheet.GetCell(pos);
                    CellInterface::Value val = cell->GetValue();
                    if (std::holds_alternative<double>(val)) {
                        return std::get<double>(val);
                    } else if (std::holds_alternative<std::string>(val)) {
                        return SafeStringToDouble(std::get<std::string>(val));
                    } else {
                        return std::get<FormulaError>(val);;
                    }
                } else if (pos.IsValid()) {
                    return 0.0;
                }else {
                    return FormulaError(FormulaError::Category::Ref);
                }
            };
            return ast_.Execute(cellexpr_func);
        } catch (FormulaError& exc) {
            return exc;
        }
    }
    
    std::string GetExpression() const override {
        std::stringstream ss;
        ast_.PrintFormula(ss);
        return ss.str();
    }
    
    std::vector<Position> GetReferencedCells() const override {
        std::vector<Position> result;
        for (Position pos : ast_.GetCells()) {
            if (result.size() == 0) {
                result.push_back(pos);
            } else if (result.back() == pos) {
                continue;
            } else {
                result.push_back(pos);
            }
        }
        return result;
    }
    
private:
    FormulaAST ast_;
};

}  // namespace

std::unique_ptr<FormulaInterface> ParseFormula(std::string expression) {
    try {
        return std::make_unique<Formula>(std::move(expression));
    } catch (...) {
        throw FormulaException("Parsing error");
    }
}
