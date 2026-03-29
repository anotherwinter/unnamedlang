#pragma once
#include "shared.h"
#include <cstdint>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

struct TypeID
{
  using __TypeID = uint32_t;
  static constexpr __TypeID __dynamicTypeIDVal = 0;
  __TypeID val = __dynamicTypeIDVal;

  inline bool operator==(const TypeID& other) const { return val == other.val; }

  inline bool operator<(const TypeID& other) const { return val < other.val; }
};

// type ID 0 is reserved for dynamic type
constexpr TypeID dynamicTypeID = { 0 };

namespace std {
template<>
struct hash<TypeID>
{
  size_t operator()(const TypeID& x) const noexcept
  {
    return std::hash<TypeID::__TypeID>{}(x.val);
  }
};
}

// nameid for name (string) of defined function or method
struct FnNameID
{
  using __FnNameID = uint32_t;
  uint32_t val;

  inline bool operator==(const FnNameID& other) const
  {
    return val == other.val;
  }

  inline bool operator<(const FnNameID& other) const { return val < other.val; }
};

namespace std {
template<>
struct hash<FnNameID>
{
  size_t operator()(const FnNameID& x) const noexcept
  {
    return std::hash<FnNameID::__FnNameID>{}(x.val);
  }
};
}

using FnParamBitmask = uint16_t;

struct FunctionID
{
  using __FunctionID = uint32_t;
  uint32_t val;

  inline bool operator==(const FunctionID& other) const
  {
    return val == other.val;
  }

  inline bool operator<(const FunctionID& other) const
  {
    return val < other.val;
  }
};

namespace std {
template<>
struct hash<FunctionID>
{
  size_t operator()(const FunctionID& x) const noexcept
  {
    return std::hash<FunctionID::__FunctionID>{}(x.val);
  }
};
}

struct FunctionInfo
{
  std::vector<std::string> paramNames;
  std::vector<TypeID> paramTypes;

  // id for each function - has to be unique between global functions, but may
  // not be unique between class methods
  const FunctionID id;

  const TypeID ownerID;
};

using FnParamCount = uint16_t;
using FunctionsMap = std::unordered_map<
  TypeID,
  std::unordered_map<
    FnNameID,
    std::unordered_map<FnParamCount, std::vector<FunctionInfo*>>>>;
using FunctionsNameToIDMap = std::unordered_map<std::string, FnNameID>;

struct MethodInfo
{
  Modifier accessibility;
  FunctionID id;
};

struct FieldInfo
{
  Modifier accessibility;
};

struct MethodDeclInfo
{
  std::string name;
  MethodInfo info;
};

struct FieldDeclInfo
{
  std::string name;
  FieldInfo info;
};

struct ClassInfo
{
  const TypeID classID;
  bool isPrimitive;
  bool isValueImmutable;
};

struct VarID
{
  using __VarID = uint32_t;
  uint32_t val;

  inline bool operator==(const VarID& other) const { return val == other.val; }

  inline bool operator<(const VarID& other) const { return val < other.val; }
};

namespace std {
template<>
struct hash<VarID>
{
  size_t operator()(const VarID& x) const noexcept
  {
    return std::hash<VarID::__VarID>{}(x.val);
  }
};
}

struct VarInfo
{
  const VarID id;
  TypeID type;
  Modifier mod;
};

using ClassesMap = std::unordered_map<TypeID, ClassInfo>;
using NameToTypeIDMap = std::unordered_map<std::string, TypeID>;
using MethodsMap = std::unordered_map<FunctionID, MethodInfo>;
using FieldsMap = std::unordered_map<VarID, FieldInfo>;

constexpr TypeID classInvalidID = std::numeric_limits<TypeID>::max();
constexpr FnNameID fnNameInvalidID = std::numeric_limits<FnNameID>::max();
constexpr VarID varInvalidID = std::numeric_limits<VarID>::max();
constexpr FunctionID fnInvalidID = std::numeric_limits<FunctionID>::max();

constexpr bool
isValid(TypeID id)
{
  return id.val != classInvalidID.val;
}
constexpr bool
isValid(VarID id)
{
  return id.val != varInvalidID.val;
}
constexpr bool
isValid(FunctionID id)
{
  return id.val != fnInvalidID.val;
}

class Diagnostics;

class SymbolRegistry
{
public:
  SymbolRegistry(Diagnostics& diag);
  [[nodiscard]] FunctionID declareFunction(
    const std::string& name,
    const std::vector<std::string>& paramNames,
    const std::vector<TypeID>& paramTypes,
    TypeID ownerID = dynamicTypeID);

  [[nodiscard]] TypeID beginDeclareClass(const std::string& name);

  [[nodiscard]] TypeID finishDeclareClass(
    TypeID classID,
    bool isPrimitive,
    bool isValueImmutable,
    const std::vector<MethodDeclInfo>& methods,
    const std::vector<FieldDeclInfo>& fields);

  [[nodiscard]] TypeID resolveClass(const std::string& name);

  [[nodiscard]] VarID declareVariable(const std::string& name,
                                      TypeID type,
                                      Modifier mod);

  [[nodiscard]] VarInfo resolveVariable(const std::string& name);

  void pushScope();
  void popScope();

private:
  SymbolRegistry(const SymbolRegistry& other) = delete;
  SymbolRegistry(SymbolRegistry&& other) = delete;

  FunctionID _fnID = { 0 };

  // first class declared/defined will be assigned to dynamic type ID, so it
  // should be dynamic class first
  TypeID _typeID = dynamicTypeID;

  std::unordered_map<TypeID, MethodsMap> _methods;

  std::unordered_map<TypeID, FieldsMap> _fields;

  std::vector<std::unordered_map<std::string, VarInfo>> _scopes;

  // defined classes
  ClassesMap _classes;

  NameToTypeIDMap _nameToTypeID;

  // defined functions
  FunctionsMap _functions;

  // cache for conversion function name (string) -> function ID
  FunctionsNameToIDMap _fnNameToID;
};