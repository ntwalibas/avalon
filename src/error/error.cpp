#include <iostream>

#include "error/error.hpp"


namespace avalon {
/**
 * the error expects the source file path that contains the error
 */
error::error(const std::string& source_path) : m_source_path(source_path) {
}
    /**
     * set_source_path
     * updates the source path as compilation units can change
     * this function is mainly used by the parser to update the source path before it passes
     * control to a new parsing unit when doing imports.
     */
    void error::set_source_path(const std::string& source_path) {
        m_source_path = source_path;
    }

    /**
     * get_source_path
     * returns the current source path
     */
    std::string& error::get_source_path() {
        return m_source_path;
    }

    /**
     * log
     * this function expects the details including the source paht where the error occured and
     * the message from the unit that raised the error. Since tokens do carry the source path, we use the same here.
     */
    void error::log(const std::string& source_path, const int line, const int column, const std::string& message) {
        if(line == 0 || column == 0)
            std::cerr << source_path << ":" << message << std::endl;
        else
            std::cerr << source_path << ":" << line << ":" << column << ": " << message << std::endl;
    }

    void error::log(const std::string& source_path, const int line, const int column, const std::string& message) const {
        log(source_path, line, column, message);
    }

    /**
     * log
     * this function expects the details of where the error occured and
     * the message from the unit that raised the error.
     */
    void error::log(const int line, const int column, const std::string& message) {
        if(line == 0 || column == 0)
            std::cerr << m_source_path << ":" << message << std::endl;
        else
            std::cerr << m_source_path << ":" << line << ":" << column << ": " << message << std::endl;
    }

    void error::log(const int line, const int column, const std::string& message) const {
        log(line, column, message);
    }

    /**
     * log
     * this function expects the message to show.
     * the function is called by units that encounter an error before working on the file to compile
     */
    void error::log(const std::string& message) {
        std::cerr << message << std::endl;
    }

    void error::log(const std::string& message) const {
        std::cerr << m_source_path << ": " << message << std::endl;
    }
}
