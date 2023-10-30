#include "Cell.hpp"

int Precedence(std::string op) {
	if(op == "+" || op == "-")
	    return 1;
	if(op == "*"||op == "/")
	    return 2;
	return 0;
}

bool IsOperator(const std::string& ch) {
    return (ch == "-" || ch == "+" || ch == "*" || ch == "/");
}

Cell::Cell() : value_("0"){};

Cell::Cell(const std::string& value) {
    value_ = value;
    if (value.empty())
        value_ = "0";
}

// int Cell::Value() {
//     return 10;
// }

std::string Cell::Str() const {
    return value_;
}

bool Cell::IsNum() const {
    return std::all_of(value_.begin(), value_.end(), ::isdigit);
}
bool Cell::IsExpression() const {
    return value_[0] == '=';
}

bool Cell::IsHref() const {
    std::string dg = "", str = "";
    size_t i = 0;
    while (isalpha(value_[i])) {
        str += value_[i];
        i++;
    }
    while (isdigit(value_[i])) {
        dg += value_[i];
        i++;
    }
    return value_ == (str+dg) && !dg.empty() && !str.empty();
}

bool Cell::IsValid() const {
    return !(value_ == "Invalid Value");
}

std::vector<std::string> Cell::SplitExp() const {
    std::vector<std::string> result;
    size_t start = 1;
    size_t found;

    while ((found = value_.find_first_of("+-*/=", start)) != std::string::npos) {
        if (found > start) {
            result.push_back(value_.substr(start, found - start));
        }
        result.push_back(std::string(1, value_[found]));  // Add the operator
        start = found + 1;
    }

    if (start < value_.size()) {
        result.push_back(value_.substr(start));
    }
    std::vector<std::string> tokens;

    size_t i = 0;
    if (!result.empty() && IsOperator(result[0])) {
        tokens.push_back("0");
        tokens.push_back(result[0]);
        i = 1;
    }

    for (;i < result.size(); ++i) {
        if (IsOperator(result[i]) && IsOperator(result[i + 1])) {
            tokens.push_back(result[i]);
            tokens.push_back("0");
            tokens.push_back(result[i + 1]);
            i++;
        } else {
            tokens.push_back(result[i]);
        }
    }

    return tokens;
}

// - 34 + 5 + - 2 + - 4