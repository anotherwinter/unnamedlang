// #pragma once
// #include "runtime.h"

// using ClassesMap = std::map<TypeID, ClassInfo*>;
// using NameToTypeIDMap = std::map<std::string, TypeID>;

// // helper for top-level - classes and functions (without top-level variables)
// class TopLevel
// {
// public:
//   TopLevel(Runtime& runtime);
//   ~TopLevel();

//   bool defineFunction(const char* name,
//                       bool isBuiltin,
//                       const std::vector<const char*>& paramNames,
//                       const std::vector<TypeID>& paramTypes,
//                       ASTNode* entry,
//                       const BuiltinFunction builtin);

//   bool defineClass(const char* name,
//                    bool isPrimitive,
//                    bool isBuiltin,
//                    const std::vector<ClassMethod>& methods,
//                    const std::vector<ClassField>& fields,
//                    const std::map<std::string, Identifier>& staticFieldsValues);

//   bool declareClass(const char* name);

//   // lookup for class or function, does partial lookup (only indicates type of
//   // result, without actual payload) for functions.
//   Identifier lookup(const char* name);

//   // resolve function given id and list of parameter types
//   Identifier resolveFunction(FnNameID id,
//                              std::vector<TypeID>& paramTypes,
//                              TypeID classID = dynamicTypeID);

//   // search for given name inside of a class, identifier must be object (class
//   // instance) for looking up instance fields or class info for everything else
//   // does only partial lookup for methods
//   Identifier lookupInClass(const char* name, Identifier id);

//   ClassInfo* getClassInfo(TypeID classID);

//   Modifier getMethodModifier(TypeID classID, FunctionID mthdID);

//   FnParamBitmask makeParamBitmask(std::vector<TypeID>& paramTypes);

// private:
//   Runtime& _runtime;

//   // name ID for function names
//   FnNameID _fnNameID = { 0 };

//   FunctionID _fnID = 0;

//   // first class declared/defined will be assigned to dynamic type ID, so it
//   // should be dynamic class first
//   TypeID _typeID = dynamicTypeID;

//   // defined classes
//   ClassesMap* _classes;

//   NameToTypeIDMap* _nameToTypeID;

//   // defined functions
//   FunctionsMap* _functions;

//   // cache for conversion function name (string) -> function ID
//   FunctionsNameToIDMap* _fnNameToID;

//   // arena for storing classinfo and functioninfo
//   BuddyAlloc _toplevelArena;

//   FunctionInfo* createFunctionInfo(const char* name,
//                                    bool isBuiltin,
//                                    const std::vector<const char*>& paramNames,
//                                    const std::vector<TypeID>& paramTypes,
//                                    const ASTNode* entry,
//                                    const BuiltinFunction builtin);

//   ClassInfo* createClassInfo(
//     const char* name,
//     bool isPrimitive,
//     bool isBuiltin,
//     const std::vector<ClassMethod>& methods,
//     const std::vector<ClassField>& fields,
//     const std::map<std::string, Identifier>& staticFieldsValues);

//   bool defineFunction(FunctionInfo* fn,
//                       FunctionsMap& fnMap,
//                       FunctionsNameToIDMap& fnNameToIDMap);

//   void insertFunctionOverload(std::vector<FunctionInfo*>& overloads,
//                               FunctionInfo* fn);

//   // check if A is compatible with B (relatively to A, B could be more loose on
//   // bitmask)
//   bool isCompatibleOverload(FnParamBitmask bmA,
//                             std::vector<TypeID>& typesA,
//                             FnParamBitmask bmB,
//                             std::vector<TypeID>& typesB);
// };