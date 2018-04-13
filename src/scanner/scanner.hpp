#ifndef AVALON_SCANNER_HPP_
#define AVALON_SCANNER_HPP_

#include <string>


namespace avalon {
    class scanner {
    public:
        /**
         * the scanner expects the path to the file with source code
         */
        scanner(const std::string& source_path);

        /**
         * scan
         * this function reads the content of a file and returns it (via return or via argument)
         * throws a "file_not_found" exception if the file to scan was not found.
         */
        std::string scan();

    private:
        /**
         * a reference to the path of the file containing the source code
         */
        const std::string& m_source_path;
    };
}

#endif
