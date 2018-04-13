#ifndef AVALON_UTILS_FILE_NOT_FOUND_H_
#define AVALON_UTILS_FILE_NOT_FOUND_H_

#include <stdexcept>


namespace avalon {
    class file_not_found : public std::runtime_error {
        public:
            file_not_found(const std::string& message);
            virtual const char * what() const noexcept;
    };
}

#endif
