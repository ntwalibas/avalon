#include <iostream>
#include <string>

#include <vector>
#include <memory>

#include <boost/filesystem.hpp>

#include "compiler/compiler.hpp"


void compile(const std::string& source_path);


int main(int argc, char * argv[])
{   if (argc != 2) {
        std::cout << "Usage: avalon [program]" << std::endl;
    }
    else {
        std::string source_path(argv[1]);
        compile(source_path);
    }

    return 0;
}

void compile(const std::string& source_path) {
    using namespace avalon;
    namespace fs = boost::filesystem;

    compiler comp;

    /* 1. configure the compiler according to options given */
    // add the current directory to the search path
    comp.add_search_path(fs::current_path());

    // add the avalon home directory to the search path
    //...

    /* 2. call the intepreter */
    comp.clean(source_path);
}
