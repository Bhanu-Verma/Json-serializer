#ifndef JSON_H
#define JSON_H

#include <unordered_map>
#include <variant>
#include <vector>
#include <string>

#include "json_value.hpp"

namespace json
{
    std::string get_str( const std::string& s );
    std::string get_str( bool val );
    std::string get_str( double val );
    std::string get_str( std::nullptr_t );
    std::string get_str( std::monostate );
    std::string get_str( const json::Json& obj );
    std::string get_str( const json::JsonValue& jVal );
    std::string get_str( const std::vector<json::JsonValue>& v );
    
    class Json {
        std::unordered_map<std::string, JsonValue> obj_;
    public:
        Json() = default;
        JsonValue& operator[](const std::string& key) { return obj_[key]; }
        const JsonValue& operator[](const std::string& key) const { return obj_.at(key); }

        // Serialization
        std::string serialize() const;
    };
}

#endif