#pragma once

#include "common.h"
#include "formula.h"

#include <functional>
#include <unordered_set>

class Sheet;

class Cell : public CellInterface {
public:
    Cell(SheetInterface& sheet);
    ~Cell();

    void Set(std::string text);
    void Clear();

    Value GetValue() const override;
    std::string GetText() const override;
    std::vector<Position> GetReferencedCells() const override;

    bool IsReferenced() const;
private:
    class Impl {
    public:
        Impl(SheetInterface& sheet, std::string text) : sheet_(sheet), value_(text) {
        }
        
        Impl(SheetInterface& sheet) : sheet_(sheet) {
        }
        
        virtual ~Impl() = default;
        
        virtual Value GetValue() const = 0;
        
        virtual std::string GetText() const = 0;
        
        virtual std::vector<Position> GetReferencedCells() const {
            return {};
        }
        
    protected:
        SheetInterface& sheet_;
        std::string value_;
    };
    
    class EmptyImpl : public Impl {
    public:
        EmptyImpl(SheetInterface& sheet) : Impl(sheet) {
        }
        
        ~EmptyImpl() = default;
        
        Value GetValue() const override {
            return value_;
        }
        
        std::string GetText() const override {
            return value_;
        }
        
    };
    
    class TextImpl : public Impl {
    public:
        TextImpl(SheetInterface& sheet, std::string text) : Impl(sheet, text) {
        }
        
        ~TextImpl() = default;
        
        Value GetValue() const override {
            if (value_.size() != 0 && value_[0] == '\'') {
                return value_.substr(1);
            } else {
                return value_;
            }
        }
        
        std::string GetText() const override {
            return value_;
        }
        
    };
    
    class FormulaImpl : public Impl {
    public:
        FormulaImpl(SheetInterface& sheet, std::string text) : Impl(sheet, text) {
            formula_ = ParseFormula(text);
        }
        
        ~FormulaImpl() = default;
        
        Value GetValue() const override {
            FormulaInterface::Value value = formula_->Evaluate(sheet_);
            if (std::holds_alternative<double>(value)) {
                return std::get<double>(value);
            } else {
                return std::get<FormulaError>(value);
            }
        }
        
        std::string GetText() const override {
            return '=' + formula_->GetExpression();
        }
        
        std::vector<Position> GetReferencedCells() const override {
            return formula_->GetReferencedCells();
        }
        
    private:
        std::unique_ptr<FormulaInterface> formula_;
    };
    
    enum class UpdateCommand {
        ADD,
        DELETE
    };
    
    SheetInterface& sheet_;
    std::unique_ptr<Impl> impl_;
    std::unordered_set<Cell*> dependent_cells_;
    mutable std::optional<Value> cache_value_;

    bool CheckForCircle(std::unique_ptr<Cell::FormulaImpl>& cell_impl) const;
    bool CheckForCircleImpl(std::unordered_set<CellInterface*>& visited_cells, const CellInterface* cell_ptr) const;
    void CacheReset();
    void UpdateDependencies(UpdateCommand command);
};
