#include "initializer.h"
#include "back/treewalk.h"
#include "diagnostics.h"

Initializer::Initializer()
  : _diag(Diagnostics::DiagnosticsKey())
  , _lex(_diag), _runtime(), _treewalk(_runtime)
{
    _runtime.setTreewalk(&_treewalk);
}