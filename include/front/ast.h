#pragma once
#include "front/lexer.h"
#include <cstdint>
#include <stdbool.h>

struct ASTNode;

enum class OpType : uint16_t
{
#define X3(Name, Op, Val) Name = Val,
#define X2(Name, Op) Name,
#define GET_X(_1, _2, _3, NAME, ...) NAME

#define X(...) GET_X(__VA_ARGS__, X3, X2)(__VA_ARGS__)
#include "front/ast_optypes.def"
#undef X
#undef GET_X
#undef X2
#undef X3
  _COUNT,
};

enum class ASTNodeType : uint16_t
{
#define X(Name) Name,

#include "front/ast_nodetypes.def"

#undef X
  _COUNT,
};

// shortcuts for nodetype values
#define X(Name) inline constexpr ASTNodeType Name = ASTNodeType::Name;
#include "front/ast_nodetypes.def"
#undef X

// shortcuts for optype values
#define X(Name, Op, ...) inline constexpr OpType Name = OpType::Name;
#include "front/ast_optypes.def"
#undef X

enum class Modifier : uint16_t;

typedef struct ASTNodeLL
{
  // pointer to current node
  ASTNode* node;
  // pointer to next statement
  struct ASTNodeLL* next;
} ASTNodeLL;

typedef struct ASTNode
{
  ASTNodeType type;
  union
  {
    // <---NODE_LIST-->
    struct
    {
      // linked list of all related statements
      ASTNodeLL* list;
    } nodeList;

    // <--NODE_PARAM_INFO-->
    struct
    {
      // name of object
      const char* name;
      // name of object type
      ASTNode* type;
    } paramInfo;

    // <---NODE_FN_DEF-->
    struct
    {
      // function name
      const char* name;
      ASTNode* params;
      // pointer to function code block
      ASTNode* code;
    } fnDef;

    // <---NODE_FN_CALL-->
    struct
    {
      // name of function which is being called
      ASTNode* callee;
      ASTNode* arguments;
    } fnCall;

    // <---NODE_CLASS_DEF-->
    struct
    {
      // name of class
      const char* name;
      ASTNode* members;
    } classDef;

    // <---NODE_CLASSMETHOD_DEF-->
    struct
    {
      // accessibility modifiers
      Modifier modifier;
      // pointer to function definition cause its same as method but without
      // modifiers
      ASTNode* function;
    } classMethodDef;

    // <---NODE_CLASSFIELD_DEF-->
    struct
    {
      // field name
      const char* name;
      // accessibility modifiers
      Modifier modifier;
      // node of variable type
      ASTNode* type;
    } classFieldDef;

    // <---NODE_ENUM_DEF-->
    struct
    {
      // enumeration name
      const char* name;
      ASTNode* elements;
    } enumDef;

    // <--NODE_ENUM_ELEMENT-->
    struct
    {
      // element name
      const char* name;
      // expression to be assigned to enum element (must evaluate to int)
      ASTNode* expr;
    } enumElement;

    // <--NODE_MODIFIER-->
    Modifier modifier;

    // <---NODE_VAR_DECL-->
    struct
    {
      // variable name
      const char* name;
      // accessibility modifiers
      Modifier modifier;
      // TODO: node of variable type, currently unused
      ASTNode* type;
      // assignment expression
      ASTNode* expr;
    } varDecl;

    // <---NODE_VAR_ASSIGN-->
    struct
    {
      // assignment operator like = or +=
      OpType op;
      // variable name
      ASTNode* lhs;
      // expression that is being assigned to variable
      ASTNode* rhs;
    } varAssign;

    // <---NODE_DEL-->
    struct
    {
      // expression to delete
      ASTNode* expr;
    } del;

    // <---NODE_WHL-->
    struct
    {
      // condition which is being evaluated at start of each iteration
      ASTNode* condition;
      // pointer to code to execute
      ASTNode* code;
    } loopWhl;

    // <---NODE_FOR-->
    struct
    {
      // list of assignments to do before loop starts
      ASTNode* assigns;
      // condition under which to execute iterations
      ASTNode* condition;
      // list of assignments to do after each iteration
      ASTNode* postIterationAssigns;
      // code to execute
      ASTNode* code;
    } loopFor;

    // <---NODE_IF-->
    struct
    {
      // condition under which if-block executes
      ASTNode* condition;
      // code to run if condition evaluates to TRUE
      ASTNode* code;
      // code to run if condition evaluates to FALSE
      ASTNode* elseBlock;
    } ifBlock;

    // <---NODE_SWITCH-->
    struct
    {
      // expression to evaluate and compare against cases
      ASTNode* expr;
      // cases to go into if expression evaluates to them
      ASTNode* cases;
    } switchStmt;

    // <---NODE_SWITCH_CASE-->
    struct
    {
      // condition of switch case
      ASTNode* expr;
      // code to execute if switch expression evaluates equal to switch case
      // expression
      ASTNode* code;
      // if current case is default case
      bool isDefault;
    } switchCase;

    // <---NODE_RET-->
    struct
    {
      ASTNode* retValue;
    } ret;

    // <---NODE_BRK-->
    struct
    {
      // condition to break on, can be null if no condition needed
      ASTNode* condition;
    } brk;

    // <---NODE_TOSS-->
    struct
    {
      // call for exception ctor to toss
      ASTNode* msg;
    } toss;

    // <---NODE_BINARYOP-->
    struct
    {
      OpType op;
      ASTNode* lhs;
      ASTNode* rhs;
    } binaryOp;

    // <---NODE_UNARYOP-->
    struct
    {
      OpType op;
      ASTNode* expr;
    } unaryOp;

    // <---NODE_MEMBER_ACCESS-->
    struct
    {
      ASTNode* left;
      ASTNode* right;
    } memberAccess;

    // <---NODE_ARR_ACCESS-->
    struct
    {
      // identificator/class field/another array access
      ASTNode* left;
      ASTNode* subscript;
    } arrayAccess;

    // <---NODE_BOOL-->
    bool boolValue;

    // <---NODE_NUMBER-->
    double numberValue;

    // <---NODE_STRING, NODE_NAME-->
    const char* stringValue;

    // <---NODE_ARRAY-->
    struct
    {
      ASTNode* elements;
    } array;

    // <---NODE_LAMBDA-->
    struct
    {
      ASTNode* captureList;
      ASTNode* params;
      ASTNode* code;
    } lambda;
  } data;

  size_t line;
  size_t col;
} ASTNode;

inline ASTNodeLL*
nodeList(const ASTNode* node)
{
  if (node == nullptr)
    return nullptr;

  return node->data.nodeList.list;
}

// TODO: remove this
void
setLexer(Lexer* lex);

const char*
op2String(OpType opType);

ASTNode*
allocNode(ASTNodeType type);

ASTNode*
duplicateNode(ASTNode* node);

void
freeNode(ASTNode* node);

void
astNodeLLConcat(ASTNode* left, ASTNode* right);

ASTNode*
astNodeLLPrepend(ASTNode* list, ASTNode* node);

void
astNodeLLAppend(ASTNode* list, ASTNode* node);

ASTNode*
newNodeList(ASTNode* first);

ASTNode*
newParamInfo(const char* name, ASTNode* type);

ASTNode*
newFnDef(const char* name, ASTNode* params, ASTNode* code);

ASTNode*
newFnCall(ASTNode* callee, ASTNode* arguments);

ASTNode*
newClassDef(const char* name, ASTNode* members);

ASTNode*
newClassMethodDef(Modifier modifier, ASTNode* function);

ASTNode*
newClassFieldDef(const char* name, Modifier modifier, ASTNode* type);

ASTNode*
newEnumDef(const char* name, ASTNode* elements);

ASTNode*
newEnumElement(const char* name, ASTNode* expr);

ASTNode*
newModifier(Modifier modifier);

ASTNode*
newVarDeclBatch(ASTNode* declarations);

ASTNode*
newVarDecl(const char* name, Modifier modifier, ASTNode* expr);

ASTNode*
newVarAssign(TokenType op, ASTNode* lhs, ASTNode* rhs);

ASTNode*
newDelStmt(ASTNode* expr);

ASTNode*
newTypeInfo(const char* name, ASTNode* innerTypes);

ASTNode*
newLoopWhl(ASTNode* condition, ASTNode* code);

ASTNode*
newLoopFor(ASTNode* assigns,
           ASTNode* condition,
           ASTNode* postIterationAssigns,
           ASTNode* code);

ASTNode*
newIfStmt(ASTNode* condition, ASTNode* code, ASTNode* elseBlock);

ASTNode*
newSwitchStmt(ASTNode* expr, ASTNode* cases);

ASTNode*
newSwitchCase(ASTNode* expr, ASTNode* code, bool isDefault);

ASTNode*
newRetStmt(ASTNode* expr);

ASTNode*
newBrkStmt(ASTNode* condition);

ASTNode*
newTossStmt(ASTNode* exceptionCtorCall);

ASTNode*
newBinaryOp(TokenType op, ASTNode* lhs, ASTNode* rhs);

ASTNode*
newUnaryPre(TokenType op, ASTNode* expr);

ASTNode*
newUnaryPost(TokenType op, ASTNode* expr);

ASTNode*
newMemberAccess(ASTNode* left, ASTNode* right);

ASTNode*
newArrayAccess(ASTNode* left, ASTNode* subscripts);

ASTNode*
newBoolValue(bool value);

ASTNode*
newNumberValue(double value);

ASTNode*
newStringValue(const char* value);

ASTNode*
newArray(ASTNode* elements);

ASTNode*
newName(const char* name);

ASTNode*
newLambda(ASTNode* captureList, ASTNode* params, ASTNode* code);

ASTNode*
debugPoint(ASTNode* node);

void
printASTRoot(ASTNode* root);

ASTNode*
newClassOpDef(const char* name, OpType op, ASTNode* params, ASTNode* code);