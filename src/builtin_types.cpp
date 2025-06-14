#include "builtin_types.h"
#include "builtin/array.h"
#include "builtin/bool.h"
#include "builtin/context.h"
#include "builtin/function.h"
#include "builtin/lambda.h"
#include "builtin/number.h"
#include "builtin/string.h"

void
builtinTypesInit()
{
  arrayClassInit();
  boolClassInit();
  contextClassInit();
  functionClassInit();
  lambdaClassInit();
  numberClassInit();
  stringClassInit();
}