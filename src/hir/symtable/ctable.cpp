#include <unordered_map>
#include <cstddef>
#include <utility>
#include <string>
#include <map>

#include "hir/symtable/exceptions/symbol_already_declared.hpp"
#include "hir/symtable/exceptions/symbol_not_found.hpp"
#include "hir/symtable/exceptions/invalid_symbol.hpp"
#include "hir/symtable/ctable.hpp"
#include "hir/ast/decl/type.hpp"


namespace avalon {
/**
 * the constructor expects nothing
 */
csymbols::csymbols() {
}

    /**
     * insert_default_constructor
     * insert a new default constructor into this symbol table
     */
    void csymbols::insert_default_constructor(default_constructor& def_cons) {
        std::pair<std::string, std::size_t> cons_key(def_cons.get_name(), def_cons.get_params().size());
        if(m_def_constructors.count(cons_key) > 0)
            throw symbol_already_declared("There already exists a default constructor with the same name and arity.");
        else
            m_def_constructors.emplace(cons_key, def_cons);
    }

    /**
     * insert_record_constructor
     * insert a new record constructor into this symbol table
     */
    void csymbols::insert_record_constructor(record_constructor& rec_cons) {
        std::pair<std::string, std::size_t> cons_key(rec_cons.get_name(), rec_cons.get_params().size());
        if(m_rec_constructors.count(cons_key) > 0)
            throw symbol_already_declared("There already exists a record constructor with the same name and arity.");
        else
            m_rec_constructors.emplace(cons_key, rec_cons);
    }

    /**
     * insert_list_constructor
     * insert a new list constructor into this symbol table
     */
    void csymbols::insert_list_constructor(list_constructor& list_cons) {
        const std::string& cons_key = list_cons.get_name();
        if(m_list_constructors.count(cons_key) > 0)
            throw symbol_already_declared("There already exists a list constructor with the same name.");
        else
            m_list_constructors.emplace(cons_key, list_cons);
    }

    /**
     * insert_map_constructor
     * insert a new map constructor into this symbol table
     */
    void csymbols::insert_map_constructor(map_constructor& map_cons) {
        const std::string& cons_key = map_cons.get_name();
        if(m_map_constructors.count(cons_key) > 0)
            throw symbol_already_declared("There already exists a map constructor with the same name.");
        else
            m_map_constructors.emplace(cons_key, map_cons);
    }

    /**
     * get_default_constructor
     * given a constructor name and arity, return the matching default constructor if it exists
     */
    default_constructor& csymbols::get_default_constructor(const std::string& name, std::size_t arity) {
        std::pair<std::string, std::size_t> cons_key(name, arity);
        try {
            return m_def_constructors.at(cons_key);
        } catch(std::out_of_range err) {
            throw symbol_not_found("There is no defaut constructor with the same name and arity declared.");
        }
    }

    /**
     * get_record_constructor
     * given a constructor name and arity, return the matching record constructor if it exists
     */
    record_constructor& csymbols::get_record_constructor(const std::string& name, std::size_t arity) {
        std::pair<std::string, std::size_t> cons_key(name, arity);
        try {
            return m_rec_constructors.at(cons_key);
        } catch(std::out_of_range err) {
            throw symbol_not_found("There is no record constructor with the same name and arity declared.");
        }
    }

    /**
     * get_list_constructor
     * given a constructor name, return the matching list constructor if it exists
     */
    list_constructor& csymbols::get_list_constructor(const std::string& name) {
        try {
            return m_list_constructors.at(name);
        } catch(std::out_of_range err) {
            throw symbol_not_found("There is no list constructor with the same name declared.");
        }
    }

    /**
     * get_map_constructor
     * given a constructor name, return the matching map constructor if it exists
     */
    map_constructor& csymbols::get_map_constructor(const std::string& name) {
        try {
            return m_map_constructors.at(name);
        } catch(std::out_of_range err) {
            throw symbol_not_found("There is no map constructor with the same name declared.");
        }
    }

    /**
     * default_constructor_exists
     * given a constructor name and its arity, find if an existing default constructor matches the same
     */
    bool csymbols::default_constructor_exists(const std::string& name, std::size_t arity) {
        std::pair<std::string, std::size_t> cons_key(name, arity);
        if(m_def_constructors.count(cons_key) > 0)
            return true;
        else
            return false;
    }

    /**
     * record_constructor_exists
     * given a constructor name and its arity, find if an existing record constructor matches the same
     */
    bool csymbols::record_constructor_exists(const std::string& name, std::size_t arity) {
        std::pair<std::string, std::size_t> cons_key(name, arity);
        if(m_rec_constructors.count(cons_key) > 0)
            return true;
        else
            return false;
    }

    /**
     * default_constructor_exists
     * given a constructor name, find if an existing default constructor matches the same
     */
    bool csymbols::default_constructor_exists(const std::string& name) {
        for(auto& def_cons : m_def_constructors) {
            if(def_cons.first.first == name)
                return true;
        }

        return false;
    }

    /**
     * record_constructor_exists
     * given a constructor name, find if an existing record constructor matches the same
     */
    bool csymbols::record_constructor_exists(const std::string& name) {
        for(auto& rec_cons : m_rec_constructors) {
            if(rec_cons.first.first == name)
                return true;
        }

        return false;
    }

    /**
     * list_constructor_exists
     * given a constructor name, find if an existing list constructor matches the same
     */
    bool csymbols::list_constructor_exists(const std::string& name) {
        if(m_list_constructors.count(name) > 0)
            return true;
        else
            return false;
    }

    /**
     * map_constructor_exists
     * given a constructor name, find if an existing map constructor matches the same
     */
    bool csymbols::map_constructor_exists(const std::string& name) {
        if(m_map_constructors.count(name) > 0)
            return true;
        else
            return false;
    }


/**
 * the constructor expects nothing
 */
ctable::ctable() {    
}

    /**
     * insert_default_constructor
     * insert a new default constructor into this symbol table
     */
    void ctable::insert_default_constructor(const std::string& ns_name, default_constructor& def_cons) {
        // we try to find if there already exists a symbol table attached to the current namespace and use it
        try {
            csymbols& cons_symbols = m_symbols.at(ns_name);
            try {
                cons_symbols.insert_default_constructor(def_cons);
            } catch(symbol_already_declared err) {
                throw symbol_already_declared("There already exists another default constructor with the same name and arity declared in this namespace.");
            }
        } catch(std::out_of_range err) {
            // if we don't have a symbol table attached to the given namespace, we create an entry for it
            csymbols cons_symbols;
            cons_symbols.insert_default_constructor(def_cons);
            m_symbols.emplace(ns_name, cons_symbols);
        }
    }

    /**
     * insert_record_constructor
     * insert a new record constructor into this symbol table
     */
    void ctable::insert_record_constructor(const std::string& ns_name, record_constructor& rec_cons) {
        // we try to find if there already exists a symbol table attached to the current namespace and use it
        try {
            csymbols& cons_symbols = m_symbols.at(ns_name);
            try {
                cons_symbols.insert_record_constructor(rec_cons);
            } catch(symbol_already_declared err) {
                throw symbol_already_declared("There already exists another record constructor with the same name and arity declared in this namespace.");
            }
        } catch(std::out_of_range err) {
            // if we don't have a symbol table attached to the given namespace, we create an entry for it
            csymbols cons_symbols;
            cons_symbols.insert_record_constructor(rec_cons);
            m_symbols.emplace(ns_name, cons_symbols);
        }
    }

    /**
     * insert_list_constructor
     * insert a new list constructor into this symbol table
     */
    void ctable::insert_list_constructor(const std::string& ns_name, list_constructor& list_cons) {
        // we try to find if there already exists a symbol table attached to the current namespace and use it
        try {
            csymbols& cons_symbols = m_symbols.at(ns_name);
            try {
                cons_symbols.insert_list_constructor(list_cons);
            } catch(symbol_already_declared err) {
                throw symbol_already_declared("There already exists another list constructor with the same name declared in this namespace.");
            }
        } catch(std::out_of_range err) {
            // if we don't have a symbol table attached to the given namespace, we create an entry for it
            csymbols cons_symbols;
            cons_symbols.insert_list_constructor(list_cons);
            m_symbols.emplace(ns_name, cons_symbols);
        }
    }

    /**
     * insert_map_constructor
     * insert a new map constructor into this symbol table
     */
    void ctable::insert_map_constructor(const std::string& ns_name, map_constructor& map_cons) {
        // we try to find if there already exists a symbol table attached to the current namespace and use it
        try {
            csymbols& cons_symbols = m_symbols.at(ns_name);
            try {
                cons_symbols.insert_map_constructor(map_cons);
            } catch(symbol_already_declared err) {
                throw symbol_already_declared("There already exists another map constructor with the same name declared in this namespace.");
            }
        } catch(std::out_of_range err) {
            // if we don't have a symbol table attached to the given namespace, we create an entry for it
            csymbols cons_symbols;
            cons_symbols.insert_map_constructor(map_cons);
            m_symbols.emplace(ns_name, cons_symbols);
        }
    }

    /**
     * get_default_constructor
     * given a namespace and a constructor name and arity, return the matching default constructor if it exists in that namespace
     */
    default_constructor& ctable::get_default_constructor(const std::string& ns_name, const std::string& cons_name, std::size_t arity) {
        try {
            csymbols& cons_symbols = m_symbols.at(ns_name);
            try {
                return cons_symbols.get_default_constructor(cons_name, arity);
            } catch(symbol_already_declared err) {
                throw symbol_already_declared("There is no default constructor with the given name and arity in this namespace.");
            }
        } catch(std::out_of_range err) {
            throw symbol_not_found("There is no default constructor with the given name and arity in this namespace.");
        }
    }

    /**
     * get_record_constructor
     * given a namespace, a constructor name and arity, return the matching record constructor if it exists in that namespace
     */
    record_constructor& ctable::get_record_constructor(const std::string& ns_name, const std::string& cons_name, std::size_t arity) {
        try {
            csymbols& cons_symbols = m_symbols.at(ns_name);
            try {
                return cons_symbols.get_record_constructor(cons_name, arity);
            } catch(symbol_already_declared err) {
                throw symbol_already_declared("There is no record constructor with the given name and arity in this namespace.");
            }
        } catch(std::out_of_range err) {
            throw symbol_not_found("There is no record constructor with the given name and arity in this namespace.");
        }
    }

    /**
     * get_list_constructor
     * given a constructor name, return the matching list constructor if it exists
     */
    list_constructor& ctable::get_list_constructor(const std::string& ns_name, const std::string& cons_name) {
        try {
            csymbols& cons_symbols = m_symbols.at(ns_name);
            try {
                return cons_symbols.get_list_constructor(cons_name);
            } catch(symbol_already_declared err) {
                throw symbol_already_declared("There is no list constructor with the given name in this namespace.");
            }
        } catch(std::out_of_range err) {
            throw symbol_not_found("There is no list constructor with the given name in this namespace.");
        }
    }

    /**
     * get_map_constructor
     * given a constructor name, return the matching map constructor if it exists
     */
    map_constructor& ctable::get_map_constructor(const std::string& ns_name, const std::string& cons_name) {
        try {
            csymbols& cons_symbols = m_symbols.at(ns_name);
            try {
                return cons_symbols.get_map_constructor(cons_name);
            } catch(symbol_already_declared err) {
                throw symbol_already_declared("There is no map constructor with the given name in this namespace.");
            }
        } catch(std::out_of_range err) {
            throw symbol_not_found("There is no map constructor with the given name in this namespace.");
        }
    }

    /**
     * default_constructor_exists
     * given a namespace and a type name and its arity, find if an existing default constructor matches the same in that namespace
     */
    bool ctable::default_constructor_exists(const std::string& ns_name, const std::string& cons_name, std::size_t arity) {
        try {
            csymbols& cons_symbols = m_symbols.at(ns_name);
            return cons_symbols.default_constructor_exists(cons_name, arity);
        } catch(std::out_of_range err) {
            return false;
        }
    }

    /**
     * record_constructor_exists
     * given a namespace and a type name and its arity, find if an existing record constructor matches the same in that namespace
     */
    bool ctable::record_constructor_exists(const std::string& ns_name, const std::string& cons_name, std::size_t arity) {
        try {
            csymbols& cons_symbols = m_symbols.at(ns_name);
            return cons_symbols.record_constructor_exists(cons_name, arity);
        } catch(std::out_of_range err) {
            return false;
        }
    }

    /**
     * default_constructor_exists
     * given a constructor name, find if an existing default constructor matches the same
     */
    bool ctable::default_constructor_exists(const std::string& ns_name, const std::string& cons_name) {
        try {
            csymbols& cons_symbols = m_symbols.at(ns_name);
            return cons_symbols.default_constructor_exists(cons_name);
        } catch(std::out_of_range err) {
            return false;
        }
    }

    /**
     * record_constructor_exists
     * given a constructor name, find if an existing record constructor matches the same
     */
    bool ctable::record_constructor_exists(const std::string& ns_name, const std::string& cons_name) {
        try {
            csymbols& cons_symbols = m_symbols.at(ns_name);
            return cons_symbols.record_constructor_exists(cons_name);
        } catch(std::out_of_range err) {
            return false;
        }
    }

    /**
     * list_constructor_exists
     * given a constructor name, find if an existing list constructor matches the same
     */
    bool ctable::list_constructor_exists(const std::string& ns_name, const std::string& cons_name) {
        try {
            csymbols& cons_symbols = m_symbols.at(ns_name);
            return cons_symbols.list_constructor_exists(cons_name);
        } catch(std::out_of_range err) {
            return false;
        }
    }

    /**
     * map_constructor_exists
     * given a constructor name, find if an existing map constructor matches the same
     */
    bool ctable::map_constructor_exists(const std::string& ns_name, const std::string& cons_name) {
        try {
            csymbols& cons_symbols = m_symbols.at(ns_name);
            return cons_symbols.map_constructor_exists(cons_name);
        } catch(std::out_of_range err) {
            return false;
        }
    }
}
