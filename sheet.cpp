#include "sheet.h"

#include "cell.h"
#include "common.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>

using namespace std::literals;

Sheet::~Sheet() {}

bool Sheet::IsRowsEnought(Position pos) const {
    return pos.row + 1 <= data_size_.rows;
}

bool Sheet::IsColsEnought(Position pos) const {
    return pos.col + 1 <= data_size_.cols;
}

bool Sheet::IsLastRowEmpty() const {
    if (data_.size() == 0) {
        return false;
    }
    auto it = std::find_if(data_.back().begin(), data_.back().end(), [](const auto& cell_ptr) {
        return cell_ptr->GetText() != "";
    });
    if (it == data_.back().end()) {
        return true;
    } else {
        return false;
    }
}

bool Sheet::IsLastColumnEmpty() const {
    if (data_.size() == 0) {
        return false;
    }
    auto it = std::find_if(data_.begin(), data_.end(),
                        [](const std::vector<std::unique_ptr<Cell>>& cell_ptr_container) {
                            return cell_ptr_container.back()->GetText() != "";
    });
    if (it == data_.end()) {
        return true;
    } else {
        return false;
    }
}

void Sheet::AddNeededRow(int row_number) {
    for (int i = 0; i < row_number; ++i) {
        std::vector<std::unique_ptr<Cell>> new_row;
        new_row.reserve(data_size_.cols);
        std::generate_n(std::back_inserter(new_row), data_size_.cols, [this] { return std::make_unique<Cell>(*this); });
        data_.push_back(std::move(new_row));
    }
}

void Sheet::AddNeededCol(int col_to_add) {
    for(auto& row : data_) {
        std::generate_n(std::back_inserter(row), col_to_add, [this] { return std::make_unique<Cell>(*this); });
    }
}

void Sheet::SetCell(Position pos, std::string text) {
    if (!pos.IsValid()) {
        throw InvalidPositionException("Invalid position");
    }
    if (!IsRowsEnought(pos)) {
        AddNeededRow(pos.row - data_size_.rows + 1);
        data_size_.rows = pos.row + 1;
    }
    if (!IsColsEnought(pos)) {
        AddNeededCol(pos.col - data_size_.cols + 1);
        data_size_.cols = pos.col + 1;
    }
    data_[pos.row][pos.col]->Set(text);
    
}

const CellInterface* Sheet::GetCell(Position pos) const {
    if (!pos.IsValid()) {
        throw InvalidPositionException("Invalid position");
    }
    if (!IsColsEnought(pos) || !IsRowsEnought(pos)) {
        return nullptr;
    }
    return data_[pos.row][pos.col].get();
}

CellInterface* Sheet::GetCell(Position pos) {
    if (!pos.IsValid()) {
        throw InvalidPositionException("Invalid position");
    }
    if (!IsColsEnought(pos) || !IsRowsEnought(pos)) {
        return nullptr;
    }
    return data_[pos.row][pos.col].get();
}

void Sheet::CheckSizeAndChange() {
    while(IsLastRowEmpty()) {
        data_.pop_back();
        --data_size_.rows;
    }
    if (data_size_.rows == 0) {
        data_size_.cols = 0;
        return;
    }
    while(IsLastColumnEmpty()) {
        for(auto& row : data_) {
            row.pop_back();
        }
        --data_size_.cols;
    }
}

bool Sheet::IsPositionInSheet(Position pos) const {
    return IsColsEnought(pos) && IsRowsEnought(pos);
}

void Sheet::ClearCell(Position pos) {
    if (!pos.IsValid()) {
        throw InvalidPositionException("Invalid position");
    }
    if (!IsColsEnought(pos) || !IsRowsEnought(pos)) {
        return;
    }
    data_[pos.row][pos.col]->Clear();
    if (pos.row + 1 == data_size_.rows || pos.col + 1 == data_size_.cols) {
        CheckSizeAndChange();
    }
}

Size Sheet::GetPrintableSize() const {
    return data_size_;
}

void Sheet::PrintValues(std::ostream& output) const {
    for (int i = 0; i < data_size_.rows; ++i) {
        for(int j = 0; j < data_size_.cols; ++j) {
            auto cell_value = data_[i][j]->GetValue();
            if (std::holds_alternative<std::string>(cell_value)) {
                output << std::get<std::string>(cell_value);
            } else if (std::holds_alternative<double>(cell_value)) {
                output << std::get<double>(cell_value);
            } else {
                output << std::get<FormulaError>(cell_value).ToString();
            }
            if (j != data_size_.cols - 1) {
                output << '\t';
            }
        }
        output << '\n';
    }
}

void Sheet::PrintTexts(std::ostream& output) const {
    for (int i = 0; i < data_size_.rows; ++i) {
        for(int j = 0; j < data_size_.cols; ++j) {
            output << data_[i][j]->GetText();
            if (j != data_size_.cols - 1) {
                output << '\t';
            }
        }
        output << '\n';
    }
}

std::unique_ptr<SheetInterface> CreateSheet() {
    return std::make_unique<Sheet>();
}
