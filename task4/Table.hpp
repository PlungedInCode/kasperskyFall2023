#ifndef TABLE_HPP_
#define TABLE_HPP_

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "Cell.hpp"


class Table{
public:
    Table(const std::string&);
    Table(int);
    void Print() const;
    void Export() const;
    void EvaluateTable();

private:
    size_t rows_, cols_;
    std::vector<std::vector<Cell>> table_;
    std::map<std::string, size_t> col_;

    void Import(const std::string&);
    void InitCols(const std::string&);
    void AddRow(const std::string&);

    std::vector<Cell> SplitLine(const std::string&);

    std::string Calculate(Cell& curr, int64_t i, int64_t j);

    std::string clc(const std::vector<std::string>&);

    std::pair<int64_t, int64_t> GetCell(const std::string&);
    // std::pair<size_t, size_t> getIdx(const std::string&);

    // std::map<std::pair<size_t, size_t>, bool> cyclic;
    std::map<std::pair<int64_t, int64_t>, int> cyclic;
};


#endif // TABLE_HPP_