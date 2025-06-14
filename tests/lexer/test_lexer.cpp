#include "../../src/errorinfo.h"
#include "../../src/lexer.h"
#include <cstdio>

int
main()
{
  lexerInit();

  char line[128];
  while (true) {
    printf("> ");
    fgets(line, sizeof(line), stdin);
    if (line[0] == 'q' && line[1] == '\n') {
      break;
    }
    lexerSetText(line);
    Token* token = nullptr;
    do {
      token = lexer();
      printf(
        "token %s, type %s\n", token->tok, lexerTypeName(token->type).c_str());

      std::vector<ErrorInfo> errors = lexerGetErrors();
      for (auto& e : errors) {
        printf(
          "error: %s. line: %lu, col: %lu\n", e.msg.c_str(), e.line, e.col);
      }
      lexerClearErrors();
    } while (token->type != TokenType::ENDOFTOKENS);
  }

  return 0;
}
