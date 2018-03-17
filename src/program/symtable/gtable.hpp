#ifndef AVALON_PROGRAM_SYMTABLE_GTABLE_HPP_
#define AVALON_PROGRAM_SYMTABLE_GTABLE_HPP_

#include <unordered_map>
#include <string>
#include <queue>

#include "program/ast/program.hpp"


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
         * program_exists
         * given an fqn name, return true if it contains a program
         */
        bool program_exists(const std::string& fqn_name);

        /**
         * set_checking_order
         * set the order through which the program in this table must be analyzed
         */
        void set_checking_order(std::queue<std::string>& checking_order);

        /**
         * get_checking_order
         * returns the order through which the program in this table must be analyzed
         */
        std::queue<std::string>& get_checking_order();

    private:
        /*
         * map between fqn(fully qualified name) and the programs within them
         */        
        std::unordered_map<std::string, program> m_programs;

        /*
         * a queue listing all programs as they are to be checked during semantic analysis
         */
        std::queue<std::string> m_checking_order;
    };
}

#endif
