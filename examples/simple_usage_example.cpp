#include <iostream>
#include <json/json.hpp>

#include <string>

using namespace std::string_literals;

int main() {
    json::Json myJson;
    
    // Using your optimized operator[]
    myJson["project"] = "FastJsonLibrary"s;
    myJson["version"] = 1.0;
    
    // Testing serialization
    std::cout << "Serialized JSON:\n" << myJson.serialize(0) << std::endl;
    
    return 0;
}