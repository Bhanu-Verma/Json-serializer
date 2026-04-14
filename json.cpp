#include "json.hpp"

#include <type_traits>
#include <sstream>

/**
 * Helper functions to get the pieces of serialized json string
 */
std::string json::get_str( const std::string& s ) {
    return "\"" + s + "\"";
}

std::string json::get_str( bool val ) {
    return (val)? "true": "false";
}

std::string json::get_str( double val ) {
    std::ostringstream ss;
    ss << val;
    return ss.str();
}

std::string json::get_str( std::nullptr_t ) {
    return "null";
}

std::string json::get_str( std::monostate ) {
    return "null";
}


std::string json::get_str( const json::Json& obj ) {
    return obj.serialize();
} 

std::string json::get_str( const json::JsonValue& jVal ) {
    auto func = []( const auto& val) -> std::string {
        using T = std::decay_t<decltype(val)>;
        if constexpr ( std::is_same_v<T, std::unique_ptr<json::Json>> ) {
            return (val)? get_str(*val) : get_str(nullptr);
        }
        else {
            return get_str(val);
        }
    };
    return std::visit(func, jVal.getVar());
} 

std::string json::get_str( const std::vector<json::JsonValue>& v ) {
    std::string res = "[";
    for( auto it = v.cbegin(); it != v.cend(); ) {
        res += get_str(*it);
        ++it;
        if ( it != v.cend() ) res += ',';
    }   
    res += ']';
    return res;
}

std::string json::Json::serialize() const
{
    std::string res = "{";
    int i = 0;
    for( auto& [key, val]: obj_ ) {
        res += get_str(key);
        res += ":";
        res += get_str(val);
        if ( i != obj_.size() - 1) res += ',';
        ++i;
    }
    res += '}';
    return res;
}