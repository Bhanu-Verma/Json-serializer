#include "tester/my_test_lib.hpp"
#include "json_lib.hpp"

#include <iostream>
#include <vector>
#include <string>

using namespace json;
using namespace std::string_literals;

// --- 1. Basic Type Tests ---

TEST(JsonValue_BasicTypes) {
    // Null
    JsonValue v_null;
    ASSERT(v_null.type() == JsonType::Null);

    // Boolean
    JsonValue v_bool(true);
    ASSERT(v_bool.type() == JsonType::Bool);
    ASSERT(v_bool.get<bool>() == true);

    // Number (double)
    JsonValue v_num(3.14);
    ASSERT(v_num.type() == JsonType::Number);
    ASSERT(v_num.get<double>() == 3.14);
}

TEST(JsonValue_StringTests) {
    // From std::string
    JsonValue v_str("Hello"s);
    ASSERT(v_str.type() == JsonType::String);
    ASSERT(v_str.get<std::string>() == "Hello");

    // From C-string
    JsonValue v_cstr("World");
    ASSERT(v_cstr.type() == JsonType::String);
    ASSERT(v_cstr.get<std::string>() == "World");
}

// --- 2. Assignment Tests ---

TEST(JsonValue_Assignment) {
    JsonValue v;
    
    v = 42.0;
    ASSERT(v.type() == JsonType::Number);
    ASSERT(v.get<double>() == 42.0);

    v = "Updated String"s;
    ASSERT(v.type() == JsonType::String);
    ASSERT(v.get<std::string>() == "Updated String");

    v = nullptr;
    ASSERT(v.type() == JsonType::Null);
}

// --- 3. Recursive Structure Tests ---

TEST(JsonValue_ArrayTests) {
    std::vector<JsonValue> list = { JsonValue(1.0), JsonValue("Item"s), JsonValue(false) };
    JsonValue v_arr(list);

    ASSERT(v_arr.type() == JsonType::Array);
    auto& retrieved = v_arr.get<std::vector<JsonValue>>();
    ASSERT(retrieved.size() == 3);
    ASSERT(retrieved[0].get<double>() == 1.0);
    ASSERT(retrieved[2].get<bool>() == false);
}

TEST(JsonValue_ObjectNesting) {
    Json sub_obj;
    sub_obj["key"] = "value"s;

    JsonValue v_obj(sub_obj);
    ASSERT(v_obj.type() == JsonType::Object);

    // Test the custom subscript operator you implemented
    ASSERT(v_obj["key"].type() == JsonType::String);
    ASSERT(v_obj["key"].get<std::string>() == "value");
}

// --- 4. Copy and Move Semantics ---
TEST(JsonValue_CopySemantics) {
    JsonValue original("Original"s);
    JsonValue copy = original; // Copy constructor

    ASSERT(copy.get<std::string>() == "Original");
    
    // Ensure deep copy (modifying copy doesn't touch original)
    copy = "Changed"s;
    ASSERT(original.get<std::string>() == "Original");
    ASSERT(copy.get<std::string>() == "Changed");
}

TEST(JsonValue_MoveSemantics) {
    JsonValue source("I am moving"s);
    JsonValue target = std::move(source); // Move constructor

    ASSERT(target.get<std::string>() == "I am moving");
    // Note: source is now in a valid but unspecified state (likely Null due to variant move)
}

TEST(JsonValue_DeepObjectCopy) {
    Json obj;
    obj["id"] = 1.0;
    
    JsonValue v1(obj);
    JsonValue v2 = v1; // Deep copy of unique_ptr<Json>

    ASSERT(v2["id"].get<double>() == 1.0);
    
    // Modify nested value in v2
    v2["id"] = 2.0;
    
    // Ensure v1 was NOT modified (tests your unique_ptr cloning logic)
    ASSERT(v1["id"].get<double>() == 1.0);
    ASSERT(v2["id"].get<double>() == 2.0);
}


TEST(Json_BasicOperations) {
    Json root;
    
    // Testing insertion via operator[]
    root["key1"] = 100.0;
    root["key2"] = "Value"s;
    root["key3"] = true;

    ASSERT(root["key1"].get<double>() == 100.0);
    ASSERT(root["key2"].get<std::string>() == "Value");
    ASSERT(root["key3"].get<bool>() == true);
}

TEST(Json_OverwriteValues) {
    Json root;
    
    root["status"] = "Pending"s;
    ASSERT(root["status"].get<std::string>() == "Pending");

    // Overwriting with a different type
    root["status"] = 200.0; 
    ASSERT(root["status"].type() == JsonType::Number);
    ASSERT(root["status"].get<double>() == 200.0);
}

// --- 2. Const Access ---

TEST(Json_ConstAccess) {
    Json root;
    root["id"] = 1.0;

    const Json& const_root = root;
    
    // Should work for existing keys
    ASSERT(const_root["id"].get<double>() == 1.0);
}

TEST(Json_ConstAccess_Throws) {
    Json root;
    const Json& const_root = root;

    bool caught = false;
    try {
        // .at() should throw because "missing" doesn't exist
        auto& val = const_root["missing"];
    } catch (const std::out_of_range& e) {
        caught = true;
    }
    
    ASSERT(caught == true);
}

// --- 3. Nested Objects (Direct Interface) ---
TEST(Json_NestedObjects) {
    Json root;
    
    // Create nested structure directly
    root["user"]["details"]["name"] = "Bhanu"s;
    root["user"]["details"]["id"] = 7.0;

    ASSERT(root["user"].type() == JsonType::Object);
    ASSERT(root["user"]["details"]["name"].get<std::string>() == "Bhanu");
    ASSERT(root["user"]["details"]["id"].get<double>() == 7.0);
}

// --- 4. Size/Existence (Implicit) ---

TEST(Json_ImplicitCreation) {
    Json root;
    
    // Accessing a non-existent key with the non-const operator[] 
    // should create a Null JsonValue automatically (default map behavior)
    JsonValue& v = root["new_key"];
    
    ASSERT(v.type() == JsonType::Null);
    ASSERT(root["new_key"].type() == JsonType::Null);
}

TEST(Json_3_depth) {
    json::Json stud;
    stud["name"] = JsonValue("Bhanu");
    stud["dept"] = JsonValue("CSE");
    
    json::Json address;
    address["pincode"] = JsonValue(207401.0);

    json::Json extraInfo;
    extraInfo["type"] = JsonValue("Landmark");
    extraInfo["value"] = JsonValue("Near Agra");

    address["comments"] = JsonValue(extraInfo); 
    stud["address"] = JsonValue(address);
    
    ASSERT(stud["name"].get<std::string>() == "Bhanu");
    ASSERT(stud["dept"].get<std::string>() == "CSE");
    ASSERT(stud["address"]["pincode"].get<double>() == 207401.0);
    ASSERT(stud["address"]["comments"]["type"].get<std::string>() == "Landmark");
}

/**                 Serialization Tests              */

#include "json.hpp"
#include "json_value.hpp"
#include "tester/my_test_lib.hpp"
#include <string>

using namespace json;
using namespace std::string_literals;

// --- 1. Primitive Serialization (Minified) ---

TEST(Serialize_Primitives_Minified) {
    Json root;
    root["name"] = "Bhanu"s;
    root["age"] = 21.0;
    root["is_student"] = true;
    root["middle_name"] = nullptr;

    std::string out = root.serialize();

    // No spaces after ':' or ','
    ASSERT(out.find("\"name\":\"Bhanu\"") != std::string::npos);
    ASSERT(out.find("\"is_student\":true") != std::string::npos);
    ASSERT(out.find("\"middle_name\":null") != std::string::npos);
}

// --- 2. Precision Double Test (Ostream) ---

TEST(Serialize_Double_Precision) {
    Json root;
    // Using ostream/oss should result in "5.5" instead of "5.500000"
    root["score"] = 5.5;
    root["integer"] = 100.0;

    std::string out = root.serialize();

    ASSERT(out.find("\"score\":5.5") != std::string::npos);
    ASSERT(out.find("\"integer\":100") != std::string::npos);
}

// --- 3. Array Serialization (Minified) ---

TEST(Serialize_Arrays_Minified) {
    Json root;
    std::vector<JsonValue> grades = { JsonValue(9.5), JsonValue(10.0) };
    root["grades"] = grades;

    std::string out = root.serialize();

    // Verifying format: [9.5,10] 
    ASSERT(out.find("\"grades\":[9.5,10]") != std::string::npos);
}

// --- 4. Nested Object (Minified) ---

TEST(Serialize_Nested_Minified) {
    Json root;
    root["outer"]["inner"] = 42.0;

    std::string out = root.serialize();

    // Full structure: {"outer":{"inner":42}}
    ASSERT(out == "{\"outer\":{\"inner\":42}}");
}

// --- 5. Empty Structures ---

TEST(Serialize_Empty_Containers) {
    Json obj;
    ASSERT(obj.serialize() == "{}");

    obj["empty_list"] = std::vector<JsonValue>{};
    ASSERT(obj.serialize() == "{\"empty_list\":[]}");
}

int main() {
    MyTester::runAllTests();
    return 0;
}