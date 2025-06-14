#pragma once
#include "error.h"
#include "errorinfo.h"
#include <string>
#include <sys/types.h>
#include <vector>

typedef enum class TokenType
{
  INVALID,

  // operators
  MINUS,
  PLUS,
  BANG,
  STAR,
  SLASH,
  MOD,
  LPAREN,
  RPAREN,
  LBRACE,
  RBRACE,
  EQUALS,
  UNARY_DEC,
  UNARY_INC,
  BIT_SHIFTL,
  BIT_SHIFTR,
  BIT_AND,
  BIT_OR,
  BIT_XOR,
  COND_GT,
  COND_GEQ,
  COND_LT,
  COND_LEQ,
  COND_EQ,
  COND_NEQ,
  LOGIC_AND,
  LOGIC_OR,
  ASSIGN_ADD,
  ASSIGN_SUB,
  ASSIGN_DIV,
  ASSIGN_MUL,
  ASSIGN_MOD,
  ASSIGN_XOR,
  ASSIGN_SHIFTL,
  ASSIGN_SHIFTR,
  ASSIGN_AND,
  ASSIGN_OR,

  // colon for semantic separation
  COLON,
  // comma for arguments/types separation
  COMMA,
  // hash for lambda indication
  HASH,

  // literals
  STRING,
  NUMBER,
  TRUE,
  FALSE,

  // custom names and other objects
  NAME,

  // access-level
  DOT,
  LSQUARE,
  RSQUARE,

  // keywords
  FN,
  RET,
  IF,
  ELSE,
  WHL,
  FOR,
  SWITCH,
  BRK,
  DEFAULT,
  CLASS,
  CONST,
  LOCAL,
  TOSS,
  VAR,
  DEL,
  STATIC,
  HIDDEN,
  ENUM,

  // delimiter (newline)
  DELIMITER,

  ENDOFTOKENS,
} TokenType;

typedef struct
{
  const char* tok;
  TokenType type;
} Token;

#ifdef LEXER_DBG
std::string
lexerTypeName(TokenType type);
#endif
void
lexerInit();
void
lexerSetText(char* str);
// recognize next token
Token*
lexer();
void
lexerClearErrors();
std::vector<ErrorInfo>&
lexerGetErrors();
