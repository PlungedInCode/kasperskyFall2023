#include <iostream>
#include <filesystem>
#include "src/Table.hpp"


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <data.csv>\n";
        return -1;
    }

    std::string filename = argv[1];
    
    if (!std::filesystem::exists(filename)) {
        std::cerr << "File " << filename << " doesn't exisits\n";
        return -2;
    }
    
    Table table(filename);
    table.EvaluateTable();
    table.Print();
    table.Export();
    return 0;
    
}