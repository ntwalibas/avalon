#include <algorithm>
#include <cstddef>
#include <string>

#include "program/symtable/exceptions/invalid_fqn.hpp"
#include "program/symtable/fqn.hpp"


/**
 * is_alpha
 * returns true if the character is a letter.
 */
static bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}


/**
 * is_dot
 * returns true if the character given is a dot.
 */
static bool is_dot(char c) {
    return c == '.';
}


/**
 * is_slash
 * returns true if the character given is a front slash,
 * false otherwise
 */
static bool is_slash(char c) {
    return c == '/';
}


/**
 * is_name_valid
 * make sure the name contains letters, underscores and dots only.
 */
static bool is_name_valid(const std::string& name) {
    std::size_t current = 0, length = name.length();
    while(current < length) {
        char current_char = name[current];
        if(!is_alpha(current_char) && !is_dot(current_char))
            return false;
        current++;
    }

    return true;
}


/**
 * is_path_valid
 * return true if the path contains letters, underscores, dots and front slahes only
 */
bool is_path_valid(const std::string& path) {
    std::size_t current = 0, length = path.length();
    while(current < length) {
        char current_char = path[current];
        if(!is_alpha(current_char) && !is_dot(current_char) && !is_slash(current_char)) {
            return false;
        }
        current++;
    }

    return true;
}


namespace avalon {
/**
 * fqn
 * default constructor expects nothing
 */
fqn::fqn() {
}

/**
 * parametrized constructor expects both the name and path equivalent of the fully qualified name
 */
fqn::fqn(const std::string& name, const std::string& path) : m_name(name), m_path(path) {
}

    /* from_name
     * returns an instance of a fully qualified name when given the qualified name.
     * it validates the name before setting it.
     * validation involves resolving the path that correponds to the name and
     * verifying that the path exists and is available for reading.
     */
    fqn fqn::from_name(const std::string& name) {
        // copy the name into the path variable for further processing independent of the name
        std::string path = name;

        // make sure the name contains valid characters only
        if(is_name_valid(name) == false)
            throw invalid_fqn("No file can be found for fqn <" + name + "> as it contains forbidden characters [by the compiler] that your operating system may allow.");

        // translate the name to a file system path
        std::replace(path.begin(), path.end(), '.', '/');
        path += ".avl";

        return fqn(name, path);
    }

    /**
     * from_path
     * returns an instance of a fully qualified name when given a path to the source.
     * it also verify that the path exists and is available for reading.
     */
    fqn fqn::from_path(const std::string& path) {
        // copy the path into the name variable for further processing independent of the path
        std::string name = path;

        // make sure the path contains valid characters only
        if(is_path_valid(path) == false)
            throw invalid_fqn("No file can be found for path <" + path + "> as it contains forbidden characters [by the compiler] that your operating system may allow.");

        // translate the path to a name
        // replace slashes by dots
        std::replace(name.begin(), name.end(), '/', '.');
        // remove the extension
        std::string::size_type xpos = path.find(".avl");
        if(xpos == std::string::npos)
            throw invalid_fqn("A file correponding to a fully qualified name is expected to end with the <.avl> extension.");
        else
            name.erase(xpos, 4);

        return fqn(name, path);
    }

    /**
     * get_name
     * returns the fqn name
     */
    std::string fqn::get_name() {
        return m_name;
    }

    std::string fqn::get_name() const {
        return m_name;
    }

    /**
     * get_path
     * returns a valid path derived from the fqn
     */
    std::string fqn::get_path() {
        return m_path;
    }

    std::string fqn::get_path() const {
        return m_path;
    }
}
