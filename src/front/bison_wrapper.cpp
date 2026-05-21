#include "front/bison_wrapper.h"
#include "front/lexer.h"
#include <stdlib.h>

namespace yy {
int
yylex(parser::value_type* yylval, Lexer& lexer)
{
  Token tok = lexer.lexer();
  switch (tok.type) {
    case TokenType::STRING:
      yylval->stringValue = tok.str;
      return parser::token_kind_type::STRING;
    case TokenType::NUMBER:
      yylval->numberValue = atof(tok.str);
      free((void*)tok.str);
      return parser::token_kind_type::NUMBER;
    case TokenType::TRUE:
      yylval->boolValue = true;
      return parser::token_kind_type::TRUE;
    case TokenType::FALSE:
      yylval->boolValue = false;
      return parser::token_kind_type::FALSE;
    case TokenType::NAME:
      yylval->stringValue = tok.str;
      return parser::token_kind_type::NAME;
    case TokenType::MINUS:
      yylval->opType = TokenType::MINUS;
      return parser::token_kind_type::MINUS;
    case TokenType::PLUS:
      yylval->opType = TokenType::PLUS;
      return parser::token_kind_type::PLUS;
    case TokenType::STAR:
      yylval->opType = TokenType::STAR;
      return parser::token_kind_type::STAR;
    case TokenType::SLASH:
      yylval->opType = TokenType::SLASH;
      return parser::token_kind_type::SLASH;
    case TokenType::MOD:
      yylval->opType = TokenType::MOD;
      return parser::token_kind_type::MOD;
    case TokenType::BANG:
      yylval->opType = TokenType::BANG;
      return parser::token_kind_type::BANG;
    case TokenType::UNARY_DEC:
      yylval->opType = TokenType::UNARY_DEC;
      return parser::token_kind_type::UNARY_DEC;
    case TokenType::UNARY_INC:
      yylval->opType = TokenType::UNARY_INC;
      return parser::token_kind_type::UNARY_INC;
    case TokenType::BIT_SHIFTL:
      yylval->opType = TokenType::BIT_SHIFTL;
      return parser::token_kind_type::BIT_SHIFTL;
    case TokenType::BIT_SHIFTR:
      yylval->opType = TokenType::BIT_SHIFTR;
      return parser::token_kind_type::BIT_SHIFTR;
    case TokenType::BIT_AND:
      yylval->opType = TokenType::BIT_AND;
      return parser::token_kind_type::BIT_AND;
    case TokenType::BIT_OR:
      yylval->opType = TokenType::BIT_OR;
      return parser::token_kind_type::BIT_OR;
    case TokenType::BIT_XOR:
      yylval->opType = TokenType::BIT_XOR;
      return parser::token_kind_type::BIT_XOR;
    case TokenType::COND_LT:
      yylval->opType = TokenType::COND_LT;
      return parser::token_kind_type::COND_LT;
    case TokenType::COND_GT:
      yylval->opType = TokenType::COND_GT;
      return parser::token_kind_type::COND_GT;
    case TokenType::COND_LEQ:
      yylval->opType = TokenType::COND_LEQ;
      return parser::token_kind_type::COND_LEQ;
    case TokenType::COND_GEQ:
      yylval->opType = TokenType::COND_GEQ;
      return parser::token_kind_type::COND_GEQ;
    case TokenType::COND_EQ:
      yylval->opType = TokenType::COND_EQ;
      return parser::token_kind_type::COND_EQ;
    case TokenType::COND_NEQ:
      yylval->opType = TokenType::COND_NEQ;
      return parser::token_kind_type::COND_NEQ;
    case TokenType::LOGIC_AND:
      yylval->opType = TokenType::LOGIC_AND;
      return parser::token_kind_type::LOGIC_AND;
    case TokenType::LOGIC_OR:
      yylval->opType = TokenType::LOGIC_OR;
      return parser::token_kind_type::LOGIC_OR;
    case TokenType::EQUALS:
      yylval->opType = TokenType::EQUALS;
      return parser::token_kind_type::EQUALS;
    case TokenType::ASSIGN_ADD:
      yylval->opType = TokenType::ASSIGN_ADD;
      return parser::token_kind_type::ASSIGN_ADD;
    case TokenType::ASSIGN_SUB:
      yylval->opType = TokenType::ASSIGN_SUB;
      return parser::token_kind_type::ASSIGN_SUB;
    case TokenType::ASSIGN_DIV:
      yylval->opType = TokenType::ASSIGN_DIV;
      return parser::token_kind_type::ASSIGN_DIV;
    case TokenType::ASSIGN_MUL:
      yylval->opType = TokenType::ASSIGN_MUL;
      return parser::token_kind_type::ASSIGN_MUL;
    case TokenType::ASSIGN_MOD:
      yylval->opType = TokenType::ASSIGN_MOD;
      return parser::token_kind_type::ASSIGN_MOD;
    case TokenType::ASSIGN_XOR:
      yylval->opType = TokenType::ASSIGN_XOR;
      return parser::token_kind_type::ASSIGN_XOR;
    case TokenType::ASSIGN_SHIFTL:
      yylval->opType = TokenType::ASSIGN_SHIFTL;
      return parser::token_kind_type::ASSIGN_SHIFTL;
    case TokenType::ASSIGN_SHIFTR:
      yylval->opType = TokenType::ASSIGN_SHIFTR;
      return parser::token_kind_type::ASSIGN_SHIFTR;
    case TokenType::ASSIGN_AND:
      yylval->opType = TokenType::ASSIGN_AND;
      return parser::token_kind_type::ASSIGN_AND;
    case TokenType::ASSIGN_OR:
      yylval->opType = TokenType::ASSIGN_OR;
      return parser::token_kind_type::ASSIGN_OR;

    case TokenType::LPAREN:
      return parser::token_kind_type::LPAREN;
    case TokenType::RPAREN:
      return parser::token_kind_type::RPAREN;
    case TokenType::LBRACE:
      return parser::token_kind_type::LBRACE;
    case TokenType::RBRACE:
      return parser::token_kind_type::RBRACE;

    case TokenType::COLON:
      return parser::token_kind_type::COLON;
    case TokenType::COMMA:
      return parser::token_kind_type::COMMA;
      // case TokenType::HASH:
      //   return parser::token_kind_type::HASH;

    case TokenType::DOT:
      yylval->opType = TokenType::DOT;
      return parser::token_kind_type::DOT;
    case TokenType::LSQUARE:
      return parser::token_kind_type::LSQUARE;
    case TokenType::RSQUARE:
      return parser::token_kind_type::RSQUARE;

    case TokenType::FN:
      return parser::token_kind_type::FN;
    case TokenType::RET:
      return parser::token_kind_type::RET;
    case TokenType::IF:
      return parser::token_kind_type::IF;
    case TokenType::ELSE:
      return parser::token_kind_type::ELSE;
    case TokenType::WHL:
      return parser::token_kind_type::WHL;
    case TokenType::FOR:
      return parser::token_kind_type::FOR;
    case TokenType::SWITCH:
      return parser::token_kind_type::SWITCH;
    case TokenType::BRK:
      return parser::token_kind_type::BRK;
    case TokenType::DEFAULT:
      return parser::token_kind_type::DEFAULT;
    case TokenType::CLASS:
      return parser::token_kind_type::CLASS;
    case TokenType::CONST:
      return parser::token_kind_type::CONST;
    case TokenType::LOCAL:
      return parser::token_kind_type::LOCAL;
    case TokenType::VAR:
      return parser::token_kind_type::VAR;
    case TokenType::DEL:
      return parser::token_kind_type::DEL;
    case TokenType::STATIC:
      return parser::token_kind_type::STATIC;
    case TokenType::HIDDEN:
      return parser::token_kind_type::HIDDEN;
    case TokenType::ENUM:
      return parser::token_kind_type::ENUM;
    case TokenType::OP:
      return parser::token_kind_type::OP;
    case TokenType::TOSS:
      return parser::token_kind_type::TOSS;

    case TokenType::DELIMITER:
      return parser::token_kind_type::DELIMITER;

    case TokenType::ERROR:
      return parser::token_kind_type::ERROR;

    case TokenType::ENDOFTOKENS:
    default:
      return parser::token_kind_type::YYEOF;
  }
}

void
parser::error(const std::string& msg)
{
  fprintf(stderr, "Parse error: %s\n", msg.c_str());
}
}