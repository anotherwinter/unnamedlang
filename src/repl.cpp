#include "repl.h"
#include "back/runtime.h"
#include "back/treewalk.h"
#include "diagnostics.h"
#include "front/ast.h"
#include "front/lexer.h"
#include "front/parser.hpp"
#include "initializer.h"
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <variant>

extern ASTNode* astRoot;

int
main(int argc, char* argv[])
{
  Initializer init{};
  Lexer& lex = init.getLexer();
  ParserContext parserCtx = { {} };
  yy::parser parser(lex, parserCtx);
  Runtime& runtime = init.getRuntime();
  Treewalk& treewalk = init.getTreewalk();

  // parser.set_debug_level(1);

  // read from file if filename specified
  if (argc > 1) {
    bool printOnly = false;
    if (std::strcmp(argv[1], "-p") == 0)
      printOnly = true;

    std::ifstream file;
    if (argc > 2)
      file = std::ifstream{ argv[2] };
    else
      file = std::ifstream{ argv[1] };

    std::ostringstream oss;
    oss << file.rdbuf();

    lex.load(oss.str());
    if (parser.parse() == 0) {
      if (printOnly) {
        printASTRoot(astRoot);
        return 0;
      }

      Identifier id = treewalk.eval(astRoot);
      auto obj = std::get_if<ObjectHeader>(&id.name);
      if (obj)
        runtime.printObject(*obj);
    }
  }
  // if no file specified, then read statements, one per line
  else {

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
      }
    }
  }

  return 0;
}