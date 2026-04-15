#include "parser.hpp"
#include "tester/my_test_lib.hpp"

#include <array>
#include <algorithm>

json::Json parseJsonObj( const std::string &input, std::size_t& pos );
std::vector<json::JsonValue> parseArray( const std::string& input, std::size_t& pos );


constexpr std::array whitespaces{ ' ', '\n', '\t', '\r' };
bool isWhitespace ( char ch ) {
    return std::find(whitespaces.cbegin(), whitespaces.cend(), ch) != whitespaces.cend();
}
constexpr std::string_view ERROR_MSG = "Invalid Json Object";



void skip_whitespace( const std::string &input, std::size_t& pos) {
    while ( pos < input.size() && isWhitespace(input[pos]) ) ++pos; 
}


bool parseBoolean( const std::string& input, std::size_t& pos ) {
    if ( input.compare(pos, 4, "true") == 0 ) {
        pos += 4;
        return true;
    }

    if ( input.compare(pos, 5, "false") == 0 ) {
        pos += 5;
        return false;
    }

    throw ERROR_MSG;
}

std::nullptr_t parseNull( const std::string& input, std::size_t& pos ) {
    if ( input.compare(pos, 4, "null") == 0 ) {
        pos += 4;
        return nullptr;
    }
    throw ERROR_MSG;
}

/**
 * @pre pos at digit or '-' or '+'
 */
double parseNumber( const std::string& input, std::size_t& pos ) {
    // Gather all possible characters that can be included in number
    std::size_t ptr = pos;
    while ( ptr < input.size() ) {
        char ch = input[ptr];
        if ( isdigit(ch) || ch == 'e' || ch == 'E' || ch == '.' || ch == '+' || ch == '-' ) ++ptr;
        else break;
    }

    std::string numStr = input.substr(pos, ptr - pos);
    try{
        std::size_t taken = 0;
        double d = std::stod(numStr, &taken);
        pos += taken;
        return d;
    }
    catch(...) {
        throw ERROR_MSG;
    }
    
}

/**
 * parses a string
 * @pre pos at '"'
 */
std::string parseString( const std::string& input, std::size_t& pos ) {
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
    if( pos >= input.size() ) throw ERROR_MSG;
    ++pos;
    return res;
}


/**
 * parses a JsonValue
 * @pre pos not at whitespace
 */
json::JsonValue parseValue( const std::string& input, std::size_t& pos ) {
    char c = input[pos];

    if ( c == 'n' ) return json::JsonValue( parseNull(input, pos) );
    if ( c == '"' ) return json::JsonValue( parseString(input, pos) );
    if ( c == 't' || c == 'f' ) return json::JsonValue( parseBoolean(input, pos) );
    if ( isdigit(c) || c == '-' || c == '+' ) return json::JsonValue( parseNumber(input, pos) );
    if ( c == '{' ) return json::JsonValue( parseJsonObj(input, pos) );
    if ( c == '[' ) return json::JsonValue( parseArray(input, pos) );

    throw ERROR_MSG;
}

/**
 * @pre pos at '['
 */
std::vector<json::JsonValue> parseArray( const std::string& input, std::size_t& pos ) {
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
    if ( pos >= input.size() ) throw ERROR_MSG;

    ++pos;
    return res;
}

/**
 * parses a json object
 * @pre pos is at '{'
 */
json::Json parseJsonObj( const std::string &input, std::size_t& pos ) {
    ++pos;      // Move pointer ahead of '{'
    
    json::Json j;
    skip_whitespace(input, pos);
    while ( pos < input.size() && input[pos] != '}' ) {
        skip_whitespace(input, pos);

        // Key must be a string
        if ( pos >= input.size() || input[pos] != '"' ) {
            throw ERROR_MSG;
        }
        const std::string key = parseString(input, pos);

        skip_whitespace(input, pos);

        if ( pos >= input.size() || input[pos] != ':' ) {
            throw ERROR_MSG;
        }
        ++pos;      // move ahead the ':'

        skip_whitespace(input, pos);
        json::JsonValue val = parseValue(input, pos);
        
        
        skip_whitespace(input, pos);
        
        if ( pos >= input.size() || (input[pos] != ',' && input[pos] != '}')) throw ERROR_MSG;
        if ( input[pos] == ',' ) ++pos;  

        j[key] = val;
    }

    if ( pos >= input.size() ) throw ERROR_MSG;
    ++pos;
    return j;
}

json::Json json::parse( const std::string &input )
{
    std::size_t currPos = 0;
    skip_whitespace(input, currPos);  

    if ( currPos >= input.size() || input[currPos] != '{' ) {
        throw ERROR_MSG;
    }

    json::Json res = parseJsonObj(input, currPos);

    skip_whitespace(input, currPos);
    if( currPos != input.size() ) {
        throw ERROR_MSG;
    }

    return res;
}


