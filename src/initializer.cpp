#include "initializer.h"
#include "diagnostics.h"

Initializer::Initializer()
  : _diag(Diagnostics::DiagnosticsKey())
  , _lex(_diag)
  , _parser(_lex, _parserCtx)
  , _typedAST(_diag)
{
}