#pragma once
#include "front/nodeallocator.h"
#include "front/symbolregistry.h"
#include "front/typedast.h"

class SymbolRegistry;
class Diagnostics;

namespace HIR {

struct NameResolutionResult
{
  TypeID ownerID = {};
  TypeID type = {};
  std::variant<std::monostate,
               TypeID,       // holds class id
               FnNameID,     // holds fnnameid
               FnResolution, // holds functionid
               VarInfo,      // holds varinfo
               TypedNode     // unresolved
               >
    res;
  bool resolved = false;
};

class TypedASTAnalyzer
{
public:
  TypedASTAnalyzer(Diagnostics& diag,
                   SymbolRegistry& reg,
                   NodeAllocator& alloc);
  inline TypedTree<Analyzed> analyze(TypedTree<Unanalyzed> root)
  {
    analyzeTypedNode(root.root);
    return { root.root };
  }

  // TODO: this is temporary, refactor this
  TypeID inferType(TypedNode* node);

private:
  TypedASTAnalyzer(const TypedASTAnalyzer& other) = delete;
  TypedASTAnalyzer(TypedASTAnalyzer&& other) = delete;

  Diagnostics& _diag;
  SymbolRegistry& _reg;
  NodeAllocator& _alloc;

  TypeID _boolType;
  TypeID _numberType;
  TypeID _stringType;
  TypeID _arrayType;

  // if set, dont try to inject "self." for fields when analyzing
  bool _ignoreSelfInjecting;

  void initPrebuilts();

  // pass 2 - semantic checks, resolution
  void analyzeTypedNode(TypedNode* node);

  // pass2 methods
  void analyzeNodeList(const NodeList& list);
  void analyzeFnDef(TypedNode* node);
  void analyzeCallExpr(TypedNode* node);
  void analyzeClassDef(TypedNode* node);
  void analyzeEnumDef(TypedNode* node);
  void analyzeVarDecl(TypedNode* node);
  void analyzeVarAssign(TypedNode* node);
  void analyzeWhl(TypedNode* node);
  void analyzeFor(TypedNode* node);
  void analyzeIf(TypedNode* node);
  void analyzeSwitch(TypedNode* node);
  void analyzeRet(TypedNode* node);
  void analyzeBrk(TypedNode* node);
  void analyzeBinaryExpr(TypedNode* node);
  void analyzeUnaryExpr(TypedNode* node);
  void analyzeMemberAccess(TypedNode* node);
  void analyzeArrayAccess(TypedNode* node);
  void analyzeBool(TypedNode* node);
  void analyzeNumber(TypedNode* node);
  void analyzeString(TypedNode* node);
  void analyzeArray(TypedNode* node);
  void analyzeName(TypedNode* node);

  NameResolutionResult resolve(TypedNode* node);
  NameResolutionResult resolveCallExpr(CallExpr& callExpr);
  NameResolutionResult resolveMemberAccess(MemberAccess& membAccess);
  NameResolutionResult resolveArrayAccess(ArrayAccess& arrAccess);
  NameResolutionResult resolveNameExpr(NameExpr& nameExpr);
  NameResolutionResult resolveSelfExpr();

  inline TypeID getType(NameResolutionResult res)
  {
    if (!res.resolved)
      return {};

    if (auto varRes = std::get_if<VarInfo>(&res.res))
      return varRes->type;
    else if (auto fnRes = std::get_if<FnResolution>(&res.res))
      return fnRes->returnType;

    return {};
  }

  TypeID typeOfCallExpr(CallExpr& callExpr);
  TypeID typeOfBinaryExpr(BinaryExpr& binary);
  TypeID typeOfUnaryExpr(UnaryExpr& unary);
  TypeID typeOfMemberAccess(MemberAccess& membAccess);
  TypeID typeOfArrayAccess(ArrayAccess& arrAccess);
  TypeID typeOfBool(BoolVal& val);
  TypeID typeOfNumber(NumVal& val);
  TypeID typeOfString(StringVal& val);
  TypeID typeOfArray(ArrayVal& val);
  TypeID typeOfName(NameExpr& nameExpr);
  TypeID typeOfSelfExpr(SelfExpr& selfExpr);

  bool isPrimitiveType(TypeID type);
  bool shouldInjectSelf(TypeID ownerID);
};

}