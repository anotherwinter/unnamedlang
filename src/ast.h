#pragma once
#include <cstdio>
#include <stdbool.h>

typedef struct ASTNode ASTNode;

enum class OperatorType
{
  BINARY_MEMB_ACCESS,
  UNARY_INC,
  UNARY_DEC,
  UNARY_NEG,
  UNARY_ARITHM_NEG,
  BINARY_MUL,
  BINARY_DIV,
  BINARY_MOD,
  BINARY_ADD,
  BINARY_SUB,
  BINARY_SHIFTL,
  BINARY_SHIFTR,
  BINARY_AND,
  BINARY_OR,
  BINARY_XOR,
  BINARY_LT,
  BINARY_GT,
  BINARY_LEQ,
  BINARY_GEQ,
  BINARY_EQ,
  BINARY_NEQ,
  BINARY_LOGIC_AND,
  BINARY_LOGIC_OR,
  EQUALS,
  ASSIGN_ADD,
  ASSIGN_SUB,
  ASSIGN_DIV,
  ASSIGN_MUL,
  ASSIGN_MOD,
  ASSIGN_XOR,
  ASSIGN_SHIFTL,
  ASSIGN_SHIFTR,
  ASSIGN_AND,
  ASSIGN_OR
};

typedef enum
{
  NODE_LIST,
  NODE_PARAM_INFO,
  NODE_FN_DEF,
  NODE_FN_CALL,
  NODE_CLASS_DEF,
  NODE_CLASSMETHOD_DEF,
  NODE_CLASSFIELD_DEF,
  NODE_ENUM_DEF,
  NODE_ENUM_ELEMENT,
  NODE_MODIFIER,
  NODE_VAR_DECL,
  NODE_VAR_ASSIGN,
  NODE_DEL,
  NODE_WHL,
  NODE_FOR,
  NODE_IF,
  NODE_SWITCH,
  NODE_SWITCH_CASE,
  NODE_RET,
  NODE_BRK,
  NODE_TOSS,
  NODE_BINARYOP,
  NODE_UNARYOP,
  NODE_MEMBER_ACCESS,
  NODE_ARR_ACCESS,
  NODE_BOOL,
  NODE_NUMBER,
  NODE_STRING,
  NODE_ARRAY,
  NODE_NAME,
  NODE_LAMBDA
} ASTNodeType;

typedef enum
{
  MODIFIER_NONE = 0,
  MOD_PUBLIC = 1 << 0,
  MOD_HIDDEN = 1 << 1,
  MOD_STATIC = 1 << 2,
} Modifier;

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
      ASTNodeLL* params;
      // pointer to function code block
      ASTNode* code;
    } fnDef;

    // <---NODE_FN_CALL-->
    struct
    {
      // name of function which is being called
      ASTNode* callee;
      ASTNodeLL* arguments;
    } fnCall;

    // <---NODE_CLASS_DEF-->
    struct
    {
      // name of class
      const char* name;
      ASTNodeLL* members;
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
      ASTNodeLL* elements;
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
      // node of variable type
      ASTNode* type;
      // assignment expression
      ASTNode* expr;
    } varDecl;

    // <---NODE_VAR_ASSIGN-->
    struct
    {
      // assignment operator like = or +=
      OperatorType op;
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
      ASTNodeLL* assigns;
      // condition under which to execute iterations
      ASTNodeLL* condition;
      // list of assignments to do after each iteration
      ASTNodeLL* postIterationAssigns;
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
      ASTNodeLL* cases;
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
      OperatorType op;
      ASTNode* lhs;
      ASTNode* rhs;
    } binaryOp;

    // <---NODE_UNARYOP-->
    struct
    {
      OperatorType op;
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
      ASTNodeLL* elements;
    } array;

    // <---NODE_LAMBDA-->
    struct
    {
      ASTNodeLL* captureList;
      ASTNodeLL* params;
      ASTNode* code;
    } lambda;
  } data;
} ASTNode;

const char*
op2String(OperatorType opType);

ASTNode*
allocNode(ASTNodeType type);

ASTNode*
duplicateNode(ASTNode* node);

void
freeNode(ASTNode* node);

void
astNodeLLAppend(ASTNodeLL* list, ASTNode* node);

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
newVarAssign(int op, ASTNode* lhs, ASTNode* rhs);

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
newBinaryOp(int op, ASTNode* lhs, ASTNode* rhs);

ASTNode*
newUnaryOp(int op, ASTNode* expr);

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

void printASTRoot(ASTNode* root);