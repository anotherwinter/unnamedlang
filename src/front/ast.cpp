#include "ast.h"
#include "lexer.h"
#include "shared.h"
#include <array>
#include <stdlib.h>
#include <string.h>

// table for nodetypes names
static constexpr std::array<const char*,
                            static_cast<size_t>(ASTNodeType::_COUNT)>
  _nodeTypeStr = {
#define X(Name) #Name,
#include "ast_nodetypes.def"
#undef X
  };

// table for optypes names
static constexpr std::array<const char*, static_cast<size_t>(OpType::_COUNT)>
  _opTypeStr = {
#define X(Name, Op, ...) #Op,
#include "ast_optypes.def"
#undef X
  };

// TODO: remove this
void
yyerror(const char* str)
{
  fprintf(stderr, "Parse error: %s\n", str);
}

static void
printAST(ASTNode* node, int indent);

// convert plain token enum value to OpType
static OpType
tokType2OpType(TokenType tokType, bool unary)
{
  if (static_cast<uint16_t>(tokType) > static_cast<uint16_t>(OpType::_COUNT)) {
    yyerror("Wrong operator passed to tokType2OpType\n");
    return OpType::_COUNT;
  }

  switch (tokType) {
    case TokenType::MINUS:
      return unary ? OP_UNARY_ARITHM_NEG : OP_BINARY_SUB;
    case TokenType::DOT:
      return OP_BINARY_MEMB_ACCESS;
    default:
      return static_cast<OpType>(tokType);
  }
}

const char*
op2String(OpType opType)
{
  return _opTypeStr[to_underlying(opType)];
}

ASTNode*
allocNode(ASTNodeType type)
{
  ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
  if (!node) {
    yyerror("failed to malloc node\n");
    exit(EXIT_FAILURE);
  }

  node->type = type;
  memset(&node->data, 0, sizeof(node->data));
  return node;
}

static ASTNodeLL*
cloneASTNodeLL(ASTNodeLL* list)
{
  ASTNodeLL* head = NULL;
  ASTNodeLL** pnext = &head;

  while (list) {
    ASTNodeLL* newItem = (ASTNodeLL*)malloc(sizeof(ASTNodeLL));
    newItem->node = duplicateNode(list->node);
    newItem->next = NULL;

    *pnext = newItem;
    pnext = &newItem->next;
    list = list->next;
  }

  return head;
}

ASTNode*
duplicateNode(ASTNode* node)
{
  if (!node)
    return NULL;

  ASTNode* copy = allocNode(node->type);

  switch (node->type) {
    case NODE_LIST: {
      copy->data.nodeList.list = cloneASTNodeLL(node->data.nodeList.list);
      break;
    }
    case NODE_PARAM_INFO: {
      copy->data.paramInfo.name = strdup(node->data.paramInfo.name);
      copy->data.paramInfo.type = duplicateNode(node->data.paramInfo.type);
      break;
    }
    case NODE_FN_DEF: {
      copy->data.fnDef.name = strdup(node->data.fnDef.name);
      copy->data.fnDef.params = cloneASTNodeLL(node->data.fnDef.params);
      copy->data.fnDef.code = duplicateNode(node->data.fnDef.code);
      break;
    }
    case NODE_FN_CALL: {
      copy->data.fnCall.callee = duplicateNode(node->data.fnCall.callee);
      copy->data.fnCall.arguments = cloneASTNodeLL(node->data.fnCall.arguments);
      break;
    }
    case NODE_CLASS_DEF: {
      copy->data.classDef.name = strdup(node->data.classDef.name);
      copy->data.classDef.members = cloneASTNodeLL(node->data.classDef.members);
      break;
    }
    case NODE_CLASSMETHOD_DEF: {
      copy->data.classMethodDef.modifier = node->data.classMethodDef.modifier;
      copy->data.classMethodDef.function =
        duplicateNode(node->data.classMethodDef.function);
      break;
    }
    case NODE_CLASSFIELD_DEF: {
      copy->data.classFieldDef.name = strdup(node->data.classFieldDef.name);
      copy->data.classFieldDef.modifier = node->data.classFieldDef.modifier;
      copy->data.classFieldDef.type =
        duplicateNode(node->data.classFieldDef.type);
      break;
    }
    case NODE_ENUM_DEF: {
      copy->data.enumDef.name = strdup(node->data.enumDef.name);
      copy->data.enumDef.elements = cloneASTNodeLL(node->data.enumDef.elements);
      break;
    }
    case NODE_ENUM_ELEMENT: {
      copy->data.enumElement.name = strdup(node->data.enumElement.name);
      copy->data.enumElement.expr = duplicateNode(node->data.enumElement.expr);
      break;
    }

    case NODE_MODIFIER:
      copy->data.modifier = node->data.modifier;
      break;
    case NODE_VAR_DECL: {
      copy->data.varDecl.name = strdup(node->data.varDecl.name);
      copy->data.varDecl.modifier = node->data.varDecl.modifier;
      copy->data.varDecl.type = duplicateNode(node->data.varDecl.type);
      copy->data.varDecl.expr = duplicateNode(node->data.varDecl.expr);
      break;
    }
    case NODE_VAR_ASSIGN: {
      copy->data.varAssign.op = node->data.varAssign.op;
      copy->data.varAssign.lhs = duplicateNode(node->data.varAssign.lhs);
      copy->data.varAssign.rhs = duplicateNode(node->data.varAssign.rhs);
      break;
    }
    case NODE_DEL: {
      copy->data.del.expr = duplicateNode(node->data.del.expr);
      break;
    }
    case NODE_WHL: {
      copy->data.loopWhl.condition =
        duplicateNode(node->data.loopWhl.condition);
      copy->data.loopWhl.code = duplicateNode(node->data.loopWhl.code);
      break;
    }
    case NODE_FOR: {
      copy->data.loopFor.assigns = cloneASTNodeLL(node->data.loopFor.assigns);
      copy->data.loopFor.condition =
        cloneASTNodeLL(node->data.loopFor.condition);
      copy->data.loopFor.postIterationAssigns =
        cloneASTNodeLL(node->data.loopFor.postIterationAssigns);
      copy->data.loopFor.code = duplicateNode(node->data.loopFor.code);
      break;
    }
    case NODE_IF: {
      copy->data.ifBlock.condition =
        duplicateNode(node->data.ifBlock.condition);
      copy->data.ifBlock.code = duplicateNode(node->data.ifBlock.code);
      copy->data.ifBlock.elseBlock =
        duplicateNode(node->data.ifBlock.elseBlock);
      break;
    }
    case NODE_SWITCH: {
      copy->data.switchStmt.expr = duplicateNode(node->data.switchStmt.expr);
      copy->data.switchStmt.cases = cloneASTNodeLL(node->data.switchStmt.cases);
      break;
    }
    case NODE_SWITCH_CASE: {
      copy->data.switchCase.expr = duplicateNode(node->data.switchCase.expr);
      copy->data.switchCase.code = duplicateNode(node->data.switchCase.code);
      copy->data.switchCase.isDefault = node->data.switchCase.isDefault;
      break;
    }
    case NODE_RET: {
      copy->data.ret.retValue = duplicateNode(node->data.ret.retValue);
      break;
    }
    case NODE_BRK: {
      copy->data.brk.condition = duplicateNode(node->data.brk.condition);
      break;
    }
    case NODE_TOSS: {
      copy->data.toss.msg = duplicateNode(node->data.toss.msg);
      break;
    }
    case NODE_BINARYOP: {
      copy->data.binaryOp.op = node->data.binaryOp.op;
      copy->data.binaryOp.lhs = duplicateNode(node->data.binaryOp.lhs);
      copy->data.binaryOp.rhs = duplicateNode(node->data.binaryOp.rhs);
      break;
    }
    case NODE_UNARYPRE:
    case NODE_UNARYPOST: {
      copy->data.unaryOp.op = node->data.unaryOp.op;
      copy->data.unaryOp.expr = duplicateNode(node->data.unaryOp.expr);
      break;
    }
    case NODE_MEMBER_ACCESS: {
      copy->data.memberAccess.left =
        duplicateNode(node->data.memberAccess.left);
      copy->data.memberAccess.right =
        duplicateNode(node->data.memberAccess.right);
      break;
    }
    case NODE_ARR_ACCESS: {
      copy->data.arrayAccess.left = duplicateNode(node->data.arrayAccess.left);
      copy->data.arrayAccess.subscript =
        duplicateNode(node->data.arrayAccess.subscript);
      break;
    }
    case NODE_BOOL:
      copy->data.boolValue = node->data.boolValue;
      break;
    case NODE_NUMBER:
      copy->data.numberValue = node->data.numberValue;
      break;
    case NODE_STRING:
      copy->data.stringValue = strdup(node->data.stringValue);
      break;
    case NODE_ARRAY: {
      copy->data.array.elements = cloneASTNodeLL(node->data.array.elements);
      break;
    }
    case NODE_NAME:
      copy->data.stringValue = strdup(node->data.stringValue);
      break;
    case NODE_LAMBDA: {
      copy->data.lambda.captureList =
        cloneASTNodeLL(node->data.lambda.captureList);
      copy->data.lambda.params = cloneASTNodeLL(node->data.lambda.params);
      copy->data.lambda.code = duplicateNode(node->data.lambda.code);
      break;
    }
    default:
      break;
  }

  return copy;
}

/* <--create nodes LL with first element--> */
static ASTNodeLL*
astNodeLLCreate(ASTNode* node)
{
  ASTNodeLL* elem = (ASTNodeLL*)malloc(sizeof(ASTNodeLL));
  if (!elem) {
    yyerror("failed to malloc node\n");
    exit(EXIT_FAILURE);
  }

  elem->node = node;
  elem->next = NULL;
  return elem;
}

/* <--append to LL of nodes--> */
void
astNodeLLAppend(ASTNode* list, ASTNode* node)
{
  if (list == NULL)
    return;
  ASTNodeLL* it = list->data.nodeList.list;
  while (it->next != NULL) {
    it = it->next;
  }
  ASTNodeLL* newElem = astNodeLLCreate(node);
  it->next = newElem;
}

static void
freeASTNodeLL(ASTNodeLL* list)
{
  while (list) {
    ASTNodeLL* next = list->next;
    freeNode(list->node);
    free(list);
    list = next;
  }
}

void
freeNode(ASTNode* node)
{
  if (!node)
    return;

  switch (node->type) {
    case NODE_LIST: {
      freeASTNodeLL(node->data.nodeList.list);
      break;
    }
    case NODE_PARAM_INFO: {
      free((void*)node->data.paramInfo.name);
      freeNode(node->data.paramInfo.type);
      break;
    }
    case NODE_FN_DEF: {
      free((void*)node->data.fnDef.name);
      freeASTNodeLL(node->data.fnDef.params);
      freeNode(node->data.fnDef.code);
      break;
    }
    case NODE_FN_CALL: {
      freeNode(node->data.fnCall.callee);
      freeASTNodeLL(node->data.fnCall.arguments);
      break;
    }
    case NODE_CLASS_DEF: {
      free((void*)node->data.classDef.name);
      freeASTNodeLL(node->data.classDef.members);
      break;
    }
    case NODE_CLASSMETHOD_DEF: {
      freeNode(node->data.classMethodDef.function);
      break;
    }
    case NODE_CLASSFIELD_DEF: {
      free((void*)node->data.classFieldDef.name);
      freeNode(node->data.classFieldDef.type);
      break;
    }
    case NODE_ENUM_DEF: {
      free((void*)node->data.enumDef.name);
      freeASTNodeLL(node->data.enumDef.elements);
      break;
    }
    case NODE_ENUM_ELEMENT: {
      free((void*)node->data.enumElement.name);
      freeNode(node->data.enumElement.expr);
      break;
    }
    case NODE_MODIFIER:
      break;
    case NODE_VAR_DECL: {
      free((void*)node->data.varDecl.name);
      freeNode(node->data.varDecl.type);
      freeNode(node->data.varDecl.expr);
      break;
    }
    case NODE_VAR_ASSIGN: {

      freeNode(node->data.varAssign.lhs);
      freeNode(node->data.varAssign.rhs);
      break;
    }
    case NODE_DEL: {
      freeNode(node->data.del.expr);
      break;
    }
    case NODE_WHL: {
      freeNode(node->data.loopWhl.condition);
      freeNode(node->data.loopWhl.code);
      break;
    }
    case NODE_FOR: {
      freeASTNodeLL(node->data.loopFor.assigns);
      freeASTNodeLL(node->data.loopFor.condition);
      freeASTNodeLL(node->data.loopFor.postIterationAssigns);
      freeNode(node->data.loopFor.code);
      break;
    }
    case NODE_IF: {
      freeNode(node->data.ifBlock.condition);
      freeNode(node->data.ifBlock.code);
      freeNode(node->data.ifBlock.elseBlock);
      break;
    }
    case NODE_SWITCH: {
      freeNode(node->data.switchStmt.expr);
      freeASTNodeLL(node->data.switchStmt.cases);
      break;
    }
    case NODE_SWITCH_CASE: {
      freeNode(node->data.switchCase.expr);
      freeNode(node->data.switchCase.code);
      break;
    }
    case NODE_RET: {
      freeNode(node->data.ret.retValue);
      break;
    }
    case NODE_BRK: {
      freeNode(node->data.brk.condition);
      break;
    }
    case NODE_TOSS: {
      freeNode(node->data.toss.msg);
      break;
    }
    case NODE_BINARYOP: {
      freeNode(node->data.binaryOp.lhs);
      freeNode(node->data.binaryOp.rhs);
      break;
    }
    case NODE_UNARYPRE:
    case NODE_UNARYPOST: {
      freeNode(node->data.unaryOp.expr);
      break;
    }
    case NODE_MEMBER_ACCESS: {
      freeNode(node->data.memberAccess.left);
      freeNode(node->data.memberAccess.right);
      break;
    }
    case NODE_ARR_ACCESS: {
      freeNode(node->data.arrayAccess.left);
      freeNode(node->data.arrayAccess.subscript);
      break;
    }
    case NODE_BOOL:
    case NODE_NUMBER:
      break;
    case NODE_STRING: {
      free((void*)node->data.stringValue);
      break;
    }
    case NODE_ARRAY: {
      freeASTNodeLL(node->data.array.elements);
      break;
    }
    case NODE_NAME: {
      free((void*)node->data.stringValue);
      break;
    }
    case NODE_LAMBDA: {
      freeASTNodeLL(node->data.lambda.captureList);
      freeASTNodeLL(node->data.lambda.params);
      freeNode(node->data.lambda.code);
      break;
    }
    default:
      break;
  }

  free(node);
}

/* linked list of nodes */
ASTNode*
newNodeList(ASTNode* first)
{
  ASTNode* listNode = allocNode(NODE_LIST);
  listNode->data.nodeList.list = astNodeLLCreate(first);
  return listNode;
}

/* <--function/method parameter info */
ASTNode*
newParamInfo(const char* name, ASTNode* type)
{
  ASTNode* n = allocNode(NODE_PARAM_INFO);
  n->data.paramInfo.name = strdup(name);
  n->data.paramInfo.type = type;
  return n;
}

/* <--function definition--> */
ASTNode*
newFnDef(const char* name, ASTNode* params, ASTNode* code)
{
  ASTNode* n = allocNode(NODE_FN_DEF);
  n->data.fnDef.name = strdup(name);
  if (params)
    n->data.fnDef.params = params->data.nodeList.list;

  n->data.fnDef.code = code;

  return n;
}

/* <--function call--> */
ASTNode*
newFnCall(ASTNode* callee, ASTNode* arguments)
{
  ASTNode* n = allocNode(NODE_FN_CALL);
  if (arguments)
    n->data.fnCall.arguments = arguments->data.nodeList.list;

  n->data.fnCall.callee = callee;

  return n;
}

/* <--class definition--> */
ASTNode*
newClassDef(const char* name, ASTNode* members)
{
  ASTNode* n = allocNode(NODE_CLASS_DEF);
  if (members)
    n->data.classDef.members = members->data.nodeList.list;

  n->data.classDef.name = strdup(name);

  return n;
}

/* <--class method definition--> */
ASTNode*
newClassMethodDef(Modifier modifier, ASTNode* function)
{
  ASTNode* n = allocNode(NODE_CLASSMETHOD_DEF);
  n->data.classMethodDef.modifier = modifier;
  n->data.classMethodDef.function = function;

  return n;
}

/* <--class field definition--> */
ASTNode*
newClassFieldDef(const char* name, Modifier modifier, ASTNode* type)
{
  ASTNode* n = allocNode(NODE_CLASSFIELD_DEF);
  n->data.classFieldDef.name = strdup(name);
  n->data.classFieldDef.modifier = modifier;
  n->data.classFieldDef.type = type;

  return n;
}

/* <--enumeration definition--> */
ASTNode*
newEnumDef(const char* name, ASTNode* elements)
{
  ASTNode* n = allocNode(NODE_ENUM_DEF);
  n->data.enumDef.name = strdup(name);
  if (elements)
    n->data.enumDef.elements = elements->data.nodeList.list;

  return n;
}

/* <--enumeration element--> */
ASTNode*
newEnumElement(const char* name, ASTNode* expr)
{
  ASTNode* n = allocNode(NODE_ENUM_ELEMENT);
  n->data.enumElement.name = strdup(name);
  n->data.enumElement.expr = expr;

  return n;
}

/* <--wrapper for modifier-> */
ASTNode*
newModifier(Modifier modifier)
{
  ASTNode* n = allocNode(NODE_MODIFIER);
  n->data.modifier = modifier;

  return n;
}

/* <--batch declaration--> */
ASTNode*
newVarDeclBatch(ASTNode* decls)
{
  ASTNodeLL* list = decls->data.nodeList.list;
  if (decls) {
    ASTNodeLL* it = list;
    while (it) {
      ASTNode* n = it->node;
      if (n->type != NODE_VAR_DECL)
        yyerror("this wont happen\n");

      it = it->next;
    }
  }

  return decls;
}

/* <--var decl with optional assignment--> */
ASTNode*
newVarDecl(const char* name, Modifier modifier, ASTNode* expr)
{
  ASTNode* n = allocNode(NODE_VAR_DECL);
  n->data.varDecl.name = strdup(name);
  n->data.varDecl.modifier = modifier;
  n->data.varDecl.type = nullptr;
  n->data.varDecl.expr = expr;

  return n;
}

/* <--variable assign--> */
ASTNode*
newVarAssign(TokenType op, ASTNode* lhs, ASTNode* rhs)
{
  ASTNode* n = allocNode(NODE_VAR_ASSIGN);
  n->data.varAssign.op = tokType2OpType(op, false);
  n->data.varAssign.lhs = lhs;
  n->data.varAssign.rhs = rhs;

  return n;
}

/* <--del--> */
ASTNode*
newDelStmt(ASTNode* expr)
{
  ASTNode* n = allocNode(NODE_DEL);
  n->data.del.expr = expr;

  return n;
}

/* <--loop whl--> */
ASTNode*
newLoopWhl(ASTNode* condition, ASTNode* code)
{
  ASTNode* n = allocNode(NODE_WHL);
  n->data.loopWhl.condition = condition;
  n->data.loopWhl.code = code;

  return n;
}

/* <--loop for--> */
ASTNode*
newLoopFor(ASTNode* assigns,
           ASTNode* condition,
           ASTNode* postIterationAssigns,
           ASTNode* code)
{
  ASTNode* n = allocNode(NODE_FOR);
  if (assigns)
    n->data.loopFor.assigns = assigns->data.nodeList.list;

  if (postIterationAssigns)
    n->data.loopFor.postIterationAssigns =
      postIterationAssigns->data.nodeList.list;

  n->data.loopFor.condition = condition->data.nodeList.list;
  n->data.loopFor.code = code;

  return n;
}

/* <--if/else--> */
ASTNode*
newIfStmt(ASTNode* condition, ASTNode* code, ASTNode* elseBlock)
{
  ASTNode* n = allocNode(NODE_IF);
  n->data.ifBlock.condition = condition;
  n->data.ifBlock.code = code;
  n->data.ifBlock.elseBlock = elseBlock;

  return n;
}

/* <--switch--> */
ASTNode*
newSwitchStmt(ASTNode* expr, ASTNode* cases)
{
  ASTNode* n = allocNode(NODE_SWITCH);
  n->data.switchStmt.expr = expr;
  if (cases)
    n->data.switchStmt.cases = cases->data.nodeList.list;

  return n;
}

/* <--switch case--> */
ASTNode*
newSwitchCase(ASTNode* expr, ASTNode* code, bool isDefault)
{
  ASTNode* n = allocNode(NODE_SWITCH_CASE);
  n->data.switchCase.expr = expr;
  n->data.switchCase.code = code;
  n->data.switchCase.isDefault = isDefault;

  return n;
}

/* <--ret--> */
ASTNode*
newRetStmt(ASTNode* expr)
{
  ASTNode* n = allocNode(NODE_RET);
  n->data.ret.retValue = expr;

  return n;
}

/* <--brk--> */
ASTNode*
newBrkStmt(ASTNode* condition)
{
  ASTNode* n = allocNode(NODE_BRK);
  n->data.brk.condition = condition;

  return n;
}

/* <--toss (throw)--> */
ASTNode*
newTossStmt(ASTNode* msg)
{
  ASTNode* n = allocNode(NODE_TOSS);
  n->data.toss.msg = msg;

  return n;
}

/* <--binary op--> */
ASTNode*
newBinaryOp(TokenType op, ASTNode* lhs, ASTNode* rhs)
{
  ASTNode* n = allocNode(NODE_BINARYOP);
  n->data.binaryOp.op = tokType2OpType(op, false);
  n->data.binaryOp.lhs = lhs;
  n->data.binaryOp.rhs = rhs;

  return n;
}

/* <--unary prefix--> */
ASTNode*
newUnaryPre(TokenType op, ASTNode* expr)
{
  ASTNode* n = allocNode(NODE_UNARYPRE);
  n->data.unaryOp.op = tokType2OpType(op, true);
  n->data.unaryOp.expr = expr;

  return n;
}

/* <--unary postfix--> */
ASTNode*
newUnaryPost(TokenType op, ASTNode* expr)
{
  ASTNode* n = allocNode(NODE_UNARYPOST);
  n->data.unaryOp.op = tokType2OpType(op, true);
  n->data.unaryOp.expr = expr;

  return n;
}

/* <--member access--> */
ASTNode*
newMemberAccess(ASTNode* left, ASTNode* right)
{
  ASTNode* n = allocNode(NODE_MEMBER_ACCESS);
  n->data.memberAccess.left = left;
  n->data.memberAccess.right = right;

  return n;
}

/* <--array access--> */
ASTNode*
newArrayAccess(ASTNode* left, ASTNode* subscript)
{
  ASTNode* n = allocNode(NODE_ARR_ACCESS);
  n->data.arrayAccess.left = left;
  n->data.arrayAccess.subscript = subscript;

  return n;
}

/* <--node wrapper for bool literal--> */
ASTNode*
newBoolValue(bool value)
{
  ASTNode* n = allocNode(NODE_BOOL);
  n->data.boolValue = value;

  return n;
}

/* <--node wrapper for number literal--> */
ASTNode*
newNumberValue(double value)
{
  ASTNode* n = allocNode(NODE_NUMBER);
  n->data.numberValue = value;

  return n;
}

/* <--node wrapper for string literal--> */
ASTNode*
newStringValue(const char* value)
{
  ASTNode* n = allocNode(NODE_STRING);
  n->data.stringValue = strdup(value);

  return n;
}

/* <--array--> */
ASTNode*
newArray(ASTNode* elements)
{
  ASTNode* n = allocNode(NODE_ARRAY);
  if (elements)
    n->data.array.elements = elements->data.nodeList.list;

  return n;
}

/* <--name--> */
ASTNode*
newName(const char* name)
{
  ASTNode* n = allocNode(NODE_NAME);
  n->data.stringValue = strdup(name);

  return n;
}

/* <--lambda--> */
ASTNode*
newLambda(ASTNode* captureList, ASTNode* params, ASTNode* code)
{
  ASTNode* n = allocNode(NODE_LAMBDA);
  n->data.lambda.captureList = captureList->data.nodeList.list;
  if (params)
    n->data.fnDef.params = params->data.nodeList.list;

  n->data.lambda.code = code;

  return n;
}

ASTNode*
debugPoint(ASTNode* node)
{
  return node;
}

static void
printIndent(int indent)
{
  for (int i = 0; i < indent; ++i) {
    putchar(' ');
  }
}

static void
printNodeList(ASTNodeLL* list, int indent)
{
  for (ASTNodeLL* cur = list; cur; cur = cur->next) {
    printAST(cur->node, indent);
  }
}

void
printAST(ASTNode* node, int indent)
{
  printIndent(indent);
  if (!node) {
    printf("(null)\n");
    return;
  }

  printf("%s", _nodeTypeStr[static_cast<size_t>(node->type)]);

  switch (node->type) {
    case NODE_LIST:
      printf(":\n");
      printNodeList(node->data.nodeList.list, indent + 2);
      break;

    case NODE_PARAM_INFO:
      printf(" name='%s'\n", node->data.paramInfo.name);
      printAST(node->data.paramInfo.type, indent + 2);
      break;

    case NODE_FN_DEF:
      printf(" name='%s'\n", node->data.fnDef.name);
      printNodeList(node->data.fnDef.params, indent + 2);
      printAST(node->data.fnDef.code, indent + 2);
      break;

    case NODE_FN_CALL:
      printf("\n");
      printAST(node->data.fnCall.callee, indent + 2);
      printNodeList(node->data.fnCall.arguments, indent + 2);
      break;

    case NODE_CLASS_DEF:
      printf(" name='%s'\n", node->data.classDef.name);
      printNodeList(node->data.classDef.members, indent + 2);
      break;

    case NODE_CLASSMETHOD_DEF:
      printf(" modifier=0x%x\n", node->data.classMethodDef.modifier);
      printAST(node->data.classMethodDef.function, indent + 2);
      break;

    case NODE_CLASSFIELD_DEF:
      printf(" name='%s' modifier=0x%x\n",
             node->data.classFieldDef.name,
             node->data.classFieldDef.modifier);
      printAST(node->data.classFieldDef.type, indent + 2);
      break;

    case NODE_ENUM_DEF:
      printf(" name='%s'\n", node->data.enumDef.name);
      printNodeList(node->data.enumDef.elements, indent + 2);
      break;

    case NODE_ENUM_ELEMENT:
      printf(" name='%s'\n", node->data.enumElement.name);
      printAST(node->data.enumElement.expr, indent + 2);
      break;

    case NODE_MODIFIER:
      printf(" modifier=0x%x\n", node->data.modifier);
      break;

    case NODE_VAR_DECL:
      printf(" name='%s' modifier=0x%x\n",
             node->data.varDecl.name,
             node->data.varDecl.modifier);
      // TODO: node of variable type, currently unused
      // printAST(node->data.varDecl.type, indent + 2);
      printAST(node->data.varDecl.expr, indent + 2);
      break;

    case NODE_VAR_ASSIGN:
      printf(" op='%s'\n", op2String(node->data.varAssign.op));
      printAST(node->data.varAssign.lhs, indent + 2);
      printAST(node->data.varAssign.rhs, indent + 2);
      break;

    case NODE_DEL:
      printf("\n");
      printAST(node->data.del.expr, indent + 2);
      break;

    case NODE_WHL:
      printf("\n");
      printAST(node->data.loopWhl.condition, indent + 2);
      printAST(node->data.loopWhl.code, indent + 2);
      break;

    case NODE_FOR:
      printf("\n");
      printNodeList(node->data.loopFor.assigns, indent + 2);
      printNodeList(node->data.loopFor.condition, indent + 2);
      printNodeList(node->data.loopFor.postIterationAssigns, indent + 2);
      printAST(node->data.loopFor.code, indent + 2);
      break;

    case NODE_IF:
      printf("\n");
      printAST(node->data.ifBlock.condition, indent + 2);
      printAST(node->data.ifBlock.code, indent + 2);
      if (node->data.ifBlock.elseBlock)
        printAST(node->data.ifBlock.elseBlock, indent + 2);
      break;

    case NODE_SWITCH:
      printf("\n");
      printAST(node->data.switchStmt.expr, indent + 2);
      printNodeList(node->data.switchStmt.cases, indent + 2);
      break;

    case NODE_SWITCH_CASE:
      printf(" default=%s\n", node->data.switchCase.isDefault ? "yes" : "no");
      if (node->data.switchCase.expr)
        printAST(node->data.switchCase.expr, indent + 2);
      printAST(node->data.switchCase.code, indent + 2);
      break;

    case NODE_RET:
      printf("\n");
      printAST(node->data.ret.retValue, indent + 2);
      break;

    case NODE_BRK:
      printf("\n");
      if (node->data.brk.condition)
        printAST(node->data.brk.condition, indent + 2);
      break;

    case NODE_TOSS:
      printf("\n");
      printAST(node->data.toss.msg, indent + 2);
      break;

    case NODE_BINARYOP:
      printf(" op='%s'\n", op2String(node->data.binaryOp.op));
      printAST(node->data.binaryOp.lhs, indent + 2);
      printAST(node->data.binaryOp.rhs, indent + 2);
      break;

    case NODE_UNARYPRE:
      printf(" prefix op='%s'\n", op2String(node->data.unaryOp.op));
      printAST(node->data.unaryOp.expr, indent + 2);
      break;

    case NODE_UNARYPOST:
      printf(" postfix op='%s'\n", op2String(node->data.unaryOp.op));
      printAST(node->data.unaryOp.expr, indent + 2);
      break;

    case NODE_MEMBER_ACCESS:
      printf("\n");
      printAST(node->data.memberAccess.left, indent + 2);
      printAST(node->data.memberAccess.right, indent + 2);
      break;

    case NODE_ARR_ACCESS:
      printf("\n");
      printAST(node->data.arrayAccess.left, indent + 2);
      printAST(node->data.arrayAccess.subscript, indent + 2);
      break;

    case NODE_BOOL:
      printf(" value=%s\n", node->data.boolValue ? "true" : "false");
      break;

    case NODE_NUMBER:
      printf(" value=%g\n", node->data.numberValue);
      break;

    case NODE_STRING:
    case NODE_NAME:
      printf(" '%s'\n", node->data.stringValue);
      break;

    case NODE_ARRAY:
      printf("\n");
      printNodeList(node->data.array.elements, indent + 2);
      break;

    case NODE_LAMBDA:
      printf("\n");
      printNodeList(node->data.lambda.captureList, indent + 2);
      printNodeList(node->data.lambda.params, indent + 2);
      printAST(node->data.lambda.code, indent + 2);
      break;

    default:
      printf(" (unknown node type)\n");
      break;
  }
}

void
printASTRoot(ASTNode* root)
{
  printAST(root, 0);
}

ASTNode*
newClassOpDef(const char* name, OpType op, ASTNode* params, ASTNode* code)
{
  ASTNode* n = newFnDef(op2String(op), params, code);
  return n;
}