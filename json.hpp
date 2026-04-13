#ifndef JSON_H
#define JSON_H

#include <unordered_map>
#include <variant>
#include <vector>
#include <string>

#include "json_value.hpp"

namespace json
{
    class Json {
        std::unordered_map<std::string, JsonValue> obj_;
    public:
        Json() = default;
        JsonValue& operator[](const std::string& key) { return obj_[key]; }
        const JsonValue& operator[](const std::string& key) const { return obj_.at(key); }
    };
}

#endif