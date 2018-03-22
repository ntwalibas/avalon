#ifndef AVALON_CHECKER_LAX_IMPORT_CHECKER_HPP_
#define AVALON_CHECKER_LAX_IMPORT_CHECKER_HPP_

#include <memory>

#include "program/ast/decl/import.hpp"
#include "program/symtable/gtable.hpp"


namespace avalon {
    class import_checker {
    public:
        /**
         * the constructor expects nothing
         */
        import_checker();

        /**
         * check
         * validate the import declaration
         */
        void check(const std::shared_ptr<import>& import_decl, gtable& gtab);
    };
}

#endif
