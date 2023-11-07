#include "Table.hpp"
#include <stack>
#include <filesystem>



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
                return DIVZERO;
            }
        } else {
            return INVlOP;
        }

    } catch(const std::exception& e) {
       return INVL;
    }
    return std::to_string(result);   
}

Table::Table(const std::string& filePath) : filePath_(filePath){
    Import(filePath);
}

void Table::Import(const std::string& filePath) {
    std::ifstream fileStream(filePath);
    if (!fileStream) {
        // Check if the file couldn't be opened.
        rows_ = 0;
        cols_ = 0;
        std::cerr << "Failed to open the file for reading: " << filePath  << " " << std::filesystem::current_path() << '\n';
        
        return; // Exit the function
    }
    std::string line;
    std::getline(fileStream, line);
    InitCols(line);
    while (std::getline(fileStream, line)) {
        AddRow(line);
    }
}

void Table::Export() const {
    // Adding outPrefix to filePath_(i.e filename)
    std::string ExportPath = filePath_;
    size_t lastSlashPos = filePath_.rfind('/');
    if (lastSlashPos != std::string::npos) {
        ExportPath.insert(lastSlashPos + 1, outPrefix);
        
    } else {
        ExportPath = outPrefix + ExportPath;
    }

    std::ofstream outFile(ExportPath);
    if (outFile.is_open()) {
        bool flag = true;
        for (const auto& col : col_) {
            if (flag) {
                flag = false;
            } else {
                outFile << ',';
            }
            outFile << col.first;
        }
        outFile << '\n';

        // Write the table data
        for (const auto& row : table_) {
            flag = true;
            for (const auto& cell : row) {
                if (flag) {
                    flag = false;
                } else {
                    outFile << ',';
                }
                outFile << cell.Str();
            }
            outFile << '\n';
        }
        outFile.close();
        std::cout << "Table data has been exported to: " << ExportPath << '\n';
    } else {
        std::cerr << "Failed to open the file for writing\n";
    }
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

void Table::EvaluateTable() {
    for (size_t i = 0; i < rows_; ++i) {
        for (size_t j = 0; j < cols_; ++j) {
           table_[i][j] = Cell(CalculateCell(table_[i][j], i, j));
        }
    }
}

Cell getSigns(Cell a, Cell b) {
    std::string temp = a.Str();
    if (!temp.empty()) {
        if (a.Str()[0] == '-') {
            if (a.Str()[0] == b.Str()[0]) {
                temp.erase(0, 1);
            } else if (a.Str()[0] != b.Str()[0]) {
                temp = '-' + temp;
            }
        } else if (b.Str()[0] == '-') {
            temp = '-' + temp;
        }
    }
    return Cell(temp);
}

std::string Table::CalculateCell(Cell curr, int64_t i = -1, int64_t j = -1) {
    // bool flag = false;
    if (curr.IsExpression()) {
        // If the cell contains an expression, split it into tokens.
        std::vector<std::string> tokens = curr.SplitExp();
        curr = EvaluateExpression(tokens);
        // Note: The recursive evaluation is not explicit in this function but happens in EvaluateExpression.
    } else if (curr.IsHref()) {
        // If the cell is a reference to another cell (IsHref), resolve the reference.
        std::pair<int64_t, int64_t> idx = GetCell(curr.Str());
        if (idx.first < 0 || idx.second < 0 || cyclic_[idx] == 1) {
            // Handle cases where the reference is invalid or cyclic.
            curr = Cell(INVL);
        } else {
            // Recursively evaluate the referenced cell and update the current cell.
            
            curr = getSigns(table_[idx.first][idx.second], curr);
            cyclic_[idx] = 1; // Mark the reference as visited to prevent cycles.
        }
        i = idx.first;
        j = idx.second;
        curr = CalculateCell(curr, idx.first, idx.second); // Recursively CalculateCell the referenced cell.
    }
    
    // Update the cyclic status of the current cell if applicable.
    if (i >= 0 && j >= 0 && (int64_t)rows_ > i && (int64_t)cols_ > j) {
        cyclic_[{i,j}] = 2;
        // if (!flag) table_[i][j] = curr.Str();
    }

    return curr.Str();
}

std::string Table::EvaluateExpression(const std::vector<std::string>& tokens) {
    std::stack<std::string> values;
    std::stack<std::string> ops;
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (IsOperator(tokens[i])) {
            // Process operators and apply precedence rules
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
                // Handle cell references in the expression
                idx = GetCell(tokens[i]);
                if (idx.first == -1 || idx.second == -1) {
                    temp = Cell(INVL);
                }
            } 

            values.push(CalculateCell(temp, idx.first, idx.second));
        }
    }
    // After processing all tokens, apply any remaining operators.
    while (!ops.empty()) {
        std::string val2 = values.top();
        values.pop();
        std::string val1 = values.top();
        values.pop();
        std::string op = ops.top();
        ops.pop();
        values.push(ApplyOp(val1, val2, op));
    }
    return values.top();
}

std::pair<int64_t, int64_t> Table::GetCell(const std::string& input) {
    std::string zn="", dg = "", str = "";
    size_t i = 0, j;
    if (input[0] == '-') {
        zn += '-';
        i++;
    }
    // Extract the column label (letters)
    while (isalpha(input[i])) {
        str += input[i];
        i++;
    }
    // Extract the row label (numbers)
    while (isdigit(input[i])) {
        dg += input[i];
        i++;
    }

    i = std::stoi(dg) - 1;
    if ((zn + str + dg) != input || col_.find(str) == col_.end() || i >= rows_ ) {
       return {-1, -1}; // Indicate an invalid cell reference
    }
    j = col_.at(str);
    return {i, j};
}