#include "lexer.h"
#include "dfalg_out.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <set>
#include <unordered_map>

#define KEYWORD_RESERVED -1

enum class LexerMode
{
  DEFAULT,
  INTERP,
  INTERP_IN_EXPR
};

std::unordered_map<std::string, TokenType> _keywords;
std::set<int> _keywordsLengths;
char* _input;
std::vector<ErrorInfo> _errors;
ulong _currentLine = 0;
ulong _currentCol = 0;
bool _hasError = false;

bool
isDigit(char c);
bool
isLetter(char c);
void
lexerMultilineComment();
void
lexerComment();
const char*
lexerNumber();
const char*
lexerString();
const char*
lexerInterpText(char quote);
bool
lexerKeyword(char* str, TokenType& type);
const char*
lexerVariable();
void
lexerInsertKeyword(Token tok);
void
lexerSetError(const std::string& msg);
bool
consumeChar(char expected);
void
advanceInput(int chars = 1);
char
getCurrentChar();
const char*
getCurrentInputPtr();
bool
match(char expected);
bool
matchNext(char expected);
void
recover();

#ifdef LEXER_DBG
std::string
lexerTypeName(TokenType type)
{
  switch (type) {
    case TokenType::INVALID: {
      return "invalid";
    }
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
    case TokenType::HASH: {
      return "#";
    }
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

#endif

void
lexerInit()
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
}

void
lexerSetText(char* str)
{
  _input = str;
  _errors.clear();
  _currentLine = 0;
  _currentCol = 0;
}

Token*
lexer()
{
  TokenType type = TokenType::INVALID;
  const char* tokStr = nullptr;
  Token* tok = (Token*)malloc(sizeof(Token));

  if (!match('\0')) {
    tokStr = nullptr;
    _hasError = false;

    // ignore spaces and tabs
    while (match(' ') || match('\t'))
      advanceInput();

    char c = *_input;

    switch (c) {
      // handle CRLF/CR line breaks
      case '\r': {
        advanceInput();
        // consume newline if it comes after CR
        consumeChar('\n');
        ++_currentLine;
        type = TokenType::DELIMITER;
        break;
      }
      // handle LF line break
      case '\n': {
        advanceInput();
        ++_currentLine;
        type = TokenType::DELIMITER;
        break;
      }
      case '-': {
        advanceInput();
        if (consumeChar('-'))
          type = TokenType::UNARY_DEC;
        else if (consumeChar('='))
          type = TokenType::ASSIGN_SUB;
        else
          type = TokenType::MINUS;

        break;
      }
      case '+': {
        advanceInput();
        if (consumeChar('+'))
          type = TokenType::UNARY_INC;
        else if (consumeChar('='))
          type = TokenType::ASSIGN_ADD;
        else
          type = TokenType::PLUS;

        break;
      }
      case '!': {
        advanceInput();
        if (consumeChar('='))
          type = TokenType::COND_NEQ;
        else
          type = TokenType::BANG;

        break;
      }
      case '*': {
        advanceInput();
        if (consumeChar('='))
          type = TokenType::ASSIGN_MUL;
        else
          type = TokenType::STAR;

        break;
      }
      case '/': {
        advanceInput();
        if (consumeChar('='))
          type = TokenType::ASSIGN_DIV;
        else
          type = TokenType::SLASH;

        break;
      }
      case '%': {
        advanceInput();
        if (consumeChar('='))
          type = TokenType::ASSIGN_MOD;
        else
          type = TokenType::MOD;

        break;
      }
      case '(': {
        advanceInput();
        type = TokenType::LPAREN;
        break;
      }
      case ')': {
        advanceInput();
        type = TokenType::RPAREN;
        break;
      }
      case '{': {
        advanceInput();
        type = TokenType::LBRACE;
        break;
      }
      case '}': {
        advanceInput();
        type = TokenType::RBRACE;

        break;
      }
      case '=': {
        advanceInput();
        if (consumeChar('='))
          type = TokenType::COND_EQ;
        else
          type = TokenType::EQUALS;

        break;
      }
      case '<': {
        advanceInput();
        if (consumeChar('='))
          type = TokenType::COND_LEQ;
        else if (consumeChar('<')) {
          if (consumeChar('='))
            type = TokenType::ASSIGN_SHIFTL;
          else
            type = TokenType::BIT_SHIFTL;
        } else
          type = TokenType::COND_LT;

        break;
      }
      case '>': {
        advanceInput();
        if (consumeChar('='))
          type = TokenType::COND_GEQ;
        else if (consumeChar('>')) {
          if (consumeChar('='))
            type = TokenType::ASSIGN_SHIFTR;
          else
            type = TokenType::BIT_SHIFTR;
        } else
          type = TokenType::COND_GT;

        break;
      }
      case '&': {
        advanceInput();
        if (consumeChar('&'))
          type = TokenType::LOGIC_AND;
        else if (consumeChar('='))
          type = TokenType::ASSIGN_AND;
        else
          type = TokenType::BIT_AND;

        break;
      }
      case '|': {
        advanceInput();
        if (consumeChar('|'))
          type = TokenType::LOGIC_OR;
        else if (consumeChar('='))
          type = TokenType::ASSIGN_OR;
        else
          type = TokenType::BIT_OR;

        break;
      }
      case '^': {
        advanceInput();
        if (consumeChar('='))
          type = TokenType::ASSIGN_XOR;
        else
          type = TokenType::BIT_XOR;
        break;
      }
      case '.': {
        advanceInput();
        type = TokenType::DOT;
        break;
      }
      case '[': {
        advanceInput();
        type = TokenType::LSQUARE;
        break;
      }
      case ']': {
        advanceInput();
        type = TokenType::RSQUARE;
        break;
      }
      case ':': {
        advanceInput();
        type = TokenType::COLON;
        break;
      }
      case ',': {
        advanceInput();
        type = TokenType::COMMA;
        break;
      }
      case '#': {
        advanceInput();
        type = TokenType::HASH;
        break;
      }
      case '~': {
        advanceInput();
        if (consumeChar('!')) {
          lexerMultilineComment();
        } else
          lexerComment();

        break;
      }
      case '\'':
      case '\"': {
        tokStr = lexerString();
        type = TokenType::STRING;
        break;
      }
      default: {
        // if current character is digit, then tokenize number
        if (isDigit(c)) {
          tokStr = lexerNumber();
          type = TokenType::NUMBER;
          break;
        }

        // if its underscore or a-Z char, tokenize keyword/name
        if (c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
          bool isKeyword = lexerKeyword(_input, type);
          // if its not a keyword then assume its a name
          if (!isKeyword) {
            tokStr = lexerVariable();
            type = TokenType::NAME;
          }
        } else {
          lexerSetError("Unknown token");
        }
      }
    }

    // if no error, then add token if in default mode (assuming token addition
    // is handled in other modes subroutines like lexerInterpolation)
    if (!_hasError) {
      tok->tok = tokStr;
      tok->type = type;
    }
    // if has error, then recover (advance to next newline)
    else {
      recover();
    }
  } else {
    // reached null-terminator, add end of tokens indicator to token stream so
    // that parser can work correctly
    tok->tok = nullptr;
    tok->type = TokenType::ENDOFTOKENS;
  }

  return tok;
}

void
lexerClearErrors()
{
  _errors.clear();
}

std::vector<ErrorInfo>&
lexerGetErrors()
{
  return _errors;
}

bool
isDigit(char c)
{
  return c >= '0' && c <= '9';
}

bool
isLetter(char c)
{
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool
consumeChar(char expected)
{
  if (*_input == expected) {
    advanceInput();
    return true;
  }

  return false;
}

void
advanceInput(int chars)
{
  char* end = _input + chars;
  while (_input != end) {
    if (match('\r')) {
      ++_currentLine;
      // keep columns as 0-indexed
      _currentCol = -1;
      if (matchNext('\n')) {
        ++_input;
      }
    } else if (match('\n')) {
      ++_currentLine;
      // keep columns as 0-indexed
      _currentCol = -1;
    }
    ++_input;
    ++_currentCol;
  }
}

char
getCurrentChar()
{
  return *_input;
}

const char*
getCurrentInputPtr()
{
  return _input;
}

bool
match(char expected)
{
  return expected == *_input;
}

bool
matchNext(char expected)
{
  return expected == *(_input + 1);
}

void
recover()
{
  while (!match('\n') && !match('\0')) {
    if (match('\\') && matchNext('\n'))
      advanceInput(2);
    else
      advanceInput();
  }

  consumeChar('\n');
  _hasError = false;
}

void
lexerMultilineComment()
{
  int chars = 0;
  while (!match('\0')) {
    ++chars;
    if (match('!') && match('~')) {
      ++chars;
      break;
    }

    advanceInput();
  }
}

void
lexerComment()
{
  int chars = 0;
  while (!match('\n') && !match('\0')) {
    ++chars;
    advanceInput();
  }
}

const char*
lexerVariable()
{
  const char* begin = getCurrentInputPtr();
  int chars = 0;
  while (match('_') || isLetter(getCurrentChar()) ||
         isDigit(getCurrentChar())) {
    ++chars;
    advanceInput();
  }

  return strndup(begin, chars);
}

const char*
lexerString()
{
  char quote = getCurrentChar();
  // skip opening quote
  advanceInput();
  int chars = 0;
  bool escape = false;
  const char* begin = getCurrentInputPtr();

  while (!match('\0')) {
    if (!escape && (match('\n') || match(quote)))
      break;

    ++chars;

    if (match('\\'))
      escape = true;
    else if (escape)
      escape = false;

    advanceInput();
  }

  if (!match(quote)) {
    lexerSetError("Unterminated string");
    return nullptr;
  }

  advanceInput();

  return strndup(begin, chars);
}

const char*
lexerInterpText(char quote)
{
  int chars = 0;
  bool escape = false;
  const char* begin = getCurrentInputPtr();

  while (!match('\0')) {
    if (!escape && (match(quote) || match('{') || match('\n')))
      break;

    ++chars;

    // if current symbol is \ then escape next symbol
    if (match('\\'))
      escape = true;
    // if escaping was enabled in previous iteration, disable it (this is
    // necessary so escaping terminating symbols works correctly)
    else if (escape)
      escape = false;

    advanceInput();
  }

  if (!match(quote) && !match('{'))
    lexerSetError("Unterminated string");

  return strndup(begin, chars);
}

bool
lexerKeyword(char* str, TokenType& type)
{
  for (auto it = _keywordsLengths.begin(); it != _keywordsLengths.end(); ++it) {
    int len = *it;
    char savedChar = *(str + len);
    *(str + len) = '\0';
    auto kw = _keywords.find({ str });
    *(str + len) = savedChar;
    if (kw != _keywords.end()) {
      type = kw->second;
      advanceInput(len);
      return true;
    }
  }

  return false;
}

const char*
lexerNumber()
{
  const char* begin = getCurrentInputPtr();
  int chars = 0;
  char c = getCurrentChar();
  int state = INITIAL_STATE;
  state = transition(state, c);
  while (state != -1) {
    ++chars;
    advanceInput();
    c = getCurrentChar();
    state = transition(state, c);
  }

  return strndup(begin, chars);
}

void
lexerInsertKeyword(Token tok)
{
  _keywords.insert({ tok.tok, tok.type });
  _keywordsLengths.insert(strlen(tok.tok));
}

void
lexerSetError(const std::string& msg)
{
  _errors.push_back({ msg, _currentLine, _currentCol });
  _hasError = true;
}