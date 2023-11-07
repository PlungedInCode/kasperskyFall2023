#include <boost/filesystem.hpp>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace boost::filesystem;

const std::string LOG_FILE_PREFIX = "prod_";
const std::string LOG_FILE_EXTENSION = ".log";
const int DISPLAY_COUNT = 3;

bool isLogFile(const std::string& filename) {
    if (filename.substr(0, LOG_FILE_PREFIX.size()) != LOG_FILE_PREFIX ||
        filename.substr(filename.size() - LOG_FILE_EXTENSION.size()) != LOG_FILE_EXTENSION) {
        return false;
    }
    std::string nums(filename.begin() + LOG_FILE_PREFIX.size(), filename.end() - LOG_FILE_EXTENSION.size());
    return std::all_of(nums.begin(), nums.end(), isdigit);
}

bool compareFilesByTime(const path& filePath1, const path& filePath2) {
    return last_write_time(filePath1) > last_write_time(filePath2);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <path to directory>\n";
        return -1;
    }

    std::string directoryPath = argv[1];

    try {
        if (!is_directory(directoryPath) || !exists(directoryPath)) {
            throw std::invalid_argument("The specified directory does not exist.");
        }

        std::vector<path> logFiles;

        for (const directory_entry& entry : directory_iterator(directoryPath)) {
            if (is_regular_file(entry) && !is_empty(entry) && isLogFile(entry.path().filename().string())) {
                logFiles.push_back(entry.path());
            }
        }

        std::sort(logFiles.begin(), logFiles.end(), compareFilesByTime);

        for (size_t i = 0; i < std::min(logFiles.size(), size_t(DISPLAY_COUNT)); ++i) {
            std::cout << absolute(logFiles[i]).string() << '\n';
        }

        if (logFiles.empty()) {
            std::cout << "There are no non-empty log files in the directory.\n";
        }
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return -2;
    }

    return 0;
}
