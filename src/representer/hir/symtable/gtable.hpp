#ifndef AVALON_HIR_SYMTABLE_GTABLE_HPP_
#define AVALON_HIR_SYMTABLE_GTABLE_HPP_

#include <unordered_map>
#include <string>
#include <queue>

#include "representer/hir/ast/program.hpp"


namespace avalon {
    class gtable {
    public:
        /**
         * the constructor expects nothing
         */
        gtable();

        /**
         * add_program
         * given a program, add it to the global table along with the fqn where it can be found
         */
        void add_program(program& prog);

        /**
         * get_program
         * given an fqn name, return the program within it
         */
        program& get_program(const std::string& fqn_name);

        /**
         * get_programs
         * returns an unordered map of all programs available in this table
         */
        std::unordered_map<std::string, program>& get_programs();

        /**
         * program_exists
         * given an fqn name, return true if it contains a program
         */
        bool program_exists(const std::string& fqn_name);

        /**
         * set_main_fqn
         * set the fqn to the program with the main function
         */
        void set_main_fqn(const std::string& fqn_name);

        /**
         * returns true if the program with the main fqn has been set
         */
        bool has_main_fqn();

        /**
         * get_main_program
         * returns the main program if one has been set, else it throws a runtime error
         */
        program& get_main_program();

    private:
        /*
         * map between fqn(fully qualified name) and the programs within them
         */        
        std::unordered_map<std::string, program> m_programs;

        /*
         * fqn to the program with the main function
         */
        std::string m_main_fqn;
    };
}

#endif
