#include <iostream>
#include <filesystem>
#include "Table.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <data.csv>\n";
        return -1;
    }

    std::string filename = argv[1];
    // std::string filename = "data.csv";

    // check wheter it's in csv format or not
    if (!std::filesystem::exists(filename)) {
        std::cerr << "File " << filename << " doesn't exisits\n";
        return -2;
    }
    
    Table table(filename);
    table.EvaluateTable();
    table.Print();
    return 0;
}