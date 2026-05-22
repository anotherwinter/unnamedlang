#include "front/lexer.h"
#include "front/dfalg_out.h"

Lexer::Lexer(Diagnostics& diag)
  : _diag(diag)
{
  lexerInsertKeyword({ "fn", TokenType::FN });
  lexerInsertKeyword({ "ret", TokenType::RET });
  lexerInsertKeyword({ "if", TokenType::IF });
  lexerInsertKeyword({ "else", TokenType::ELSE });
  lexerInsertKeyword({ "whl", TokenType::WHL });
  lexerInsertKeyword({ "for", TokenType::FOR });
  lexerInsertKeyword({ "switch", TokenType::SWITCH });
  lexerInsertKeyword({ "brk", TokenType::BRK });
  lexerInsertKeyword({ "default", TokenType::DEFAULT });
  lexerInsertKeyword({ "class", TokenType::CLASS });
  lexerInsertKeyword({ "const", TokenType::CONST });
  lexerInsertKeyword({ "true", TokenType::TRUE });
  lexerInsertKeyword({ "false", TokenType::FALSE });
  lexerInsertKeyword({ "local", TokenType::LOCAL });
  lexerInsertKeyword({ "toss", TokenType::TOSS });
  lexerInsertKeyword({ "var", TokenType::VAR });
  lexerInsertKeyword({ "del", TokenType::DEL });
  lexerInsertKeyword({ "static", TokenType::STATIC });
  lexerInsertKeyword({ "hidden", TokenType::HIDDEN });
  lexerInsertKeyword({ "enum", TokenType::ENUM });
  lexerInsertKeyword({ "op", TokenType::OP });
}

void
Lexer::load(const std::string& str)
{
  _str = str;
  _ptr = _str.c_str();
  _end = _str.end().base();
  _line = 1;
  _col = 1;
  _error = false;
  _eof = str.empty();
  _lastTok = { nullptr, TokenType::ENDOFTOKENS, _line, _col };
}

Token
Lexer::lexer()
{
  if (_eof)
    return { nullptr, TokenType::ENDOFTOKENS, _line, _col };

  Token tok;
  skipSpaces();
  char c = getChar();

  tok.line = _line;
  tok.col = _col;

  advance();

  switch (c) {
    // handle CRLF/CR line breaks
    case '\r': {
      // consume newline if it comes after CR
      consume('\n');
      tok.type = TokenType::DELIMITER;
      break;
    }
    // handle LF line break
    case '\n': {
      tok.type = TokenType::DELIMITER;
      break;
    }
    case '-': {
      if (consume('-'))
        tok.type = TokenType::UNARY_DEC;
      else if (consume('='))
        tok.type = TokenType::ASSIGN_SUB;
      else
        tok.type = TokenType::MINUS;

      break;
    }
    case '+': {
      if (consume('+'))
        tok.type = TokenType::UNARY_INC;
      else if (consume('='))
        tok.type = TokenType::ASSIGN_ADD;
      else
        tok.type = TokenType::PLUS;

      break;
    }
    case '!': {
      if (consume('='))
        tok.type = TokenType::COND_NEQ;
      else
        tok.type = TokenType::BANG;

      break;
    }
    case '*': {
      if (consume('='))
        tok.type = TokenType::ASSIGN_MUL;
      else
        tok.type = TokenType::STAR;

      break;
    }
    case '/': {
      if (consume('='))
        tok.type = TokenType::ASSIGN_DIV;
      else
        tok.type = TokenType::SLASH;

      break;
    }
    case '%': {
      if (consume('='))
        tok.type = TokenType::ASSIGN_MOD;
      else
        tok.type = TokenType::MOD;

      break;
    }
    case '(': {
      tok.type = TokenType::LPAREN;
      break;
    }
    case ')': {
      tok.type = TokenType::RPAREN;
      break;
    }
    case '{': {
      tok.type = TokenType::LBRACE;
      break;
    }
    case '}': {
      tok.type = TokenType::RBRACE;
      break;
    }
    case '=': {
      if (consume('='))
        tok.type = TokenType::COND_EQ;
      else
        tok.type = TokenType::EQUALS;

      break;
    }
    case '<': {
      if (consume('='))
        tok.type = TokenType::COND_LEQ;
      else if (consume('<')) {
        if (consume('='))
          tok.type = TokenType::ASSIGN_SHIFTL;
        else
          tok.type = TokenType::BIT_SHIFTL;
      } else
        tok.type = TokenType::COND_LT;

      break;
    }
    case '>': {
      if (consume('='))
        tok.type = TokenType::COND_GEQ;
      else if (consume('>')) {
        if (consume('='))
          tok.type = TokenType::ASSIGN_SHIFTR;
        else
          tok.type = TokenType::BIT_SHIFTR;
      } else
        tok.type = TokenType::COND_GT;

      break;
    }
    case '&': {
      if (consume('&'))
        tok.type = TokenType::LOGIC_AND;
      else if (consume('='))
        tok.type = TokenType::ASSIGN_AND;
      else
        tok.type = TokenType::BIT_AND;

      break;
    }
    case '|': {
      if (consume('|'))
        tok.type = TokenType::LOGIC_OR;
      else if (consume('='))
        tok.type = TokenType::ASSIGN_OR;
      else
        tok.type = TokenType::BIT_OR;

      break;
    }
    case '^': {
      if (consume('='))
        tok.type = TokenType::ASSIGN_XOR;
      else
        tok.type = TokenType::BIT_XOR;
      break;
    }
    case '.': {
      tok.type = TokenType::DOT;
      break;
    }
    case '[': {
      tok.type = TokenType::LSQUARE;
      break;
    }
    case ']': {
      tok.type = TokenType::RSQUARE;
      break;
    }
    case ':': {
      tok.type = TokenType::COLON;
      break;
    }
    case ',': {
      tok.type = TokenType::COMMA;
      break;
    }
    // TODO: lambdas
    // case '#': {
    //   tok.type = TokenType::HASH;
    //   break;
    // }
    case '~': {
      // skip comments
      lexerComment();
      tok.type = TokenType::COMMENT;

      break;
    }
    case '\'':
    case '\"': {
      tok.str = lexerString(c);
      tok.type = TokenType::STRING;
      break;
    }
    default: {
      // if current character is digit, then tokenize number
      if (isDigit(c)) {
        tok.str = lexerNumber();
        tok.type = TokenType::NUMBER;
        break;
      }

      // if its underscore or a-Z char, tokenize keyword/name
      if (c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
        const char* before = ptr() - 1;
        bool isKeyword = lexerKeyword(tok.type);
        // if its not a keyword then assume its a name
        if (!isKeyword) {
          tok.str = lexerName();
          tok.type = TokenType::NAME;
        }
#ifdef DBG
        else {
          tok.str = strndup(before, ptr() - before);
        }
#endif
      } else {
        error(LexerError::UnknownToken);
      }
    }
  }

  // if has error, then recover (advance to next newline)
  if (_error) {
    tok.type = TokenType::ERROR;
    recover();
  }

  _lastTok = tok;

  return tok;
}

const char*
Lexer::errorMsg(LexerError code)
{
  // TODO: errors
  return "lexer error";
}

const char*
Lexer::lexerTypeName(TokenType type)
{
#ifndef DBG
  return "no debug";
#endif

  switch (type) {
    case TokenType::PLUS: {
      return "+";
    }
    case TokenType::MINUS: {
      return "-";
    }
    case TokenType::STAR: {
      return "*";
    }
    case TokenType::SLASH: {
      return "/";
    }
    case TokenType::MOD: {
      return "%";
    }
    case TokenType::LPAREN: {
      return "(";
    }
    case TokenType::RPAREN: {
      return ")";
    }
    case TokenType::LBRACE: {
      return "{";
    }
    case TokenType::RBRACE: {
      return "}";
    }
    case TokenType::EQUALS: {
      return "=";
    }
    case TokenType::UNARY_DEC: {
      return "--";
    }
    case TokenType::UNARY_INC: {
      return "++";
    }
    case TokenType::BIT_SHIFTL: {
      return "<<";
    }
    case TokenType::BIT_SHIFTR: {
      return ">>";
    }
    case TokenType::BIT_AND: {
      return "&";
    }
    case TokenType::BIT_OR: {
      return "|";
    }
    case TokenType::BIT_XOR: {
      return "^";
    }
    case TokenType::COND_GT: {
      return ">";
    }
    case TokenType::COND_GEQ: {
      return ">=";
    }
    case TokenType::COND_LT: {
      return "<";
    }
    case TokenType::COND_LEQ: {
      return "<=";
    }
    case TokenType::COND_EQ: {
      return "==";
    }
    case TokenType::COND_NEQ: {
      return "!=";
    }
    case TokenType::LOGIC_AND: {
      return "&&";
    }
    case TokenType::LOGIC_OR: {
      return "||";
    }
    case TokenType::ASSIGN_ADD: {
      return "+=";
    }
    case TokenType::ASSIGN_SUB: {
      return "-=";
    }
    case TokenType::ASSIGN_DIV: {
      return "/=";
    }
    case TokenType::ASSIGN_MUL: {
      return "*=";
    }
    case TokenType::ASSIGN_MOD: {
      return "%=";
    }
    case TokenType::ASSIGN_SHIFTL: {
      return "<<=";
    }
    case TokenType::ASSIGN_SHIFTR: {
      return ">>=";
    }
    case TokenType::ASSIGN_AND: {
      return "&=";
    }
    case TokenType::ASSIGN_OR: {
      return "|=";
    }
    case TokenType::BANG: {
      return "!";
    }
    case TokenType::COLON: {
      return ":";
    }
    case TokenType::COMMA: {
      return ",";
    }
    // case TokenType::HASH: {
    //   return "#";
    // }
    case TokenType::STRING: {
      return "string";
    }
    case TokenType::NUMBER: {
      return "number";
    }
    case TokenType::TRUE: {
      return "true";
    }
    case TokenType::FALSE: {
      return "false";
    }
    case TokenType::NAME: {
      return "name";
    }
    case TokenType::DOT: {
      return ".";
    }
    case TokenType::LSQUARE: {
      return "[";
    }
    case TokenType::RSQUARE: {
      return "]";
    }
    case TokenType::DELIMITER: {
      return "delimiter";
    }
    case TokenType::ENDOFTOKENS: {
      return "end of tokens";
    }
    default:
      return "keyword";
  }
}

void
Lexer::recover()
{
  while (!_eof && !match('\n'))
    advance();

  consume('\n');
  _error = false;
}

void
Lexer::lexerComment()
{
  // handle multiline comment
  if (consume('!')) {
    bool end = false;

    // advance to the end of comment or eof
    while (!_eof) {
      if (consume('!') && consume('~')) {
        end = true;
        break;
      } else
        advance();
    }

    if (!end)
      error(LexerError::UnterminatedComment);
  }
  // handle one line comment
  else {
    while (!_eof && !match('\n'))
      advance();
  }
}

const char*
Lexer::lexerName()
{
  // set begin as ptr -1 since one char was consumed prior this call
  const char* begin = ptr() - 1;
  int chars = 1;
  while (match('_') || isLetter(getChar()) || isDigit(getChar())) {
    ++chars;
    advance();
  }

  return strndup(begin, chars);
}

const char*
Lexer::lexerString(char quote)
{
  int chars = 0;
  bool escape = false;
  const char* begin = ptr();

  while (!_eof) {
    if (!escape && (match('\n') || match(quote)))
      break;

    ++chars;

    if (match('\\'))
      escape = true;
    else if (escape)
      escape = false;

    advance();
  }

  if (!match(quote)) {
    error(LexerError::UnterminatedString);
    return nullptr;
  }

  advance();

  return strndup(begin, chars);
}

bool
Lexer::lexerKeyword(TokenType& type)
{
  // set begin as ptr -1 since one char was consumed prior this call
  const char* begin = ptr() - 1;
  for (auto it = _keywordsLengths.begin(); it != _keywordsLengths.end(); ++it) {
    size_t len = *it;
    auto kw = _keywords.find({ begin, len });
    if (kw != _keywords.end()) {
      type = kw->second;
      // advance to len - 1 because one char was already advanced before call
      advance(len - 1);
      return true;
    }
  }

  return false;
}

const char*
Lexer::lexerNumber()
{
  // set begin as ptr -1 since one char was consumed prior this call
  const char* begin = ptr() - 1;
  int chars = 1;
  char c = getChar();
  int state = INITIAL_STATE;
  state = transition(state, c);

  while (state != -1) {
    ++chars;
    advance();
    c = getChar();
    state = transition(state, c);
  }

  return strndup(begin, chars);
}

void
Lexer::error(LexerError code)
{
  _error = true;
  _diag.putMsg(static_cast<ErrorCode>(code), _line, _col);
}