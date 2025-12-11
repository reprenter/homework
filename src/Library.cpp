#include "Library.h"
#include <fstream>
#include <iostream>

Library::Library(const std::string& dataFile) : dataFile(dataFile) {
    std::ifstream file(dataFile);
    if (!file.is_open()) {
        std::ofstream newFile(dataFile);
        if (newFile.is_open()) {
            newFile << "---USERS---" << std::endl;
            newFile.close();
        }
    }
}