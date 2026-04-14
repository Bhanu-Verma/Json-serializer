#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include "json_lib.hpp"

#include "tester/my_test_lib.hpp"

TEST(BasicTest) {
    ASSERT( 1 == 1);
}

using namespace std::string_literals;

int main() {
    try {
        // --- 1. Basic Object Creation ---
        json::Json stud;
        
        // Testing operator[] and assignment from different types
        stud["name"] = "Bhanu Verma"s;
        stud["rollno"] = 22075021.0; // JSON numbers are doubles
        stud["cpi"] = 9.77;
        stud["is_graduated"] = false;

        std::cout << std::get<double>(stud["rollno"].getVar()) << '\n';

        // --- 2. Nested Array Creation ---
        // Testing if we can wrap a vector into a JsonValue
        std::vector<json::JsonValue> spi_list = {
            json::JsonValue(9.63), 
            json::JsonValue(9.80), 
            json::JsonValue(10.0)
        };
        stud["sem_wise_spi"] = spi_list;

        // --- 3. Retrieval and Verification ---
        std::cout << "Testing retrieval...\n";

        // Accessing the name
        // Note: You'll need to expose a way to get the variant or use a getter
        std::string name = stud["name"].get<std::string>();
        std::cout << "Name: " << name << "\n";
        assert(name == "Bhanu Verma");

        // Accessing the number
        double cpi = stud["cpi"].get<double>();
        std::cout << "CPI: " << cpi << "\n";
        assert(cpi == 9.77);

        // --- 4. Deep Nesting Test ---
        json::Json address;
        address["city"] = "Varanasi"s;
        address["zip"] = 221005.0;
        
        stud["location"] = address;

        std::cout << "City: " << stud["location"]["city"].get<std::string>() << "\n";
        
        std::cout << "\nAll manual construction tests passed!\n";

        // serialization test
        const std::string addr_as_str = address.serialize();
        std::cout << addr_as_str << '\n';

        const std::string stud_as_str = stud.serialize();
        std::cout << stud_as_str << '\n';        

    } catch (const std::exception& e) {
        std::cerr << "Test failed with error: " << e.what() << "\n";
        return 1;
    }

    // MyTester::runAllTests();
    return 0;
}