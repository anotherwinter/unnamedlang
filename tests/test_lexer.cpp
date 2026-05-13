#include "init_tests.h"
#include <cstdio>
#include <gtest/gtest.h>

int
main()
{
  Lexer& lex = init.getLexer();

  char line[128];
  while (true) {
    printf("> ");
    fgets(line, sizeof(line), stdin);
    if (line[0] == 'q' && line[1] == '\n') {
      break;
    }
    lex.load(line);
    Token token;
    do {
      token = lex.lexer();
      printf(
        "token %s, type %s\n", token.str, lex.lexerTypeName(token.type));

    } while (token.type != TokenType::ENDOFTOKENS);
  }

  return 0;
}
