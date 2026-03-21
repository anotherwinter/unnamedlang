#pragma once
#include "back/runtime.h"
#include "back/treewalk.h"
#include "diagnostics.h"
#include "front/lexer.h"

class Initializer
{
public:
  Initializer();

  inline Lexer& getLexer() { return _lex; }
  inline Diagnostics& getDiag() { return _diag; }
  inline Runtime& getRuntime() { return _runtime; }
  inline Treewalk& getTreewalk() { return _treewalk; }

private:
  Diagnostics _diag;
  Lexer _lex;
  Runtime _runtime;
  Treewalk _treewalk;
};