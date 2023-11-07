#ifndef TABLE_HPP_
#define TABLE_HPP_

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "Cell.hpp"

#define outPrefix "Exp"

class Table{
public:
    Table() = delete;

    Table(const std::string&);
    void Print() const;
    void Export() const;
    void EvaluateTable();

    bool operator==(const Table& other) const {
        return other.table_ == table_ && col_ == other.col_;
    }

    std::vector<Cell> operator[](const size_t i) const {
        try {        
            return table_[i];
        }
        catch(const std::exception& e) {
            std::cerr << e.what() << '\n';
            throw std::out_of_range("Index out of bounds");
        }   
    }

    inline size_t Size() const {
        return table_.size();
    }

private:
    size_t rows_, cols_;
    std::vector<std::vector<Cell>> table_;
    std::map<std::string, size_t> col_;
    std::string filePath_;

    void Import(const std::string&);
    void InitCols(const std::string&);
    void AddRow(const std::string&);

    std::vector<Cell> SplitLine(const std::string&);

    std::string CalculateCell(Cell curr, int64_t i, int64_t j);

    std::string EvaluateExpression(const std::vector<std::string>&);

    std::pair<int64_t, int64_t> GetCell(const std::string&);

    std::map<std::pair<int64_t, int64_t>, int> cyclic_;
};

#endif // TABLE_HPP_