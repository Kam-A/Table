#pragma once

#include "cell.h"
#include "common.h"

#include <functional>
#include <vector>

class Sheet : public SheetInterface {
public:
    ~Sheet();

    void SetCell(Position pos, std::string text) override;
    const CellInterface* GetCell(Position pos) const override;
    CellInterface* GetCell(Position pos) override;
    void ClearCell(Position pos) override;
    Size GetPrintableSize() const override;
    void PrintValues(std::ostream& output) const override;
    void PrintTexts(std::ostream& output) const override;
    bool IsPositionInSheet(Position pos) const;
    
private:
    
    std::vector<std::vector<std::unique_ptr<Cell>>> data_;
    Size data_size_{0, 0};
    
    bool IsRowsEnought(Position pos) const;
    bool IsColsEnought(Position pos) const;
    bool IsLastRowEmpty() const;
    bool IsLastColumnEmpty() const;
    void AddNeededRow(int row_number);
    void AddNeededCol(int col_number);
    void CheckSizeAndChange();
};
