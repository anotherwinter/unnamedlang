#include "repl.h"
#include "back/runtime.h"
#include "back/treewalk.h"
#include "diagnostics.h"
#include "front/ast.h"
#include "front/lexer.h"
#include "front/parser.hpp"
#include "initializer.h"
#include <stdio.h>
#include <variant>

extern ASTNode* astRoot;

int
main()
{
  Initializer init{};
  Lexer& lex = init.getLexer();
  yy::parser parser(lex);
  Runtime& runtime = init.getRuntime();
  Treewalk& treewalk = init.getTreewalk();

  // parser.set_debug_level(1);

  char line[128];
  while (true) {
    printf("UnnamedLang> ");
    if (fgets(line, sizeof(line), stdin) == nullptr) {
      if (!feof(stdin))
        printf("Input error\n");

      break;
    }

    if (line[0] == 'q' && line[1] == '\n')
      break;

    lex.load(line);
    if (parser.parse() == 0) {
      Identifier id = treewalk.eval(astRoot);
      auto obj = std::get_if<ObjectHeader>(&id.name);
      if (obj)
        runtime.printObject(*obj);
    } else {
      printf("parse fail\n");
    }
  }

  return 0;
}