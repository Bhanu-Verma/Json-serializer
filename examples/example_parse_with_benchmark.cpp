#include <json/json_lib.hpp>
#include <benchmarking/benchmarker.hpp>

#include <fstream>
#include <sstream>
#include <filesystem>

int main() {
    try {
        std::ifstream fs { "large.json" };
        if ( !fs ) {
            std::cerr << "Error: Could not open large.json" << std::endl;
        }

        std::string json_file_content = "";
        
        std::string temp;
        while( fs >> temp ) {
            json_file_content += temp;
        }

        Benchmarker::run("Parser", [](std::string_view sv){ return json::parse(sv); }, 10, json_file_content );  
    } catch (std::string_view e) {
        std::cerr << e << "\n";
        return 1;
    }

    return 0;
}