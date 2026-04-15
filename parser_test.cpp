#include "parser.hpp"
#include "tester/my_test_lib.hpp"

using namespace json;

TEST(Parse_Primitives) {
    std::string input = "{\"name\": \"Bhanu\", \"age\": 21, \"cool\": true}";
    Json result = json::parse(input);

    ASSERT(result["name"].get<std::string>() == "Bhanu");
    ASSERT(result["age"].get<double>() == 21.0);
    ASSERT(result["cool"].get<bool>() == true);
}

TEST(Parse_Arrays) {
    std::string input = "{\"scores\": [10, 20, 30]}";
    Json result = json::parse(input);

    auto& arr = result["scores"].get<std::vector<JsonValue>>();
    ASSERT(arr.size() == 3);
    ASSERT(arr[1].get<double>() == 20.0);
}

TEST(Parse_Whitespace_Handling) {
    // JSON is often formatted with messy spacing
    std::string input = R"(
    {
        "key" :    "value"  ,
        "list" : [ 1 ]
    }
    )";
    Json result = json::parse(input);
    ASSERT(result["key"].get<std::string>() == "value");
}

// --- 2. Arrays and Nesting ---

TEST(Parse_Nested_Structures) {
    std::string input = R"({
        "tags": ["cpp", "parser", 2026],
        "meta": {
            "version": 1.1,
            "stable": false
        }
    })";

    Json res = json::parse(input);

    // Check Array
    auto& tags = res["tags"].get<std::vector<JsonValue>>();
    ASSERT(tags.size() == 3);
    ASSERT(tags[0].get<std::string>() == "cpp");
    ASSERT(tags[2].get<double>() == 2026.0);

    // Check Nested Object
    ASSERT(res["meta"]["version"].get<double>() == 1.1);
    ASSERT(res["meta"]["stable"].get<bool>() == false);
}

// --- 3. Scientific Notation & Signs ---

TEST(Parse_Scientific_Notation) {
    std::string input = R"({
        "large": 1.2e3,
        "small": -2.5E-2,
        "plus_sign": +42
    })";

    Json res = json::parse(input);

    ASSERT(res["large"].get<double>() == 1200.0);
    ASSERT(res["small"].get<double>() == -0.025);
    ASSERT(res["plus_sign"].get<double>() == 42.0);
}

// --- 4. Escaped Strings ---

TEST(Parse_Escaped_Strings) {
    // JSON: {"path": "C:\\Users\\Bhanu", "note": "Line1\nLine2"}
    std::string input = R"({"path": "C:\\Users\\Bhanu", "note": "Line1\nLine2"})";
    
    Json res = json::parse(input);

    std::string s = res["path"].get<std::string>();
    std::cerr << "Path: " << s << '\n';
    ASSERT(res["path"].get<std::string>() == "C:\\Users\\Bhanu");
    ASSERT(res["note"].get<std::string>().find('\n') != std::string::npos);
}

// --- 5. Empty Collections & Whitespace ---

TEST(Parse_Empty_And_Messy) {
    std::string input = " { \"empty_obj\" : { } , \"empty_arr\" : [ ] } ";
    
    Json res = json::parse(input);

    ASSERT(res["empty_obj"].type() == JsonType::Object);
    ASSERT(res["empty_arr"].type() == JsonType::Array);
}

// --- 6. Failure Cases (Should Throw) ---

TEST(Parse_Invalid_Json) {
    // Missing closing brace
    ASSERT_THROWS(json::parse("{\"key\": 1"), std::string_view);
    
    // Missing colon
    ASSERT_THROWS(json::parse("{\"key\" 1}"), std::string_view);
    
    // Trailing comma in array (Standard JSON doesn't allow this)
    ASSERT_THROWS(json::parse("{\"arr\": [1, 2, ]}"), std::string_view);
    
    // Key is not a string
    ASSERT_THROWS(json::parse("{123: \"val\"}"), std::string_view);
}