#include "Table.hpp"
#include <stack>

std::string ApplyOp(const std::string& lhs, const std::string& rhs, const std::string& op) {
    int64_t result = 0;
    try {
        int64_t left = std::stoi(lhs);
        int64_t right =std::stoi(rhs);
        if (op == "+") {
            result = left + right;
        } else if (op == "-") {
            result = left - right;
        } else if (op == "*") {
            result = left * right;
        } else if (op == "/") {
            if (right != 0) {
                result = left / right;
            } else {
                return "Division by zero";
            }
        } else {
            return "Invalid operator";
        }

    } catch(const std::exception& e) {
       return "Invalid Value";
    }
    return std::to_string(result);   
}

// /*---------------------------------------------------------------*/

Table::Table(int n) : table_(n, std::vector<Cell> (n)) {

}

Table::Table(const std::string& filename) {
    Import(filename);
}


void Table::Print() const {
    for (const auto& col : col_) {
        std::cout << col.first << ' ';
    }
    std::cout << '\n';
    for (auto col : table_) {
        for (auto el : col) {
            std::cout << el.Str() << '|';
        }
        std::cout << '\n';
    }
}

std::vector<Cell> Table::SplitLine(const std::string& line) {
    std::vector<Cell> values;
    std::string curr = "";
    for (const auto& ch : line) {
        if (ch == ' ')
            continue;
        else if (ch == ',') {
            values.push_back(curr);
            curr = "";
        } else
            curr += ch;
    }

    if (!curr.empty()) 
        values.push_back(curr);

    return values;
}

void Table::InitCols(const std::string& line) {
    std::vector<Cell> col = SplitLine(line);
    cols_ = col.size();

    for (size_t i = 0; i < cols_; ++i) {
        col_[col[i].Str()] = i;
    }

    rows_ = 0;
}

void Table::AddRow(const std::string& line) {
    std::vector<Cell> row = SplitLine(line);
    row.resize(cols_);
    table_.push_back(row);
    rows_++;
}


void Table::Import(const std::string& filename) {
    std::ifstream fileStream(filename);
    if (fileStream.is_open()) {
        std::string line;
        std::getline(fileStream, line);
        InitCols(line);
        while (std::getline(fileStream, line)) {
            AddRow(line);
        }
    } else {
        std::cerr << "Failed to open the file for reading\n";
    }
}

int l;

void Table::EvaluateTable() {
    for (size_t i = 0; i < rows_; ++i) {
        for (size_t j = 0; j < cols_; ++j) {
            // cyclic[{i, j}] = true;
            l = -1;
            table_[i][j] = Cell(Calculate(table_[i][j], i, j));
            cyclic.clear();
        }
    }
}
void displayRecursion() {
    for (int i = 0; i < l; i++) {
        std::cout << "    "; // Add indentation based on depth
    }
}


std::string Table::Calculate(Cell& curr, int64_t i = -1, int64_t j = -1) {
    l++;
    // displayRecursion();
    // std::cout << "Entering " << curr.Str() << '\n';

    if (curr.IsExpression()) {
        std::vector<std::string> tokens = curr.SplitExp();
        curr = clc(tokens);
    } else if (!curr.IsValid()) {
        curr = Cell("Invalid Value");
    } else if (curr.IsHref()) {
        std::pair<int64_t, int64_t> idx = GetCell(curr.Str());
        if (idx.first < 0 || idx.second < 0 || cyclic[idx] == 1) {
            curr = Cell("Invalid Value");
        } else {
            curr = table_[idx.first][idx.second];
            cyclic[idx] = 1;
        }
        i = idx.first;
        j = idx.second;
        curr = Calculate(curr, idx.first, idx.second);
    }
    
    if (i >= 0 && j >= 0 && (int64_t)rows_ > i && (int64_t)cols_ > j) {
        cyclic[{i,j}] = 2;
        table_[i][j] = curr.Str();
    }
    // displayRecursion();
    l--;
    // std::cout << "Exiting "<< curr.Str() << '\n';

    return curr.Str();
}

std::string Table::clc(const std::vector<std::string>& tokens) {
    std::stack<std::string> values;
    std::stack<std::string> ops;
    for (size_t i = 0; i < tokens.size(); ++i) {
        // std::cout << tokens[i] << ' ' << IsOperator(tokens[i]) << '\n';
        if (IsOperator(tokens[i])) {
            while (!ops.empty() && Precedence(ops.top()) >= Precedence(tokens[i])) {
                std::string val2 = values.top();
                values.pop();

                std::string val1 = values.top();
                values.pop();

                std::string op = ops.top();
                ops.pop();

                values.push(ApplyOp(val1, val2, op));
            }
            ops.push(tokens[i]);
        } else {
            Cell temp = Cell(tokens[i]);

            std::pair<int64_t, int64_t> idx = {-1, -1};
            if (temp.IsHref()) {
                idx = GetCell(temp.Str());
                if (idx.first == -1 || idx.second == -1) {
                    temp = Cell("Invalid Value");
                }
            } 

            values.push(Calculate(temp, idx.first, idx.second));
        }
    }
    while (!ops.empty()) {
        std::string val2 = values.top();
        values.pop();
        std::string val1 = values.top();
        values.pop();
        std::string op = ops.top();
        ops.pop();
        values.push(ApplyOp(val1, val2, op));
    }
    // Print(); 
    return values.top();
}

std::pair<int64_t, int64_t> Table::GetCell(const std::string& input) {
    std::string dg = "", str = "";
    size_t i = 0, j;
    while (isalpha(input[i])) {
        str += input[i];
        i++;
    }
     while (isdigit(input[i])) {
        dg += input[i];
        i++;
    }

    i = std::stoi(dg) - 1;
    if (str + dg != input || col_.find(str) == col_.end() || i >= rows_ ) {
       return {-1, -1};
    }
    j = col_.at(str);
    return {i, j};
}