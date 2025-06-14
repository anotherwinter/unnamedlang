#include "context.h"
#include "../runtime.h"

void
setExceptionHandler(std::vector<Object*>* args)
{
  Object* lambdaObj = args->at(0);
  contextSetExHandler(lambdaObj);
}

void
contextClassInit()
{
  Object* setHandlerFn =
    builtinFnObj({ { "handler", "Lambda" } }, setExceptionHandler);
  ClassMember setHandlerBuiltin = { "setExceptionHandler",
                                    setHandlerFn,
                                    (Modifier)(MOD_HIDDEN | MOD_STATIC) };

  registerClass("Context", true, { setHandlerBuiltin });
  // set class default ctor and dtor as hidden
  ClassInfo* info = contextLookupClass("Context");
  info->methods->at("ctor")["fn_ctor"].accessibility = MOD_HIDDEN;
  info->methods->at("dtor")["fn_dtor"].accessibility = MOD_HIDDEN;
}