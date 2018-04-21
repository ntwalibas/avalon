#ifndef AVALON_CHECKER_DECL_TYPE_GENERATOR_HPP_
#define AVALON_CHECKER_DECL_TYPE_GENERATOR_HPP_

#include <memory>

#include "representer/hir/ast/decl/type.hpp"


namespace avalon {
    class type_generator {
    public:
        /**
         * generate
         * given a type instance, generate a type declaration from it that is a specialization
         * of the type constructor from which that type instance was built
         */
        static std::shared_ptr<type> generate(type_instance& instance);

    private:
        /**
         * the default constructor is private
         */
        type_generator();
    };
}

#endif
