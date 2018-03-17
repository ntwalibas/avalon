#ifndef AVALON_SCANNER_HPP_
#define AVALON_SCANNER_HPP_


namespace avalon {
    class scanner {
    public:
        /**
         * the scanner expects the path to the file with source code
         */
        scanner(const std::string& source_path);

        /**
         * scan
         * this function is called by the lexer to get the next chunk of
         * text to be tokenized.
         * at the moment, it reads the whole file and returns
         * the content of the file.
         * throws a "std::runtime_exception" exception if the file to scan was not found.
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
