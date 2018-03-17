#include <stdexcept>
#include <iostream>
#include <fstream>
#include <vector>

#include "compiler/exceptions/file_not_found.hpp"
#include "scanner/scanner.hpp"


namespace avalon {
scanner::scanner(const std::string& source_path) : m_source_path(source_path) {        
}

    std::string scanner::scan() {
        std::ifstream source_file(m_source_path.c_str(), std::ios::in | std::ios::ate);

        // make sure the file is available for reading
        // we can't do this with boost because a file is only available for reading when we actually open it
        if (!source_file.good()) {
            throw file_not_found("Failed to open source file <" + m_source_path + ">. Does the file exist?");
        }

        // read the entire source file content into the source string
        // 1. get the file size and position cursor at the beginning
        std::ifstream::pos_type file_size = source_file.tellg();
        source_file.seekg(0, std::ios::beg);

        // 2. read the file into a char (one byte) vector
        std::vector<char> bytes(file_size);
        source_file.read(&bytes[0], file_size);

        // 3. put the bytes read into the string
        std::string source(&bytes[0], file_size);

        // return the source string
        return source;
    }
}
