#include <unordered_map>
#include <string>

#include "representer/exceptions/symbol_already_declared.hpp"
#include "representer/exceptions/symbol_not_found.hpp"
#include "representer/mir/symtable/mir_gtable.hpp"
#include "representer/mir/ast/mir_program.hpp"

namespace avalon {
    /**
     * the constructor expects nothing
     */
    mir_gtable::mir_gtable() {
    }

    /**
     * add_program
     * given a program, add it to the global table along with the fqn where it can be found
     */
    void mir_gtable::add_program(mir_program& prog) {
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
    mir_program& mir_gtable::get_program(const std::string& fqn_name) {
        try {
            return m_programs.at(fqn_name);
        } catch(std::out_of_range err) {
            throw symbol_not_found("There is no program associated with the FQN given.");
        }
    }

    /**
     * program_exists
     * given an fqn name, return true if it contains a program
     */
    bool mir_gtable::program_exists(const std::string& fqn_name) {
        if(m_programs.count(fqn_name) > 0)
            return true;
        else
            return false;
    }
}
