#include <iostream>
#include <string>

#include <vector>
#include <memory>

#include <boost/filesystem.hpp>

#include "compiler/compiler.hpp"

#include "lexer/token.hpp"


void compile(const std::string& source_path);


int main(int argc, char * argv[])
{   if (argc < 2 || argc > 2) {
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
    /* Instead, we should be adding the given source code directory to the path
       and not where the compiler was run from
     */
    comp.add_search_path(fs::current_path());

    // add the avalon home directory to the search path

    /* 2. call the intepreter */
    //std::string path = compiler::get_source_path(source_path);
    //std::cout << source_path << std::endl;
    comp.import(source_path);
    /*
    std::vector<std::shared_ptr<token> > tokens = comp.lex(source_path);
    for(auto& tok : tokens)
        std::cout << * tok << std::endl;
    */
}
