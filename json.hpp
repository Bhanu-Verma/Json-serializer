#ifndef JSON_H
#define JSON_H

#include <unordered_map>
#include <variant>
#include <vector>
#include <string>

#include "json_value.hpp"

namespace json
{
    std::string get_str( const std::string& s, int );
    std::string get_str( bool val, int );
    std::string get_str( double val, int );
    std::string get_str( std::nullptr_t, int );
    std::string get_str( std::monostate, int );
    std::string get_str( const json::Json& obj, int );
    std::string get_str( const json::JsonValue& jVal, int );
    std::string get_str( const std::vector<json::JsonValue>& v, int );

    class Json {
        std::unordered_map<std::string, JsonValue> obj_;
    public:
        Json() = default;

        JsonValue& operator[](const std::string& key) { return obj_[key]; }
        const JsonValue& operator[](const std::string& key) const { return obj_.at(key); }

        JsonValue& operator[]( std::string&& key) { return obj_[std::move(key)]; }
        const JsonValue& operator[]( std::string&& key) const { return obj_.at(std::move(key)); }

        // Serialization
        std::string serialize(int indent = -1) const;  // indent -1 indicates no pretty print
    };

    std::string prettyPrint( const Json& obj );
    std::ostream& operator<<(std::ostream& o, const Json& j);
}

#endif