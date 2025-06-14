#include "ast.h"
#include "bnf.tab.h"
#include "lexer.h"
#include <stdlib.h>

extern ASTNode* astRoot;
extern void
yyerror(const char* s);

int
yylex(void)
{
  Token* tok = lexer();
  switch (tok->type) {
    case TokenType::STRING:
      yylval.stringValue = tok->tok;
      return STRING;
    case TokenType::NUMBER:
      yylval.numberValue = atof(tok->tok);
      free((void*)tok->tok);
      return NUMBER;
    case TokenType::TRUE:
      yylval.boolValue = true;
      return TRUE;
    case TokenType::FALSE:
      yylval.boolValue = false;
      return FALSE;
    case TokenType::NAME:
      yylval.stringValue = tok->tok;
      return NAME;
    /* … усі оператори … */
    case TokenType::MINUS:
      yylval.opType = MINUS;
      return MINUS;
    case TokenType::PLUS:
      yylval.opType = PLUS;
      return PLUS;
    case TokenType::BANG:
      yylval.opType = BANG;
      return BANG;
    case TokenType::STAR:
      yylval.opType = STAR;
      return STAR;
    case TokenType::SLASH:
      yylval.opType = SLASH;
      return SLASH;
    case TokenType::MOD:
      yylval.opType = MOD;
      return MOD;
    case TokenType::EQUALS:
      yylval.opType = EQUALS;
      return EQUALS;
    case TokenType::UNARY_DEC:
      yylval.opType = UNARY_DEC;
      return UNARY_DEC;
    case TokenType::UNARY_INC:
      yylval.opType = UNARY_INC;
      return UNARY_INC;
    case TokenType::BIT_SHIFTL:
      yylval.opType = BIT_SHIFTL;
      return BIT_SHIFTL;
    case TokenType::BIT_SHIFTR:
      yylval.opType = BIT_SHIFTR;
      return BIT_SHIFTR;
    case TokenType::BIT_AND:
      yylval.opType = BIT_AND;
      return BIT_AND;
    case TokenType::BIT_OR:
      yylval.opType = BIT_OR;
      return BIT_OR;
    case TokenType::BIT_XOR:
      yylval.opType = BIT_XOR;
      return BIT_XOR;
    case TokenType::COND_GT:
      yylval.opType = COND_GT;
      return COND_GT;
    case TokenType::COND_GEQ:
      yylval.opType = COND_GEQ;
      return COND_GEQ;
    case TokenType::COND_LT:
      yylval.opType = COND_LT;
      return COND_LT;
    case TokenType::COND_LEQ:
      yylval.opType = COND_LEQ;
      return COND_LEQ;
    case TokenType::COND_EQ:
      yylval.opType = COND_EQ;
      return COND_EQ;
    case TokenType::COND_NEQ:
      yylval.opType = COND_NEQ;
      return COND_NEQ;
    case TokenType::LOGIC_AND:
      yylval.opType = LOGIC_AND;
      return LOGIC_AND;
    case TokenType::LOGIC_OR:
      yylval.opType = LOGIC_OR;
      return LOGIC_OR;
    case TokenType::ASSIGN_ADD:
      yylval.opType = ASSIGN_ADD;
      return ASSIGN_ADD;
    case TokenType::ASSIGN_SUB:
      yylval.opType = ASSIGN_SUB;
      return ASSIGN_SUB;
    case TokenType::ASSIGN_DIV:
      yylval.opType = ASSIGN_DIV;
      return ASSIGN_DIV;
    case TokenType::ASSIGN_MUL:
      yylval.opType = ASSIGN_MUL;
      return ASSIGN_MUL;
    case TokenType::ASSIGN_MOD:
      yylval.opType = ASSIGN_MOD;
      return ASSIGN_MOD;
    case TokenType::ASSIGN_XOR:
      yylval.opType = ASSIGN_XOR;
      return ASSIGN_XOR;
    case TokenType::ASSIGN_SHIFTL:
      yylval.opType = ASSIGN_SHIFTL;
      return ASSIGN_SHIFTL;
    case TokenType::ASSIGN_SHIFTR:
      yylval.opType = ASSIGN_SHIFTR;
      return ASSIGN_SHIFTR;
    case TokenType::ASSIGN_AND:
      yylval.opType = ASSIGN_AND;
      return ASSIGN_AND;
    case TokenType::ASSIGN_OR:
      yylval.opType = ASSIGN_OR;
      return ASSIGN_OR;
    case TokenType::DOT:
      yylval.opType = DOT;
      return DOT;

    case TokenType::WHL:
      return WHL;
    case TokenType::FOR:
      return FOR;
    case TokenType::SWITCH:
      return SWITCH;
    case TokenType::BRK:
      return BRK;
    case TokenType::DEFAULT:
      return DEFAULT;
    case TokenType::CLASS:
      return CLASS;
    case TokenType::CONST:
      return CONST;
    case TokenType::VAR:
      return VAR;
    case TokenType::DEL:
      return DEL;
    case TokenType::STATIC:
      return STATIC;
    case TokenType::HIDDEN:
      return HIDDEN;
    case TokenType::ENUM:
      return ENUM;

    case TokenType::LPAREN:
      return LPAREN;
    case TokenType::RPAREN:
      return RPAREN;
    case TokenType::LBRACE:
      return LBRACE;
    case TokenType::RBRACE:
      return RBRACE;
    case TokenType::LSQUARE:
      return LSQUARE;
    case TokenType::RSQUARE:
      return RSQUARE;
    case TokenType::COLON:
      return COLON;
    case TokenType::COMMA:
      return COMMA;
    case TokenType::HASH:
      return HASH;
    case TokenType::DELIMITER:
      return DELIMITER;

    case TokenType::FN:
      return FN;
    case TokenType::RET:
      return RET;
    case TokenType::IF:
      return IF;
    case TokenType::ELSE:
      return ELSE;
    case TokenType::TOSS:
      return TOSS;

    case TokenType::ENDOFTOKENS:
    case TokenType::INVALID:
    default:
      return YYEOF;
  }
}
