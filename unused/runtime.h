// #pragma once
// #include "../front/shared.h"
// #include "buddy.h"
// #include <cstddef>
// #include <cstdint>
// #include <functional>
// #include <map>
// #include <setjmp.h>
// #include <string>
// #include <unordered_map>
// #include <variant>
// #include <vector>

// struct ClassInfo;
// enum class OpType : uint16_t;
// enum class ContextFlags : uint16_t;

// enum class ValueType
// {
//   None,
//   Bool,
//   Number,
//   String,
//   Null,
//   Array,
//   // TODO: Lambda,
//   Instance
// };

// struct Identifier;

// struct TypeID
// {
//   using __TypeID = uint32_t;
//   static constexpr __TypeID __dynamicTypeIDVal = 0;
//   __TypeID val = __dynamicTypeIDVal;

//   inline bool operator==(const TypeID& other) const { return val == other.val; }

//   inline bool operator<(const TypeID& other) const { return val < other.val; }
// };

// // type ID 0 is reserved for dynamic type
// constexpr TypeID dynamicTypeID = { 0 };

// namespace std {
// template<>
// struct hash<TypeID>
// {
//   size_t operator()(const TypeID& x) const noexcept
//   {
//     return std::hash<TypeID::__TypeID>{}(x.val);
//   }
// };
// }

// struct ObjectHeader
// {
//   TypeID objClass;
//   bool isConst;
//   bool isOnHeap;
//   ValueType valueType;
//   // actual object data
//   union
//   {
//     // primitive types
//     bool boolValue;
//     double numValue;
//     const char* stringValue;

//     // non-primitive builtin types
//     std::byte* builtinPayload;
//     // class instance data
//     Identifier* instancePayload;
//   } data;
// };

// struct Array
// {
//   uint64_t size;
//   Identifier* objects;
// };

// struct ASTNode;

// // struct Lambda
// // {
// //   ASTNode* entry;
// //   std::map<std::string, ObjectHeader*>* captureList;
// //   std::vector<ParameterInfo>* params;
// // };

// struct ExHandler
// {
//   ASTNode* entry;
//   int exType;
// };

// class Runtime;
// using BuiltinFunction =
//   std::function<ObjectHeader(Runtime&, const std::vector<ObjectHeader>&)>;
// using FnParamBitmask = uint16_t;
// using FunctionID = uint32_t;

// struct FunctionInfo
// {
//   const char* name;
//   bool isBuiltin;
//   union
//   {
//     const ASTNode* entry;
//     BuiltinFunction cb;
//   } code;
//   FnParamBitmask bm;
//   std::vector<const char*>* paramNames;
//   std::vector<TypeID>* paramTypes;

//   // id for each function - has to be unique between global functions, but may
//   // not be unique between class methods
//   FunctionID id;
// };

// enum class NameType
// {
//   None,
//   FunctionNameID,
//   ClassID,
//   Object,
//   Field,
//   Method,
// };

// struct ClassMethod
// {
//   Modifier accessibility;
//   const char* name;
//   const bool isBuiltin;
//   const std::vector<const char*>* paramNames;
//   const std::vector<TypeID>* paramTypes;
//   const ASTNode* entry;
//   const BuiltinFunction builtin;
// };

// struct ClassField
// {
//   const char* name;
//   Modifier accessibility;
//   size_t fieldOffset;
// };

// using FnParamCount = uint16_t;

// // nameid for name (string) of defined function or method
// struct FnNameID
// {
//   using __FnNameID = uint32_t;
//   uint32_t val;

//   inline bool operator==(const FnNameID& other) const
//   {
//     return val == other.val;
//   }

//   inline bool operator<(const FnNameID& other) const { return val < other.val; }
// };

// namespace std {
// template<>
// struct hash<FnNameID>
// {
//   size_t operator()(const FnNameID& x) const noexcept
//   {
//     return std::hash<FnNameID::__FnNameID>{}(x.val);
//   }
// };
// }

// using FunctionsMap = std::unordered_map<
//   FnNameID,
//   std::unordered_map<FnParamCount, std::vector<FunctionInfo*>>>;
// using FunctionsNameToIDMap = std::unordered_map<std::string, FnNameID>;

// using FieldsMap = std::map<std::string, ClassField>;
// using ObjectsMap = std::map<std::string, Identifier>;
// using MethodsModifiersMap = std::unordered_map<FunctionID, Modifier>;

// // class type info (name, its members)
// struct ClassInfo
// {
//   const char* name;
//   bool isPrimitive;
//   bool isBuiltin;
//   FunctionsMap* methods;
//   FunctionsNameToIDMap* mhtdNameToID;
//   MethodsModifiersMap* mthdIDToModifier;
//   FieldsMap* fields;
//   ObjectsMap* staticFieldsValues;
//   // size of instance payload (memory chunk for fields values) in bytes
//   const size_t payloadSize;
//   const TypeID typeID;
// };

// // generic identifier, which may be metadata or object
// struct Identifier
// {
//   // handle for object location
//   uint32_t locationHandle = 0;

//   using NameVariant = std::variant<std::monostate,
//                                    FnNameID,
//                                    TypeID,
//                                    FunctionInfo*,
//                                    ClassInfo*,
//                                    ObjectHeader>;

//   NameVariant name;
// };

// struct StringInfo
// {
//   const char* str;
//   size_t refcount;
// };

// class Treewalk;
// class Context;
// class TopLevel;

// class Runtime
// {
// public:
//   Runtime();

//   inline void setTreewalk(Treewalk* treewalk) { _treewalk = treewalk; }

//   inline TypeID findClass(const char* name)
//   {
//     Identifier id = lookup(name);
//     auto classID = std::get_if<TypeID>(&id.name);
//     if (!classID) {
//       error("findClass: unknown class %s\n", name);
//       return dynamicTypeID;
//     }

//     return *classID;
//   }

//   void* alloc(size_t n);

//   // check if two objects are pointing to the same memory (for non-primitives)
//   // or are equal (for primitives)
//   bool compareObjects(ObjectHeader& A, ObjectHeader& B);

//   bool defineFunction(const char* name,
//                       bool isBuiltin,
//                       const std::vector<const char*>& paramNames,
//                       const std::vector<TypeID>& paramTypes,
//                       ASTNode* entry,
//                       const BuiltinFunction builtin);

//   bool defineClass(
//     const char* name,
//     bool isPrimitive,
//     bool isBuiltin,
//     const std::vector<ClassMethod>& methods,
//     const std::vector<ClassField>& fields,
//     const std::map<std::string, Identifier>& staticFieldsValues);

//   // fwd declaration of class
//   bool declareClass(const char* name);

//   // try to define variable in current context
//   bool defineVariable(const char* name, Identifier id);

//   // try to get interned string for given char pointer
//   const char* getInternedString(const char* str);

//   // try to free memory for interned string
//   void releaseInternedString(const char* str);

//   void setLastEval(Identifier id);
//   Identifier getLastEval();

//   // search for given name. does full lookup for variables/classes/etc, but
//   // partial lookup for functions
//   Identifier lookup(const char* name);

//   // resolve function given id and list of parameter types
//   Identifier resolveFunction(FnNameID id, std::vector<TypeID>& paramTypes);

//   // search for given name inside of a class, identifier must be object (class
//   // instance) for looking up instance fields or typeid for everything else
//   // does only partial lookup for methods
//   Identifier lookupInClass(const char* name, Identifier id);

//   // create/update cached map entry for given id
//   void cacheLookup(Identifier* id);

//   void printObject(const ObjectHeader& hdr);

//   template<size_t N, typename... Args>
//   void error(const char (&fmt)[N], Args&&... args)
//   {
//     std::fprintf(stderr, fmt, std::forward<Args>(args)...);

//     setExceptionFlag();
//     runtimeRecover();
//   }

//   // if currently has unhandled exception
//   bool hasUnhandledException() const;

//   // get current context flags
//   ContextFlags getContextFlags() const;

//   // set flag for current context
//   void setContextFlag(ContextFlags flag);

//   // reset flag for current context
//   void resetContextFlag(ContextFlags flag);

//   // call function/class method, identifier is passed if calling class instance
//   // method or static method
//   Identifier call(FnNameID fnNameID,
//                   const std::vector<ObjectHeader>& args,
//                   Identifier id = { 0, std::monostate{} });

//   // try to return from fn using longjmp
//   void returnFromFunction();

//   // try to assign value to identifier using location handle
//   bool tryAssign(uint32_t handle, Identifier value);

//   Identifier accessArray(Identifier array, size_t idx);

//   ObjectHeader invalidObjectHeader() const;

//   bool isPositiveInteger(double x) const;

//   OpType doubleToOpType(double x);

//   bool isPrimitive(TypeID classID);

//     ClassInfo* getClassInfo(TypeID classID);

//   // default object builders
//   Identifier nullObj();
//   Identifier boolObj(bool value);
//   Identifier numberObj(double num);
//   Identifier stringObj(const char* str, bool duplicate);
//   Identifier arrayObj(const std::vector<ObjectHeader>& elems);
//   Identifier classInstance(TypeID classID,
//                            const std::vector<ObjectHeader>& ctorArgs);

// private:
//   Context* _current = nullptr;
//   Context* _global = nullptr;
//   Treewalk* _treewalk = nullptr;
//   std::unordered_map<std::string, StringInfo> _internedStrings;

//   TopLevel* _topLevel;

//   // cache for lookups, value pointing to original identifier which resides
//   // somewhere
//   std::unordered_map<uint32_t, Identifier*> _lookupCache;
//   uint32_t _lookupHandleID;

//   // arena for storing context headers
//   BuddyAlloc _ctxArena;

//   // arena for storing interned strings
//   BuddyAlloc _stringsArena;

//   // jmp_buf for function calls
//   jmp_buf _env;

//   static constexpr uint16_t _maxFnParameters = 0;

//   const char* duplicateString(const char* str);

//   void contextPush(const std::unordered_map<std::string, Identifier>& table,
//                    ContextFlags flags,
//                    ClassInfo* clsInfo);

//   void contextPop();

//   // recover from exception to last safe point, requires UNHANDLED_EX flag
//   void runtimeRecover();

//   // check if given class member is visible in current context
//   bool isCurrentlyVisible(Identifier memb, TypeID receiverClassID);

//   // check if given object header is movable or copyable (non-primitive vs
//   // primitive)
//   bool isMovable(ObjectHeader hdr) const;

//   // duplicate primitive type object
//   Identifier duplicateObject(Identifier id);

//   void setExceptionFlag();

//   Identifier callFunction(FunctionInfo* fn,
//                           const std::vector<ObjectHeader>& args,
//                           ClassInfo* clsInfo);

//   Identifier lookupGlobalUncached(const char* name);

//   void defineClass(ClassInfo* cls);

//   void defineFunction(FunctionInfo* fn);
// };
