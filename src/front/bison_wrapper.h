#pragma once
#include "ast.h"
#include "parser.hpp"

namespace yy {
int
yylex(parser::value_type* yylval, Lexer& lexer);
}