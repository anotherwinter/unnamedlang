#include "front/typedast.h"
#include "front/symbolregistry.h"
#include "front/typedastanalyzer.h"
#include "front/typedastbuilder.h"
#include <cstdio>

using namespace HIR;

TypedAST::TypedAST(Diagnostics& diag)
  : _diag(diag)
{
  _reg = std::make_unique<SymbolRegistry>(_diag);
  _builder = std::make_unique<TypedASTBuilder>(_diag, *_reg);
  _analyzer = std::make_unique<TypedASTAnalyzer>(_diag, *_reg);
}

TypedAST::~TypedAST() {}

TypedTree<Unanalyzed>
TypedAST::build(const ASTNode* root)
{
  return _builder->build(root);
}

TypedTree<Analyzed>
TypedAST::analyze(TypedTree<Unanalyzed> root)
{
  return _analyzer->analyze(root);
}

void
TypedAST::print(const TypedNode* node, int indent)
{
  printIndent(indent);
  if (!node) {
    printf("(null)\n");
    return;
  }

  int nextIndent = indent + 2;
  auto idx = node->node.index();
  printf("%s\n", _typedNodesStr[node->node.index()]);
  std::visit(
    Overloaded{
      [&](const std::monostate&) { printf("nothing\n"); },
      [&](const NodeList& list) {
        for (auto& e : list.nodes)
          print(e, nextIndent);
      },
      [&](const FnDef& fnDef) {
        printf("(id: %u)\n", fnDef.fnID.val);
        print(fnDef.body, nextIndent);
      },
      [&](const CallExpr& callExpr) {
        printIndent(nextIndent);
        printf("callee:\n");
        print(callExpr.callee, nextIndent + 2);

        printIndent(nextIndent);
        printf("args:\n");
        for (auto& a : callExpr.args)
          print(a, nextIndent + 2);
      },
      [&](const ClassDef& classDef) {
        printf("(id: %u)\n", classDef.classID.val);

        printIndent(nextIndent);
        printf("methods:\n");
        for (auto& m : classDef.methods) {
          print(m, nextIndent + 2);
        }

        printIndent(nextIndent);
        printf("fields:\n");
        for (auto& f : classDef.fields) {
          printIndent(nextIndent + 2);
          printf("%s\n", f);
        }
      },
      [&](const EnumDef& enumDef) {
        for (auto& v : enumDef.vals) {
          printIndent(nextIndent);
          printf("%s\n", v.name);
        }
      },
      [&](const VarDecl& varDecl) {
        printIndent(nextIndent);
        printf("name:\n");
        printIndent(nextIndent + 2);
        printf("%s\n", varDecl.name);

        if (!std::holds_alternative<std::monostate>(varDecl.val->node)) {
          printIndent(nextIndent);
          printf("value:\n");
          print(varDecl.val, nextIndent + 2);

          printIndent(nextIndent);
          printf("type:\n");
          printIndent(nextIndent + 2);
          printf("%u\n", _analyzer->inferType(varDecl.val).val);
        }
      },
      [&](const VarAssign& varAssign) {
        printIndent(nextIndent);
        printf("op: %s\n", op2String(static_cast<OpType>(varAssign.op)));

        printIndent(nextIndent);
        printf("lhs:\n");
        print(varAssign.lhs, nextIndent + 2);

        printIndent(nextIndent);
        printf("type:\n");
        printIndent(nextIndent + 2);
        printf("%u\n", _analyzer->inferType(varAssign.lhs).val);

        printIndent(nextIndent);
        printf("rhs:\n");
        print(varAssign.rhs, nextIndent + 2);

        printIndent(nextIndent);
        printf("type:\n");
        printIndent(nextIndent + 2);
        printf("%u\n", _analyzer->inferType(varAssign.rhs).val);
      },
      [&](const LoopWhl& loopWhl) {
        printIndent(nextIndent);
        printf("cond:\n");
        print(loopWhl.cond, nextIndent + 2);

        printIndent(nextIndent);
        printf("body:\n");
        print(loopWhl.body, nextIndent + 2);
      },
      [&](const LoopFor& loopFor) {
        printIndent(nextIndent);
        printf("pre:\n");
        print(loopFor.assigns, nextIndent + 2);

        printIndent(nextIndent);
        printf("cond:\n");
        print(loopFor.cond, nextIndent + 2);

        printIndent(nextIndent);
        printf("post:\n");
        print(loopFor.postIter, nextIndent + 2);

        printIndent(nextIndent);
        printf("body:\n");
        print(loopFor.body, nextIndent + 2);
      },
      [&](const StmtIf& stmtIf) {
        printIndent(nextIndent);
        printf("cond:\n");
        print(stmtIf.cond, nextIndent + 2);

        printIndent(nextIndent);
        printf("body:\n");
        print(stmtIf.body, nextIndent + 2);

        if (!std::holds_alternative<std::monostate>(stmtIf.elseBody->node)) {
          printIndent(nextIndent);
          printf("else body:\n");
          print(stmtIf.elseBody, nextIndent + 2);
        }
      },
      [&](const StmtSwitch& stmtSwitch) {
        printIndent(nextIndent);
        printf("expr:\n");
        print(stmtSwitch.expr, nextIndent);

        printIndent(nextIndent);
        printf("cases:\n");

        for (auto& c : stmtSwitch.cases) {
          printIndent(nextIndent + 2);
          printf("case:\n");

          printIndent(nextIndent + 4);
          printf("expr:\n");
          print(c.expr, nextIndent + 6);

          printIndent(nextIndent + 4);
          printf("body:\n");
          print(c.body, nextIndent + 6);
        }
      },
      [&](const StmtRet& stmtRet) {
        if (!std::holds_alternative<std::monostate>(stmtRet.retValue->node)) {
          printIndent(nextIndent);
          printf("expr:\n");
          print(stmtRet.retValue, nextIndent + 2);
        }
      },
      [&](const StmtBrk& stmtBrk) {
        if (!std::holds_alternative<std::monostate>(stmtBrk.cond->node)) {
          printIndent(nextIndent);
          printf("cond:\n");
          print(stmtBrk.cond, nextIndent + 2);
        }
      },
      [&](const BinaryExpr& binaryExpr) {
        printIndent(nextIndent);
        printf("op: %s\n", op2String(static_cast<OpType>(binaryExpr.op)));

        printIndent(nextIndent);
        printf("lhs:\n");
        print(binaryExpr.lhs, nextIndent + 2);

        printIndent(nextIndent);
        printf("type:\n");
        printIndent(nextIndent + 2);
        printf("%u\n", _analyzer->inferType(binaryExpr.lhs).val);

        printIndent(nextIndent);
        printf("rhs:\n");
        print(binaryExpr.rhs, nextIndent + 2);

        printIndent(nextIndent);
        printf("type:\n");
        printIndent(nextIndent + 2);
        printf("%u\n", _analyzer->inferType(binaryExpr.rhs).val);
      },
      [&](const UnaryExpr& unaryExpr) {
        std::string precStr = "(";
        if (unaryExpr.prec == UnaryExprPrec::Prefix)
          precStr += "prefix)";
        else
          precStr += "postfix)";

        printf("%s\n", precStr.c_str());

        printIndent(nextIndent);
        printf("op: %s\n", op2String(static_cast<OpType>(unaryExpr.op)));

        printIndent(nextIndent);
        printf("expr:\n");
        print(unaryExpr.expr, nextIndent + 2);

        printIndent(nextIndent);
        printf("type:\n");
        printIndent(nextIndent + 2);
        printf("%u\n", _analyzer->inferType(unaryExpr.expr).val);
      },
      [&](const MemberAccess& membAccess) {
        printIndent(nextIndent);
        printf("base:\n");
        print(membAccess.base, nextIndent + 2);

        for (auto& m : membAccess.memb) {
          printIndent(nextIndent);
          printf("member:\n");
          print(m, nextIndent + 2);
        }
      },
      [&](const ArrayAccess& arrAccess) {
        printIndent(nextIndent);
        printf("base:\n");
        print(arrAccess.base, nextIndent + 2);

        printIndent(nextIndent);
        printf("index:\n");
        print(arrAccess.index, nextIndent + 2);
      },
      [&](const BoolVal& boolVal) {
        std::string valStr = boolVal.val ? "true" : "false";
        printIndent(nextIndent);
        printf("%s\n", valStr.c_str());
      },
      [&](const NumVal& numVal) {
        std::string valStr = std::to_string(numVal.val);
        printIndent(nextIndent);
        printf("%s\n", valStr.c_str());
      },
      [&](const StringVal& stringVal) {
        printIndent(nextIndent);
        printf("%s\n", stringVal.val);
      },
      [&](const ArrayVal& arrVal) {
        for (auto& e : arrVal.list)
          print(e, nextIndent);
      },
      [&](const NameExpr& nameExpr) {
        printIndent(nextIndent);
        printf("%s\n", nameExpr.name);
      },
    },
    node->node);
}