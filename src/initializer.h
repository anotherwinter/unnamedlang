#pragma once
#include "diagnostics.h"
#include "front/lexer.h"
#include "front/parser.hpp"

class Initializer
{
public:
  Initializer();

  inline Diagnostics& getDiag() { return _diag; }
  inline Lexer& getLexer() { return _lex; }
  inline yy::parser& getParser() { return _parser; }

private:
  Diagnostics _diag;
  Lexer _lex;
  yy::parser _parser;
  ParserContext _parserCtx = {};
};