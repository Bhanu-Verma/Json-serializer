#ifndef PARSER_H
#define PARSER_H

#include "json_lib.hpp"

namespace json {
    Json parse( const std::string& str );
};  

#endif