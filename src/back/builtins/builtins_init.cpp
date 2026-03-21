#include "builtins_init.h"
#include "../runtime.h"
#include "array.h"
#include "bool.h"
#include "builtin_fns.h"
#include "number.h"
#include "string.h"

void
builtinsInit(Runtime& runtime)
{
  runtime.declareClass("var");
  runtime.defineVariable("null", runtime.nullObj());

  numberClassInit(runtime);
  boolClassInit(runtime);
  // TODO: fix arrays
  // arrayClassInit(runtime);
  stringClassInit(runtime);

  builtinFunctionsInit(runtime);
}