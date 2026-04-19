#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <fstream>
#include <chrono>

#include "json_lib.hpp"
#include "parser.hpp"
#include "tester/my_test_lib.hpp"
#include "benchmarker.hpp"

using namespace std::string_literals;

int main() {
    try {
        std::ifstream fs { "5mb.json" };
        std::string json_file_content = "";
        
        std::string temp;
        while( fs >> temp ) {
            json_file_content += temp;
        }

        json::Json obj = json::parse( json_file_content );
        Benchmarker::run("Serializer", [](const json::Json& j){ return j.serialize(); }, 10, obj );  
    } catch (std::string_view e) {
        std::cerr << e << "\n";
        return 1;
    }


    return 0;
}