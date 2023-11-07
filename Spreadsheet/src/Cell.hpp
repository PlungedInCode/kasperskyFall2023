#ifndef CELL_HPP_
#define CELL_HPP_

#include <string>
#include <stdexcept>
#include <algorithm>
#include <vector>

const std::string INVL = "Invalid_Value";
const std::string INVlOP = "Invalid_Operator";
const std::string DIVZERO = "Division_By_Zero";

class Cell{
public:
    Cell();
    Cell(const std::string&);
    
    // int Value();
    std::string Str() const;

    char operator[](const size_t i) const {
        return value_[i];
    }

    bool operator==(const Cell& other) const {
        return other.value_ == value_;
    }
     bool operator !=(const Cell& other) const {
        return other.value_ != value_;
    }


    bool IsNum() const;
    bool IsExpression() const;
    bool IsHref() const;
    bool IsValid() const;

    std::vector<std::string> SplitExp() const;

private:
    std::string value_;
};

int Precedence(std::string op);

bool IsOperator(const std::string& ch);

#endif // CELL_HPP_