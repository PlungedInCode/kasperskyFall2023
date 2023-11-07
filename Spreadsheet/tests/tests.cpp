#include <gtest/gtest.h>
#include "../src/Table.hpp"
#include "../src/Cell.hpp"

const int TestFiles = 7;
const std::string dataPath = "../../tests/CsvFiles/Data";
const std::string ExpDataPath = "../../tests/CsvFiles/ExpData";

bool IsEqual(const Table& a, const Table& b) {
    if (a.Size() != b.Size() || (a.Size() != 0 && a[0].size() != b[0].size())) {
        return false;
    }
    for (size_t i = 0; i < a.Size(); ++i) {
        for (size_t j = 0; j < a[0].size(); ++j) {
            if (a[i][j] != b[i][j]) 
                return false;
        }
    }
    return true;
}

TEST(TableTest, TableEq) {
    for (int i = 0; i < TestFiles; ++i) {
        Table a1(dataPath + std::to_string(i) +".csv");
        a1.EvaluateTable();
        Table a2(ExpDataPath + std::to_string(i) +".csv");
        ASSERT_TRUE(IsEqual(a1, a2));
    }   
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
