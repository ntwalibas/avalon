#ifndef AVALON_COMPILER_INVALID_DIRECTORY_H_
#define AVALON_COMPILER_INVALID_DIRECTORY_H_

#include <stdexcept>


namespace avalon {
    class invalid_directory : public std::runtime_error {
        public:
            invalid_directory(const std::string& message);
            virtual const char * what() const noexcept;
    };
}

#endif
