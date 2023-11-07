#ifndef CELL_HPP_
#define CELL_HPP_

#include <string>
#include <stdexcept>
#include <algorithm>
#include <vector>

class Cell{
public:
    Cell();
    Cell(const std::string&);
    
    // int Value();
    std::string Str() const;

    char operator[](const size_t i) const {
        return value_[i];
    }


    Cell operator-() const {
        try {
            int64_t result = -std::stoi(value_);
            return Cell(std::to_string(result));
        } catch (std::invalid_argument&) {
            return Cell("Invalid Value");
        }
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