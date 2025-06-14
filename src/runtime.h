#pragma once
#include "ast.h"
#include "types.h"
#include <csetjmp>
#include <string>
#include <unordered_map>

typedef enum
{
  NONE = 0,
  // if in global context
  IN_GLOBAL = 1 << 0,
  // if inside of a function
  IN_FUNCTION = 1 << 1,
  // if inside of a method (method also counts as a function)
  IN_METHOD = 1 << 2,
  // if inside of a breakable construct (switch, whl)
  IN_BREAKABLE = 1 << 3,
  // if broken inside breakable
  BROKEN = 1 << 4,
  // if has unhandled exception
  UNHANDLED_EX = 1 << 5,
} ContextFlags;

void
runtimeInit();

void*
allocOnArena(size_t n);

// add temporary object to list for freeing
void
addTemporaryObject(Object* ptr);

void
cleanTemporaryObjects();

// ordinary duplication without interning, set duplicate to false to just
// register string in context, without duplicating
const char*
duplicateString(const char* str, bool duplicate);

// try to get interned string for given char pointer
const char*
getInternedString(const char* str);

// try to free memory for interned string
void
releaseInternedString(const char* str);

void
contextPop();

void
contextPush(std::unordered_map<std::string, Object*> table,
            int flags,
            Object* instance);

// set ex handler, could be lambda or ordinary function, 0 parameters signature
void
contextSetExHandler(Object* handler);

// recover from exception to last safe point, requires UNHANDLED_EX flag
void
runtimeRecover();

// set checkpoint for recovering to next global statement
int
setGlobalCheckpoint();

// try to return from fn using longjmp
void
returnFromFunction();

void
contextSetLastEval(Object* obj);

Object*
contextGetLastEval();

int
contextGetFlags();

void
contextSetFlag(int flag);

void
contextResetFlag(int flag);

// get current class instance
Object*
contextGetCurrentInstance();

// search in global and current context
Object**
contextLookup(const char* name, bool supressErrors = false);

ClassInfo*
contextLookupClass(const char* className);

// look up class member of given class, whether its a field or method overload.
// set lookForStatic to true if looking specifically for static only members.
// set supressErrors to true to disable errors
Object**
contextLookupClassMember(const char* className,
                         const char* rawMemberName,
                         const char* overloadedMemberName,
                         bool lookForStatic,
                         bool supressErrors = false);

// increment object refcount
void
retainObject(Object* obj);

// decrement object refcount and try to release it if it gets to 0
void
releaseObject(Object* obj);

// try to register object in current context
bool
registerObject(const char* objName, Object* obj);

void
printObject(Object* obj);

// register global function
void
registerFunction(const char* fnName, Object* fnObj);

// register class
void
registerClass(const char* className,
              bool isPrimitive,
              std::vector<ClassMember> members);

void
setError(bool exit, const char* str, ...);

void
runtimeSetStrictMode(bool strict);

bool
runtimeGetStrictMode();

// call function, function name may be null as its use only in error messages.
// instance used only if flag IN_METHOD passed
void
callFunction(const char* fnName,
             Function* fn,
             std::vector<Object*> args,
             int flags,
             Object* instance);

void
callLambda(Lambda* lambda, std::vector<Object*> args);

// call class member, if instance is nullptr then assuming that calling static
// member
void
callClassMember(const char* className,
                const char* memberName,
                Object* instance,
                std::vector<Object*> args);

void
callBinaryOp(OperatorType opType, Object* lhs, Object* rhs);

void
callUnaryOp(OperatorType opType, Object* expr);

// resolve lhs and return pointer to pointer to registered object, so it can be
// changed which name/array element etc points at which object
Object**
resolveLHS(ASTNode* lhsNode, bool suppressErrors = false);