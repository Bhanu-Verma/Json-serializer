#include "json/json_lib.hpp"
#include "benchmarking/benchmarker.hpp"
#include <fstream>
#include <sstream>

int main() {
    std::ifstream file("5mb.json");
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string input = buffer.str();

    {
        Benchmarker::run("Parser Statistics", 
                        [](std::string_view sv){
                            return json::parse(sv);
                        }, 
                        10, 
                        input
                    );
    }
    return 0;
}