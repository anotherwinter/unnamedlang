#include "repl.h"
#include "./src/ast.h"
#include "./src/bnf.tab.h"
#include "./src/eval_ast.h"
#include "./src/lexer.h"
#include "./src/runtime.h"
#include <stdio.h>

extern ASTNode* astRoot;

int
main()
{
  lexerInit();
  runtimeInit();

  char line[128];
  while (true) {
    printf("> ");
    fgets(line, sizeof(line), stdin);
    if (line[0] == 'q' && line[1] == '\n') {
      break;
    }
    lexerSetText(line);
    // yydebug = 1;
    if (yyparse() == 0) {
      evalProgram(astRoot);
      printObject(contextGetLastEval());
      releaseObject(contextGetLastEval());
    } else {
      printf("parse fail\n");
    }
  }

  return 0;
}