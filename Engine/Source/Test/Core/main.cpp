#include "TestCore.h"
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char** argv){
    using namespace RTTestCore;
    std::vector<std::string> filters;
    bool list=false;
    for(int i=1;i<argc;++i){
        std::string a=argv[i];
        if(a=="--list"||a=="-l") list=true; else filters.push_back(a);
    }
    if(list){
        std::cout << "Available suites and cases:\n";
        for(auto& s: Registry::instance().suites()){
            std::cout << "- " << s->name() << "\n";
            for(auto& c: s->cases()) std::cout << "  * " << c.name << "\n";
        }
        return 0;
    }
    auto results = run_all(filters);
    return summarize_and_exit(results);
}
