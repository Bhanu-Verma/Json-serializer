#include "json/parser.hpp"

#include <iostream>
#include <array>
#include <algorithm>
#include <charconv>     // For string to double conversion using std::from_chars

json::Json parseJsonObj( std::string_view input, std::size_t& pos );
std::vector<json::JsonValue> parseArray( std::string_view input, std::size_t& pos );


constexpr std::array whitespaces{ ' ', '\n', '\t', '\r' };

static constexpr auto lookupTalbe = []() {
    std::array<uint8_t, 256> table{}; 
    for (char c : whitespaces) {
        table[static_cast<unsigned char>(c)] = 1;
    }
    return table;
}();

constexpr std::string_view ERROR_MSG = "Invalid Json Object";

void skip_whitespace( std::string_view input, std::size_t& pos) {
    while ( pos < input.size() && lookupTalbe[static_cast<unsigned char>(input[pos])] ) ++pos; 
}


bool parseBoolean( std::string_view input, std::size_t& pos ) {
    if ( input.compare(pos, 4, "true") == 0 ) {
        pos += 4;
        return true;
    }

    if ( input.compare(pos, 5, "false") == 0 ) {
        pos += 5;
        return false;
    }

    std::cerr << "parseBool failed\n";
    throw ERROR_MSG;
}

std::nullptr_t parseNull( std::string_view input, std::size_t& pos ) {
    if ( input.compare(pos, 4, "null") == 0 ) {
        pos += 4;
        return nullptr;
    }
    std::cerr << "parseNull failed\n";
    throw ERROR_MSG;
}

/**
 * @pre pos at digit or '-' or '+'
 */
double parseNumber( std::string_view input, std::size_t& pos ) {
    // '+' is not allowed as the first character
    if ( input[pos] == '+' ) ++pos;
    const char* first = input.data() + pos;
    const char* last = input.data() + input.size();
    double val;
    auto [ptr, ec] = std::from_chars(first, last, val);
    
    if (ec != std::errc()){
        std::cerr << "parseNumber failed\n";
        throw ERROR_MSG;
    } 
    
    pos += (ptr - first);
    return val;
}

/**
 * parses a string
 * @pre pos at '"'
 */
std::string parseString( std::string_view input, std::size_t& pos ) {
    ++pos;      // Move pointer ahead of '"'

    std::string res = "";
    while ( pos < input.size() && input[pos] != '"')
    {
        if ( input[pos] == '\\' ) {
            ++pos;
            if ( pos >= input.size() ) throw ERROR_MSG;
            switch (input[pos]) {
                case '"':  res += '"';  break;
                case '\\': res += '\\'; break;
                case '/':  res += '/';  break;
                case 'b':  res += '\b'; break;
                case 'f':  res += '\f'; break;
                case 'n':  res += '\n'; break;
                case 'r':  res += '\r'; break;
                case 't':  res += '\t'; break;
                default:
                    throw ERROR_MSG; // Invalid escape sequence
            }
            ++pos;
            continue;
        }
        if ( pos < input.size() ) {
            res += input[pos];
        }
        ++pos;
    }
    if( pos >= input.size() ) {
        std::cerr << "parseBool failed\n";
        throw ERROR_MSG;
    }
    ++pos;
    return res;
}


/**
 * parses a JsonValue
 * @pre pos not at whitespace
 */
json::JsonValue parseValue( std::string_view input, std::size_t& pos ) {
    char c = input[pos];

    if ( c == 'n' ) return json::JsonValue( parseNull(input, pos) );
    if ( c == '"' ) return json::JsonValue( parseString(input, pos) );
    if ( c == 't' || c == 'f' ) return json::JsonValue( parseBoolean(input, pos) );
    if ( isdigit(c) || c == '-' || c == '+' ) return json::JsonValue( parseNumber(input, pos) );
    if ( c == '{' ) return json::JsonValue( parseJsonObj(input, pos) );
    if ( c == '[' ) return json::JsonValue( parseArray(input, pos) );

    std::cerr << "praseValue failed\n";
    throw ERROR_MSG;
}

/**
 * @pre pos at '['
 */
std::vector<json::JsonValue> parseArray( std::string_view input, std::size_t& pos ) {
    ++pos;

    std::vector<json::JsonValue> res;

    skip_whitespace(input, pos);
    while ( pos < input.size() && input[pos] != ']' )
    { 
        skip_whitespace(input, pos);
        
        res.push_back(parseValue(input, pos));

        skip_whitespace(input, pos);

        if ( pos >= input.size() || (input[pos] != ',' && input[pos] != ']')) throw ERROR_MSG;
        if ( input[pos] == ',' ) ++pos;
    }
    if ( pos >= input.size() ) {
        std::cerr << "parseArray failed\n";
        throw ERROR_MSG;
    }

    ++pos;
    return res;
}

/**
 * parses a json object
 * @pre pos is at '{'
 */
json::Json parseJsonObj( std::string_view input, std::size_t& pos ) {
    ++pos;      // Move pointer ahead of '{'
    
    json::Json j;
    skip_whitespace(input, pos);
    while ( pos < input.size() && input[pos] != '}' ) {
        skip_whitespace(input, pos);

        // Key must be a string
        if ( pos >= input.size() || input[pos] != '"' ) {
            std::cerr << "key not found\n";
            throw ERROR_MSG;
        }
        std::string key = parseString(input, pos);

        skip_whitespace(input, pos);

        if ( pos >= input.size() || input[pos] != ':' ) {
            std::cerr << ": not found\n";
            throw ERROR_MSG;
        }
        ++pos;      // move ahead the ':'

        skip_whitespace(input, pos);
        json::JsonValue val = parseValue(input, pos);
        
        
        skip_whitespace(input, pos);
        
        if ( pos >= input.size() || (input[pos] != ',' && input[pos] != '}') ) {
            std::cerr << "} not found\n";
            throw ERROR_MSG;
        }
        if ( input[pos] == ',' ) ++pos;  

        j[std::move(key)] = std::move(val);
    }

    if ( pos >= input.size() ) throw ERROR_MSG;
    ++pos;
    return j;
}

json::Json json::parse( std::string_view input )
{
    std::size_t currPos = 0;
    skip_whitespace(input, currPos);  

    if ( currPos >= input.size() || input[currPos] != '{' ) {
        std::cerr << "{ not at start\n";
        throw ERROR_MSG;
    }

    json::Json res = parseJsonObj(input, currPos);

    skip_whitespace(input, currPos);
    if( currPos != input.size() ) {
        std::cerr << "complete input not consumed\n";
        throw ERROR_MSG;
    }

    return res;
}


