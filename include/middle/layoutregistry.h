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
    BlockID begin;
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
    LayoutRegistry();
    ~LayoutRegistry();

    inline void addFnLayout(FunctionID id, FunctionLayout layout) {
      _fnLayoutMap.emplace(id, layout);
    }

  private:
    LayoutRegistry(const LayoutRegistry& other) = delete;
    LayoutRegistry(LayoutRegistry&& other) = delete;

    FunctionLayoutMap _fnLayoutMap;
    ClassLayoutMap _classLayoutMap;
  };

}