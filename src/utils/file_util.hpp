#ifndef AVALON_UTILS_FILE_HPP_
#define AVALON_UTILS_FILE_HPP_

#include <vector>
#include <string>

#include <boost/filesystem.hpp>


namespace avalon {
    class file_util {
    public:
        /**
         * the constructor expects the possible search paths where to check for files
         */
        file_util(std::vector<std::string>& search_paths);

        /**
         * source_exists
         * given a source path, this function returns true if the path exists in
         * one of the search paths.
         */
        bool source_exists(const std::string& source_path);

        /**
         * get_source_path
         * given a source file, get the absolute path to it.
         * returns a string containing said absolute path.
         * in case the file was not found, throw "file_not_found" exception.
         */
        std::string get_source_path(const std::string& source_path);

    private:
        /**
         * a vector of possible search paths
         */
        std::vector<std::string> m_search_paths;
    };
}

#endif
