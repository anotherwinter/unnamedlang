#pragma once
#include "symbolregistry.h"
#include "typedast.h"

class SymbolRegistry;
class Diagnostics;

namespace HIR {

struct NameResolutionResult
{
  TypeID ownerID = {};
  TypeID type = {};
  std::variant<std::monostate,
               TypeID,        // holds class id
               FnNameID,      // holds fnnameid
               FnResolution,  // holds functionid
               VarResolution, // holds varid for var / field
               TypedNode      // unresolved
               >
    res;
  bool resolved = false;
};

class TypedASTAnalyzer
{
public:
  TypedASTAnalyzer(Diagnostics& diag, SymbolRegistry& reg);
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

  TypeID _boolType;
  TypeID _numberType;
  TypeID _stringType;
  TypeID _arrayType;

  void initPrebuilts();

  // pass 2 - semantic checks, resolution
  void analyzeTypedNode(const TypedNode* node);

  // pass2 methods
  void analyzeNodeList(const NodeList& list);
  void analyzeFnDef(const TypedNode* node);
  void analyzeCallExpr(const TypedNode* node);
  void analyzeClassDef(const TypedNode* node);
  void analyzeEnumDef(const TypedNode* node);
  void analyzeVarDecl(const TypedNode* node);
  void analyzeVarAssign(const TypedNode* node);
  void analyzeWhl(const TypedNode* node);
  void analyzeFor(const TypedNode* node);
  void analyzeIf(const TypedNode* node);
  void analyzeSwitch(const TypedNode* node);
  void analyzeRet(const TypedNode* node);
  void analyzeBrk(const TypedNode* node);
  void analyzeBinaryExpr(const TypedNode* node);
  void analyzeUnaryExpr(const TypedNode* node);
  void analyzeMemberAccess(const TypedNode* node);
  void analyzeArrayAccess(const TypedNode* node);
  void analyzeBool(const TypedNode* node);
  void analyzeNumber(const TypedNode* node);
  void analyzeString(const TypedNode* node);
  void analyzeArray(const TypedNode* node);
  void analyzeName(const TypedNode* node);

  NameResolutionResult resolve(TypedNode* node);
  NameResolutionResult resolveCallExpr(CallExpr& callExpr);
  NameResolutionResult resolveMemberAccess(MemberAccess& membAccess);
  NameResolutionResult resolveArrayAccess(ArrayAccess& arrAccess);
  NameResolutionResult resolveNameExpr(NameExpr& nameExpr);

  inline TypeID getType(NameResolutionResult res)
  {
    if (!res.resolved)
      return {};

    if (auto varRes = std::get_if<VarResolution>(&res.res))
      return varRes->varInfo.type;
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
};

}