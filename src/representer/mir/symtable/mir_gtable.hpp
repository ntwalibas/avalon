#ifndef AVALON_MIR_SYMTABLE_GTABLE_HPP_
#define AVALON_MIR_SYMTABLE_GTABLE_HPP_

#include <unordered_map>
#include <string>

#include "representer/mir/ast/mir_program.hpp"


namespace avalon {
    class mir_gtable {
    public:
        /**
         * the constructor expects nothing
         */
        mir_gtable();

        /**
         * add_program
         * given a program, add it to the global table along with the fqn where it can be found
         */
        void add_program(mir_program& prog);

        /**
         * get_program
         * given an fqn name, return the program within it
         */
        mir_program& get_program(const std::string& fqn_name);

        /**
         * program_exists
         * given an fqn name, return true if it contains a program
         */
        bool program_exists(const std::string& fqn_name);

    private:
        /*
         * map between fqn(fully qualified name) and the programs within them
         */        
        std::unordered_map<std::string, mir_program> m_programs;
    };
}

#endif
