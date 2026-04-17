#ifndef PARSER_H
#define PARSER_H

#include "json_lib.hpp"

namespace json {
    Json parse( std::string_view str );
};  

#endif