#include "json.hpp"

#include <type_traits>
#include <sstream>

auto isPretty = []( int indent ) -> bool { return (indent != -1); };
auto nextIndent = []( int indent ) -> int { return (indent == -1 )? -1 : indent + 1; };
constexpr auto INDENT_CHAR = ' ';

/**
 * Helper functions to get the pieces of serialized json string
 */
std::string json::get_str( const std::string& s, int /* indent */ ) {   // indentation is not used anyway
    return "\"" + s + "\"";
}

std::string json::get_str( bool val, int /* indent */ ) {
    return (val)? "true": "false";
}

std::string json::get_str( double val, int /* indent */ ) {
    std::ostringstream ss;
    ss << val;
    return ss.str();
}

std::string json::get_str( std::nullptr_t, int /* indent */ ) {
    return "null";
}

std::string json::get_str( std::monostate, int /* indent */ ) {
    return "null";
}


std::string json::get_str( const json::Json& obj, int indent ) {
    return obj.serialize(indent);
} 

std::string json::get_str( const json::JsonValue& jVal, int indent ) {
    auto func = [indent = indent]( const auto& val) -> std::string {
        using T = std::decay_t<decltype(val)>;
        if constexpr ( std::is_same_v<T, std::unique_ptr<json::Json>> ) {
            return (val)? get_str(*val, indent) : get_str(nullptr, indent);
        }
        else {
            return get_str(val, indent);
        }
    };
    return std::visit(func, jVal.getVar());
} 

std::string json::get_str( const std::vector<json::JsonValue>& v, int indent ) {
    if ( v.empty() ) return "[]";

    bool pretty = isPretty(indent);

    std::string res = "[";
    if ( pretty ) res += '\n';
    
    std::string indentString = "";
    if ( pretty ) indentString = std::string(indent+1, INDENT_CHAR);

    for( auto it = v.cbegin(); it != v.cend(); ) {
        res += indentString;
        res += get_str(*it, nextIndent(indent));
        ++it;
        if ( it != v.cend() ) res += ',';
        if ( isPretty(indent) ) res += '\n';
    }   
    if ( isPretty(indent) ) res += std::string(indent, INDENT_CHAR);
    res += ']';
    return res;
}

std::string json::prettyPrint( const Json &obj )
{
    return obj.serialize(0);
}

std::ostream &json::operator<<(std::ostream &o, const Json &j)
{
    std::string temp { json::prettyPrint(j) };
    return o << temp;
}

std::string json::Json::serialize(int indent) const
{
    if( obj_.empty() ) return "{}";
    bool pretty = isPretty(indent);

    std::string res = "{";
    if ( pretty ) res += '\n';

    std::string indentString = "";
    if ( pretty ) indentString = std::string(indent+1, INDENT_CHAR);

    int i = 0;
    for( auto& [key, val]: obj_ ) {
        res += indentString;

        res += get_str(key, nextIndent(indent));
        res += ':';
        if ( pretty ) res += ' ';

        res += get_str(val, nextIndent(indent));

        if ( i != obj_.size() - 1 ) res += ',';
        if ( pretty ) res += '\n';
        ++i;
    }

    if ( pretty ) res += std::string(indent, INDENT_CHAR);
    res += '}';
    return res;
}