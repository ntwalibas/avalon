#include <memory>

/* AST */
#include "program/ast/decl/import.hpp"
#include "program/ast/program.hpp"
/* Symbol table */
#include "program/symtable/exceptions/symbol_not_found.hpp"
#include "program/symtable/gtable.hpp"
#include "program/symtable/scope.hpp"

/* Checker */
#include "checker/stage_one/decl/import_checker.hpp"
#include "checker/exceptions/invalid_import.hpp"


namespace avalon {
/**
 * the constructor expects nothing
 */
import_checker::import_checker() {    
}

    /**
     * check
     * validate the import declaration
     */
    void import_checker::check(const std::shared_ptr<import>& import_decl, gtable& gtab) {        
    }
}
