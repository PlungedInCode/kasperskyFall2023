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

Cell::Cell(const std::string& value) : value_(value.empty() ? "0" : value) {}

std::string Cell::Str() const {
    return value_;
}

// check negative nums too
bool Cell::IsNum() const {
    if (value_[0] == '-') 
        return std::all_of(value_.begin() + 1, value_.end(), ::isdigit);
    return std::all_of(value_.begin(), value_.end(), ::isdigit);
}
bool Cell::IsExpression() const {
    return value_[0] == '=';
}

bool Cell::IsHref() const {
    std::string zn = "", dg = "", str = "";
    size_t i = 0;
    if (value_[0] == '-') {
        zn += '-';
        i++;
    }
    while (i < value_.size() && isalpha(value_[i])) {
        str += value_[i];
        i++;
    }
    while (i < value_.size() && isdigit(value_[i])) {
        dg += value_[i];
        i++;
    }
    return value_ == (zn + str + dg) && !dg.empty() && !str.empty();
}

bool Cell::IsValid() const {
    return !(value_ == INVL);
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
    if (result.size() > 1 && IsOperator(result[0])) {
        result[1] = result[0] + result[1];
        i++;
    }
    for (; i < result.size(); ++i) {
        if (!IsOperator(result[i])) {
            tokens.push_back(result[i]);
        } else if (i < result.size() - 2 && IsOperator(result[i]) && IsOperator(result[i + 1])) {
            tokens.push_back(result[i]);
            result[i + 2] = result[i + 1] + result[i + 2];
            i++;
        } else {
            tokens.push_back(result[i]);
        }
    }
    return tokens;
}

