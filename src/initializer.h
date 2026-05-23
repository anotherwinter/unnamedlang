#pragma once
#include "diagnostics.h"
#include "front/lexer.h"
#include "front/parser.hpp"
#include "front/typedast.h"
#include "middle/tacbuilder.h"

class Initializer
{
public:
  Initializer();

  inline Diagnostics& getDiag() { return _diag; }
  inline Lexer& getLexer() { return _lex; }
  inline yy::parser& getParser() { return _parser; }
  inline HIR::TypedAST& getTypedAST() { return _typedAST; }
  inline MIR::TACBuilder& getTACBuilder() { return _tacBuilder; }

private:
  Diagnostics _diag;
  Lexer _lex;
  yy::parser _parser;
  ParserContext _parserCtx = {};
  HIR::TypedAST _typedAST;
  MIR::TACBuilder _tacBuilder;
};