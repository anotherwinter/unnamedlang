#pragma once
#include "shared.h"
#include <cstdint>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

struct TypeID
{
  using __TypeID = uint32_t;
  // dynamicID value is used for everything uninitialized/unresolved
  static constexpr __TypeID dynamicID = 0;
  // invalidID reserved for future uses
  static constexpr __TypeID invalidID = std::numeric_limits<__TypeID>::max();
  __TypeID val = dynamicID;

  inline bool operator==(const TypeID& other) const { return val == other.val; }

  inline bool operator!=(const TypeID& other) const { return val != other.val; }

  inline bool operator<(const TypeID& other) const { return val < other.val; }

  inline TypeID operator++()
  {
    ++val;
    return *this;
  }

  inline TypeID operator++(int)
  {
    val++;
    return *this;
  }
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
  static constexpr __FnNameID invalidID =
    std::numeric_limits<__FnNameID>::max();
  uint32_t val = std::numeric_limits<__FnNameID>::max();

  inline bool operator==(const FnNameID& other) const
  {
    return val == other.val;
  }

  inline bool operator!=(const FnNameID& other) const
  {
    return val != other.val;
  }

  inline bool operator<(const FnNameID& other) const { return val < other.val; }

  inline FnNameID operator++()
  {
    ++val;
    return *this;
  }

  inline FnNameID operator++(int)
  {
    val++;
    return *this;
  }
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
  static constexpr __FunctionID invalidID =
    std::numeric_limits<__FunctionID>::max();
  uint32_t val = std::numeric_limits<__FunctionID>::max();

  inline bool operator==(const FunctionID& other) const
  {
    return val == other.val;
  }

  inline bool operator!=(const FunctionID& other) const
  {
    return val != other.val;
  }

  inline bool operator<(const FunctionID& other) const
  {
    return val < other.val;
  }

  inline FunctionID operator++()
  {
    ++val;
    return *this;
  }

  inline FunctionID operator++(int)
  {
    val++;
    return *this;
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

struct VarID
{
  using __VarID = uint32_t;
  static constexpr __VarID invalidID = std::numeric_limits<__VarID>::max();
  uint32_t val = std::numeric_limits<__VarID>::max();

  inline bool operator==(const VarID& other) const { return val == other.val; }

  inline bool operator!=(const VarID& other) const { return val != other.val; }

  inline bool operator<(const VarID& other) const { return val < other.val; }

  inline VarID operator++()
  {
    ++val;
    return *this;
  }

  inline VarID operator++(int)
  {
    val++;
    return *this;
  }
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

// reserved fn name ids for operator for each class
enum class OperatorID : FnNameID::__FnNameID
{
  Add = 0,
  Sub,
  Mul,
  Div,
  Mod,
  Shiftl,
  Shiftr,
  And,
  Or,
  Xor,
  Lt,
  Gt,
  Leq,
  Geq,
  Eq,
  Neq,
  LogicAnd,
  LogicOr,
  UnaryNeg,
  UnaryInc,
  UnaryDec,
  _Count
};

struct FunctionInfo
{
  std::vector<std::string> paramNames;
  std::vector<TypeID> paramTypes;

  // id for each function - has to be unique between global functions, but may
  // not be unique between class methods
  const FunctionID id = {};

  const TypeID ownerID = {};

  const TypeID returnType = {};

  const Modifier mod = Modifier::None;

  const FnParamBitmask bitMask = 0;
};

struct VarInfo
{
  // points to variable id for global variables or to index in class for fields
  VarID id = {};
  TypeID type = {};
  Modifier mod = Modifier::None;
};

struct FnNameResolution
{
  FnNameID id = {};
  TypeID ownerID = {};
};

struct FnResolution
{
  FunctionID id = {};
  TypeID ownerID = {};
  TypeID returnType = {};
  Modifier mod = Modifier::None;
};

struct MethodDeclInfo
{
  FnNameID nameID = {};
  FunctionID id = {};
};

struct FieldDeclInfo
{
  std::string name;
  VarInfo info = {};
};

struct FnDeclKey
{
  FunctionID id;
  FnNameID nameID;
};

struct ClassInfo
{
  const TypeID classID = {};
  bool isPrimitive = false;
  bool isValueImmutable = false;
};

struct NameResolution
{
  TypeID ownerID = {};
  std::variant<std::monostate, VarInfo, FnNameID> nameID;
};

inline constexpr bool
isValid(TypeID id)
{
  return id.val != TypeID::dynamicID;
}

inline constexpr bool
isValid(FnNameID id)
{
  return id.val != FnNameID::invalidID;
}

inline constexpr bool
isValid(VarID id)
{
  return id.val != VarID::invalidID;
}

inline constexpr bool
isValid(FunctionID id)
{
  return id.val != FunctionID::invalidID;
}

using ClassesMap = std::unordered_map<TypeID, ClassInfo>;
using NameToTypeIDMap = std::unordered_map<std::string, TypeID>;
using MethodsMap = std::unordered_map<FnNameID, std::vector<FunctionID>>;
using FieldsMap = std::unordered_map<std::string, VarInfo>;

using FnParamCount = uint16_t;
using FunctionsMap = std::unordered_map<
  TypeID,
  std::unordered_map<
    FnNameID,
    std::unordered_map<FnParamCount, std::vector<FunctionInfo*>>>>;
using FunctionsNameToIDMap = std::unordered_map<std::string, FnNameID>;
using FunctionInfoStorage = std::unordered_map<FunctionID, FunctionInfo>;
using VariablesMap = std::unordered_map<VarID, VarInfo>;
using VariableNametoVarIDMap = std::unordered_map<std::string, VarID>;

class Diagnostics;
class Scope;

class SymbolRegistry
{
public:
  SymbolRegistry(Diagnostics& diag);
  ~SymbolRegistry();
  [[nodiscard]] FnDeclKey declareFunction(
    const std::string& name,
    const std::vector<std::string>& paramNames,
    const std::vector<TypeID>& paramTypes,
    TypeID ownerID = {});

  [[nodiscard]] FunctionInfo* resolveFunction(FunctionID id);

  // try to resolve exact function signature
  [[nodiscard]] FnResolution resolveFunction(
    FnNameID nameID,
    const std::vector<TypeID>& paramTypes,
    TypeID ownerID = {});

  // find all overloads, matching with given signature
  [[nodiscard]] std::vector<FnResolution>* resolveFunctionOverloads(
    FnNameID nameID,
    const std::vector<TypeID>& paramTypes,
    TypeID ownerID = {},
    bool exact = false,
    bool single = false);

  [[nodiscard]] NameResolution resolveName(const std::string& name);

  [[nodiscard]] NameResolution resolveVarCurScope(const std::string& name);

  [[nodiscard]] TypeID beginDeclareClass(const std::string& name);

  [[nodiscard]] TypeID finishDeclareClass(
    TypeID classID,
    bool isPrimitive,
    bool isValueImmutable,
    const std::vector<MethodDeclInfo>& methods,
    const std::vector<FieldDeclInfo>& fields);

  [[nodiscard]] TypeID resolveClass(const std::string& name);

  [[nodiscard]] VarID declareVariable(const std::string& name,
                                      TypeID type = {},
                                      Modifier mod = Modifier::None);

  [[nodiscard]] FnNameID resolveMethodName(TypeID ownerID,
                                           const std::string& name);

  [[nodiscard]] VarInfo resolveField(TypeID ownerID, const std::string& name);

  [[nodiscard]] VarInfo* resolveVariable(VarID id);

  [[nodiscard]] VarInfo* resolveField(VarID id, TypeID ownerID);

  // push new scope. optional typeid parameter also declares fields from
  // specified class
  void pushScope(TypeID ownerID = {}, FunctionID fnID = {});
  void popScope();

  bool isGlobalScope();
  
  TypeID getCurScopeOwnerID();

private:
  SymbolRegistry(const SymbolRegistry& other) = delete;
  SymbolRegistry(SymbolRegistry&& other) = delete;

  Diagnostics& _diag;

  FunctionID _fnID = { 0 };
  FnNameID _nameID = { 0 };

  // first class declared/defined will be assigned to dynamic type ID, so it
  // should be dynamic class first
  TypeID _typeID = { dynamicTypeID };

  std::unordered_map<TypeID, MethodsMap> _methods;

  std::unordered_map<TypeID, FieldsMap> _fields;

  std::vector<std::unique_ptr<Scope>> _scopes;

  // defined classes
  ClassesMap _classes;

  NameToTypeIDMap _nameToTypeID;

  // defined functions
  FunctionsMap _functions;

  // map where functioninfo structs stored
  FunctionInfoStorage _fnInfoStorage;

  // cache for conversion function name (string) -> function name ID
  FunctionsNameToIDMap _fnNameToID;

  inline FnParamBitmask makeFnParamBitmask(
    const std::vector<TypeID>& paramTypes)
  {
    FnParamBitmask bitmask = 0;
    uint16_t pos = 0;

    for (auto& id : paramTypes) {
      if (id.val != dynamicTypeID.val)
        bitmask |= (1 << pos);

      pos <<= 1;
    }

    return bitmask;
  }

  inline bool areTypesCompatible(TypeID a, TypeID b, bool exact = false)
  {
    if (!exact && (a == dynamicTypeID || b == dynamicTypeID))
      return true;

    return a == b;
  }

  void insertFunctionOverload(FunctionInfo& fnInfo,
                              std::vector<FunctionInfo*>& overloads);
};