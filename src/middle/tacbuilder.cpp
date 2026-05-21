// #include "middle/tacbuilder.h"
// #include "diagnostics.h"
// #include "front/symbolregistry.h"
// #include "front/typedast.h"
// #include "middle/ssastate.h"

// using namespace MIR;
// using namespace HIR;

// TACBuilder::TACBuilder(Diagnostics& diag, LayoutRegistry& layReg)
//   : _diag(diag)
//   , _arena()

// {
//   _last = allocBlock();
// }

// Block*
// TACBuilder::allocBlock()
// {
//   void* raw = _arena.alloc(sizeof(Block), alignof(Block));
//   if (!raw) {
//     _diag.putMsg(STUB_ERR, 0, 0);
//     return nullptr;
//   }

//   return new (raw) Block();
// }

// ValueID
// TACBuilder::buildFromAST(const TypedNode* node)
// {
//   if (!node)
//     return {};

//   // std::visit(Overloaded{
//   //              [&](const std::monostate&) {},
//   //              [&](const NodeList& list) {},
//   //              [&](const CallExpr& callExpr) {},
//   //              [&](const VarDecl& varDecl) {},
//   //              [&](const VarAssign& varAssign) {},
//   //              [&](const LoopWhl& loopWhl) {},
//   //              [&](const LoopFor& loopFor) {},
//   //              [&](const StmtIf& stmtIf) {},
//   //              [&](const StmtSwitch& stmtSwitch) {},
//   //              [&](const StmtRet& stmtRet) {},
//   //              [&](const StmtBrk& stmtBrk) {},
//   //              [&](const BinaryExpr& binaryExpr) {},
//   //              [&](const UnaryExpr& unaryExpr) {},
//   //              [&](const MemberAccess& membAccess) {},
//   //              [&](const ArrayAccess& arrAccess) {},
//   //              [&](const BoolVal& boolVal) {},
//   //              [&](const NumVal& numVal) {},
//   //              [&](const StringVal& stringVal) {},
//   //              [&](const ArrayVal& arrVal) {},
//   //              [&](const NameExpr& nameExpr) {},
//   //            },
//   //            node->node);
// }

// ValueID
// TACBuilder::buildNodeList(const NodeList& list)
// {
//   for (auto& n : list.nodes)
//     buildFromAST(n);

//   return {};
// }

// ValueID
// TACBuilder::buildCallExpr(const CallExpr& callExpr)
// {
//   buildFromAST(callExpr.callee);

//   for (auto& a : callExpr.args)
//     buildCallArgument(a);

//   // if expression was resolved statically
//   if (callExpr.resType == CallExpr::ResolutionType::Static) {
//     ValueID val = _ssa->makeValueID();

//     _last->instructions.push_back(
//       { OpCode::Call, { TACValue{ callExpr.fnRes } }, val });
//   }
//   // if expression to be resolved dynamically
//   else if (callExpr.resType == CallExpr::ResolutionType::Dynamic) {
//   }
// }

// ValueID
// TACBuilder::buildVarDecl(const VarDecl& varDecl)
// {
//   _ssa->declare(varDecl.id, { varDecl.name, {} });

//   // if declaration has value, make instruction
//   if (varDecl.val) {
//     ValueID val = buildFromAST(varDecl.val);
//     _ssa->assign(varDecl.id, val);
//     _last->instructions.push_back(
//       { OpCode::Load, { TACValue{ varDecl.id }, TACValue{ val } } });

//     return val;
//   }

//   return {};
// }

// ValueID
// TACBuilder::buildVarAssign(const VarAssign& varAssign)
// {
//   ValueID lhs = buildFromAST(varAssign.lhs);
//   ValueID rhs = buildFromAST(varAssign.rhs);

//   if (varAssign.op > ExprOp::Assign) {
//     OpCode op;
//     switch (varAssign.op) {
//       case ExprOp::AssignAdd:
//         op = OpCode::Add;
//         break;
//       case ExprOp::AssignSub:
//         op = OpCode::Sub;
//         break;
//       case ExprOp::AssignMul:
//         op = OpCode::Mul;
//         break;
//       case ExprOp::AssignDiv:
//         op = OpCode::Div;
//         break;
//       case ExprOp::AssignMod:
//         op = OpCode::Mod;
//         break;
//       case ExprOp::AssignShiftl:
//         op = OpCode::Shiftl;
//         break;
//       case ExprOp::AssignShiftr:
//         op = OpCode::Shiftr;
//         break;
//       case ExprOp::AssignAnd:
//         op = OpCode::And;
//         break;
//       case ExprOp::AssignOr:
//         op = OpCode::Or;
//         break;
//       case ExprOp::AssignXor:
//         op = OpCode::Xor;
//         break;
//       default:
//         break;
//     }

//     _last->instructions.push_back({ op, { TACValue{ lhs }, TACValue{ rhs } } });
//   }

//   _ssa->assign(va, ValueID val)
// }

// ValueID
// TACBuilder::buildLoopWhl(const LoopWhl& loopWhl)
// {
// }

// ValueID
// TACBuilder::buildLoopFor(const LoopFor& loopFor)
// {
// }

// ValueID
// TACBuilder::buildIf(const StmtIf& stmtIf)
// {
// }

// ValueID
// TACBuilder::buildSwitch(const StmtSwitch& stmtSwitch)
// {
// }

// ValueID
// TACBuilder::buildRet(const StmtRet& stmtRet)
// {
// }

// ValueID
// TACBuilder::buildBrk(const StmtBrk& stmtBrk)
// {
// }

// ValueID
// TACBuilder::buildBinary(const BinaryExpr& binary)
// {
// }

// ValueID
// TACBuilder::buildUnary(const UnaryExpr& unary)
// {
// }

// ValueID
// TACBuilder::buildMemberAccess(const MemberAccess& membAccess)
// {
// }

// ValueID
// TACBuilder::buildArrayAccess(const ArrayAccess& arrAccess)
// {
// }

// ValueID
// TACBuilder::buildBool(const BoolVal& val)
// {
// }

// ValueID
// TACBuilder::buildNumber(const NumVal& val)
// {
// }

// ValueID
// TACBuilder::buildString(const StringVal& val)
// {
// }

// ValueID
// TACBuilder::buildArray(const ArrayVal& val)
// {
// }

// ValueID
// TACBuilder::buildName(const NameExpr& name)
// {
// }

// ValueID
// TACBuilder::buildExpr(const HIR::TypedNode* expr, bool ref)
// {
//   if (!expr)
//     return {};

//   auto res = std::visit(
//     Overloaded{
//       [&](const MemberAccess& membAccess) {
//         return buildMemberAccess(membAccess, ref);
//       },
//       [&](const ArrayAccess& arrAccess) {
//         return buildArrayAccess(arrAccess, ref);
//       },
//       [&](const NameExpr& nameExpr) { return buildName(nameExpr, ref); },
//       [&](const auto& val) { return ValueID{}; },
//     },
//     expr->node);
// }

// ValueID
// TACBuilder::buildMemberAccess(const HIR::MemberAccess& membAccess, bool ref)
// {
//   ValueID base = {};
//   OpCode loadOp = ref ? OpCode::LoadRef : OpCode::Load;
//   base = buildExpr(membAccess.base, ref);

//   for (size_t i = 0; i < membAccess.memb.size() - 1; ++i) {
//     auto memb = membAccess.memb[i];
//     if (auto )

//     Instruction getRef = {OpCode::GetRef, {TACValue{base}, }}  
//     base = 
//   }
// }

// ValueID
// TACBuilder::buildArrayAccess(const HIR::ArrayAccess& arrAccess, bool ref)
// {
// }

// ValueID
// TACBuilder::buildName(const HIR::NameExpr& name, bool ref)
// {
//   if
// }