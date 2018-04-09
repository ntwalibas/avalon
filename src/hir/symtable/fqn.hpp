#ifndef AVALON_HIR_SYMTABLE_FQN_HPP_
#define AVALON_HIR_SYMTABLE_FQN_HPP_


namespace avalon {
    class fqn {
    public:
        /**
         * fqn
         * default constructor expects nothing
         */
        fqn();

        /**
         * parametrized constructor expects both the name and path equivalent of the fully qualified name
         */
        fqn(const std::string& name, const std::string& path);

        /* from_name
         * returns an instance of a fully qualified name when given the qualified name.
         * it validates the name before setting it.
         * validation involves resolving the path that correponds to the name and
         * verifying that the path exists and is available for reading.
         */
        static fqn from_name(const std::string& name);

        /**
         * from_path
         * returns an instance of a fully qualified name when given a path to the source.
         * it also verify that the path exists and is available for reading.
         */
        static fqn from_path(const std::string& path);

        /**
         * get_fqn
         * returns the fqn name
         */
        std::string get_name();
        std::string get_name() const;

        /**
         * get_path
         * returns a valid path derived from the fqn
         */
        std::string get_path();
        std::string get_path() const;

    private:
        /**
         * a string representation of a fqn
         */
        std::string m_name;

        /**
         * the path equivalent of a fqn
         */
        std::string m_path;
    };
    
    /**
     * operator==
     * compares two fqns for equality
     */
    inline bool operator==(const fqn& lhs, const fqn& rhs) {
        if (lhs.get_name() == rhs.get_name())
            return true;
        else
            return false;
    }
    
    /**
     * operator!=
     * compares two fqns for inequality
     */
    inline bool operator!=(const fqn& lhs, const fqn& rhs) {
        return !(lhs == rhs);
    }
}

#endif
