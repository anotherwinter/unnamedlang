#pragma once
#include "ast.h"
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

struct Object;

typedef void (*BuiltinFunction)(std::vector<Object*>*);

// struct which holds string pointer
typedef struct alignas(8)
{
  const char* str;
  bool isInterned;
} StringValue;

// struct that represents object
typedef struct alignas(8) Object
{
  size_t refcount;
  const char* type;
  bool isConst;
  bool isCompound;
  union
  {
    // map is used only if object is compound
    std::unordered_map<std::string, Object*>* objectFields;
    // if its a simple object (number, string) use only one pointer to actual
    // struct
    void* objectPtr;
  } data;
} Object;

typedef struct alignas(8)
{
  double value;
} Number;

typedef struct alignas(8)
{
  const char* value;
} String;

typedef struct alignas(8)
{
  bool value;
} Bool;

typedef struct alignas(8)
{
  const char* name;
  const char* type;
} ParameterInfo;

typedef struct alignas(8)
{
  std::vector<Object*>* objects;
} Array;

typedef struct alignas(8)
{
  bool isBuiltin;
  union
  {
    ASTNode* entry;
    BuiltinFunction cb;
  } code;
  std::vector<ParameterInfo>* params;
} Function;

typedef struct alignas(8)
{
  ASTNode* entry;
  std::map<std::string, Object*>* captureList;
  std::vector<ParameterInfo>* params;
} Lambda;

typedef struct alignas(8)
{
  ASTNode* entry;
  int exType;
} ExHandler;

// these objects are not in default symbolic tables and their memory managed
// differently

// class member (name, current value)
typedef struct alignas(8)
{
  const char* name;
  Object* value;
  Modifier accessibility;
} ClassMember;

// class type info (name, its members)
typedef struct alignas(8)
{
  const char* name;
  // if primitive type
  bool isPrimitive;
  std::map<std::string, std::map<std::string, ClassMember>>* methods;
  std::map<std::string, ClassMember>* fields;
} ClassInfo;

// default object "ctors"
Object*
nullObj();
Object*
boolObj(bool value);
Object*
numberObj(double num);
Object*
stringObj(const char* str, bool duplicate);
Object*
arrayObj(const std::vector<Object*>& elems);
Object*
functionObj(ASTNode* entry, const std::vector<ParameterInfo>& params);
Object*
lambdaObj(ASTNode* entry,
          const std::map<std::string, Object*>& captureList,
          const std::vector<ParameterInfo>& params);
Object*
classInstance(const char* className, const std::vector<Object*>& ctorArgs);
Object*
builtinFnObj(const std::vector<ParameterInfo>& params, BuiltinFunction cb);

// empty dynamically typed uninitialized object
Object*
dynamicObj();