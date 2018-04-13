#include <string>
#include <queue>

#include "representer/exceptions/symbol_already_declared.hpp"
#include "representer/exceptions/symbol_not_found.hpp"
#include "representer/hir/symtable/gtable.hpp"
#include "representer/hir/ast/program.hpp"

namespace avalon {
/**
 * the constructor expects nothing
 */
gtable::gtable() {
}

    /**
     * add_program
     * given a program, add it to the global table along with the fqn where it can be found
     */
    void gtable::add_program(program& prog) {
        std::string fqn_name = prog.get_fqn().get_name();

        if(m_programs.count(fqn_name) > 0)
            throw symbol_already_declared("The FQN that contains the given program already exists in the global symbol table.");
        else
            m_programs.emplace(fqn_name, prog);
    }

    /**
     * get_program
     * given an fqn name, return the program within it
     */
    program& gtable::get_program(const std::string& fqn_name) {
        try {
            return m_programs.at(fqn_name);
        } catch(std::out_of_range err) {
            throw symbol_not_found("There is no program associated with the FQN given.");
        }
    }

    /**
     * get_programs
     * returns an unordered map of all programs available in this table
     */
    std::unordered_map<std::string, program>& gtable::get_programs() {
        return m_programs;
    }

    /**
     * program_exists
     * given an fqn name, return true if it contains a program
     */
    bool gtable::program_exists(const std::string& fqn_name) {
        if(m_programs.count(fqn_name) > 0)
            return true;
        else
            return false;
    }
}
