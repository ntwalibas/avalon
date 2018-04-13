#ifndef AVALON_UTILS_PARSE_HPP_
#define AVALON_UTILS_PARSE_HPP_

#include <string>

#include "representer/hir/ast/program.hpp"


namespace avalon {
    class parse_util {
    public:
        /**
         * parse
         * given a source path, this function calls the parser to parse
         * then returns the resulting parsed program.
         */
        static program parse(const std::string& source_path, std::vector<std::string>& search_paths);
    };
}

#endif
