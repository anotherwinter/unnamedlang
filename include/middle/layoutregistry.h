#pragma once
#include "front/symbolregistry.h"
#include "shared.h"
#include "middle/tacbuilder.h"
#include <unordered_map>

namespace MIR
{

  struct FunctionLayout
  {
    // block that contains first instructions
    Block begin;
  };

  struct FieldLayout
  {
    TypeID type;
    Modifier mod;
  };

  struct ClassLayout
  {
    bool isPrimitive = false;
    std::vector<FieldLayout> fields;
  };

  using FunctionLayoutMap = std::unordered_map<FunctionID, FunctionLayout>;
  using ClassLayoutMap = std::unordered_map<TypeID, ClassLayout>;

  class LayoutRegistry
  {
  public:
    LayoutRegistry(SymbolRegistry& symReg);

  private:
    LayoutRegistry(const LayoutRegistry& other) = delete;
    LayoutRegistry(LayoutRegistry&& other) = delete;

    FunctionLayout _fnLayoutMap;
    ClassLayoutMap _classLayoutMap;
    SymbolRegistry& _symReg;
  };

}