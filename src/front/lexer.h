#pragma once
#include "../diagnostics.h"
#include "../errorcode_bases.h"
#include <cstring>
#include <set>
#include <string>
#include <unordered_map>

enum class TokenType : uint16_t
{
#define X2(Name, Val) Name = Val,
#define X1(Name) Name,
#define GET_X(_1, _2, NAME, ...) NAME
#define X(...) GET_X(__VA_ARGS__, X2, X1)(__VA_ARGS__)

#include "lex_tokentypes.def"
#undef X
#undef GET_X
#undef X1
#undef X2
};

typedef struct
{
  const char* str;
  TokenType type;
  size_t line;
  size_t col;
} Token;

// TODO: implement error messaging and tie to diag
enum class LexerError : ErrorCode
{
  UnknownToken = LEX_ERR_BASE,
  NoEscapedChar,
  UnterminatedComment,
  UnterminatedString
};

class Lexer
{
public:
  Lexer(Diagnostics& diag);
  void load(const std::string& str);
  Token lexer();
  static const char* errorMsg(LexerError code);

#ifdef LEXER_DBG
  const char* lexerTokenName(TokenType type);
#endif

private:
  std::string _str;
  size_t _line;
  size_t _col;
  const char* _ptr;
  const char* _end;
  bool _error;
  bool _eof;
  std::unordered_map<std::string, TokenType> _keywords;
  std::set<size_t> _keywordsLengths;
  Diagnostics& _diag;

  inline char getChar() { return *_ptr; }

  inline void skipSpaces()
  {
    // ignore spaces and tabs
    while (match(' ') || match('\t'))
      ++_ptr;
  }

  inline void advance()
  {
    skipSpaces();

    if (_eof)
      return;

    // advance one symbol
    ++_ptr;

    if (*_ptr == '\n') {
      ++_line;
      _col = 0;
    } else
      ++_col;

    _eof = _ptr == _end;
  }

  inline void advance(size_t n)
  {
    _ptr = (_ptr + n < _end) ? _ptr + n : _end;
    _eof = _ptr == _end;
  }

  inline bool consume(char expected)
  {
    if (*_ptr == expected) {
      advance();
      return true;
    }

    return false;
  }

  inline bool match(char expected) { return *_ptr == expected; }

  inline void lexerInsertKeyword(const Token& tok)
  {
    _keywords.insert({ tok.str, tok.type });
    _keywordsLengths.insert(strlen(tok.str));
  }

  inline bool isDigit(char c) { return c >= '0' && c <= '9'; }

  inline bool isLetter(char c)
  {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
  }

  inline const char* ptr() { return _ptr; }

  void recover();
  void lexerComment();
  const char* lexerNumber();
  const char* lexerString(char quote);
  bool lexerKeyword(TokenType& type);
  const char* lexerName();
  void error(LexerError code);
};