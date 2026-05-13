// #include "array.h"
// #include "../runtime.h"
// #include <cstring>

// ObjectHeader
// arrayCtor(Runtime& runtime, const std::vector<ObjectHeader>& args)
// {
//   double count = args.at(0).data.numValue;
//   if (!runtime.isPositiveInteger(count)) {
//     runtime.error("Array(): elements count has to be positive integer");
//     return runtime.invalidObjectHeader();
//   } else
//     return runtime
//       .arrayObj({ static_cast<size_t>(count), runtime.nullObj().object })
//       .object;
// }

// void
// arrayClassInit(Runtime& runtime)
// {
//   std::vector<ParameterInfo> bnCtorParams = { { "count", "Number" } };
//   Modifier ctorMod = Modifier::None;

//   // build class method for ctor
//   FunctionInfo* ctorFn =
//     runtime.createFunctionInfo("Array", true, bnCtorParams, nullptr, arrayCtor);
//   ClassMember builtinCtor = { ctorFn->name, ctorMod, NameType::Method };
//   builtinCtor.member.mthdFn = ctorFn;

//   Identifier classId = { NameType::Class };
//   classId.name.classInfo =
//     runtime.createClassInfo("Array", false, true, { builtinCtor }, {});
//   runtime.define(classId.name.classInfo->name, classId);
// }