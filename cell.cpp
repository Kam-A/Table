#include "cell.h"

#include <cassert>
#include <iostream>
#include <string>
#include <optional>
#include <unordered_set>

// Реализуйте следующие методы
Cell::Cell(SheetInterface& sheet) : sheet_(sheet) {
    impl_ = std::make_unique<EmptyImpl>(sheet_);
}

Cell::~Cell() {}

bool Cell::CheckForCircleImpl(std::unordered_set<CellInterface*>& visited_cells, const CellInterface* cell_ptr) const {
    if (!cell_ptr) {
        return false;
    }
    if (cell_ptr == this) {
        return true;
    }
    for (const auto& cell_pos : cell_ptr->GetReferencedCells()) {
        if (visited_cells.count(sheet_.GetCell(cell_pos)) != 0) {
            continue;
        }
        if (CheckForCircleImpl(visited_cells, sheet_.GetCell(cell_pos))) {
            return true;
        }
        visited_cells.insert(sheet_.GetCell(cell_pos));
    }
    return false;
    
}
bool Cell::CheckForCircle(std::unique_ptr<FormulaImpl>& cell_impl) const {
    std::unordered_set<CellInterface*> visited_cells;
    for(auto& cell_pos : cell_impl->GetReferencedCells()) {
        if (CheckForCircleImpl(visited_cells, sheet_.GetCell(cell_pos))) {
            return true;
        }
        visited_cells.insert(sheet_.GetCell(cell_pos));
    }
    return false;
}

void Cell::CacheReset() {
    cache_value_.reset();
    for(Cell* cell_ptr : dependent_cells_) {
        cell_ptr->CacheReset();
    }
}

void Cell::UpdateDependencies(UpdateCommand command) {
    for(auto cell_pos : GetReferencedCells()) {
        CellInterface* cell_ptr = sheet_.GetCell(cell_pos);
        if (cell_ptr) {
            if (command == UpdateCommand::DELETE) {
                static_cast<Cell*>(cell_ptr)->dependent_cells_.erase(this);
            } else {
                static_cast<Cell*>(cell_ptr)->dependent_cells_.insert(this);
            }
        }
    }
}

void Cell::Set(std::string text) {
    if (text.size() != 0 && text[0] == '=' && text.size() != 1) {
        auto new_content_ptr = std::make_unique<FormulaImpl>(sheet_, text.substr(1));
        if (CheckForCircle(new_content_ptr)) {
            throw CircularDependencyException("Circular");
        }
        CacheReset();
        UpdateDependencies(UpdateCommand::DELETE);
        impl_ = std::move(new_content_ptr);
        UpdateDependencies(UpdateCommand::ADD);
    } else if (text.size() == 0) {
        CacheReset();
        UpdateDependencies(UpdateCommand::DELETE);
        impl_ = std::make_unique<EmptyImpl>(sheet_);
    } else {
        CacheReset();
        UpdateDependencies(UpdateCommand::DELETE);
        impl_ = std::make_unique<TextImpl>(sheet_, text);
    }
}

void Cell::Clear() {
    Set("");
}

Cell::Value Cell::GetValue() const {
    if (!cache_value_) {
        cache_value_ = impl_->GetValue();
    }
    return cache_value_.value();
}

std::string Cell::GetText() const {
    return impl_->GetText();
}

std::vector<Position> Cell::GetReferencedCells() const {
    return impl_->GetReferencedCells();
}

bool Cell::IsReferenced() const {
    return dependent_cells_.size() != 0;
}
