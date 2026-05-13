#include "init_tests.h"
#include <cstring>
#include <fstream>
#include <gtest/gtest.h>

extern ASTNode* astRoot;

int
main(int argc, char* argv[])
{
  bool analyze = argc > 1 && std::strcmp(argv[1], "-a") == 0;

  // read from file if filename specified
  if (argc > 2) {
    std::ifstream file;
    if (analyze)
      file = std::ifstream{ argv[2] };
    else
      file = std::ifstream{ argv[1] };

    std::ostringstream oss;
    oss << file.rdbuf();

    init.getLexer().load(oss.str());
    if (init.getParser().parse() == 0) {
      auto tree = init.getTypedAST().build(astRoot);
      if (analyze) {
        auto analyzedTree = init.getTypedAST().analyze(tree);
        init.getTypedAST().print(analyzedTree.root);
      } else
        init.getTypedAST().print(tree.root);

      auto id = init.getDiag().getLastMsgID();
      if (isValid(id)) {
        for (size_t i = 0; i < id; ++i)
          printf("%s\n", init.getDiag().getDiagStr(i).c_str());
      }
      init.getDiag().clear();
    }

    return 0;
  }

  char line[128];
  while (true) {
    printf("> ");
    if (fgets(line, sizeof(line), stdin) == nullptr) {
      if (!feof(stdin))
        printf("Input error\n");

      break;
    }

    if (line[0] == 'q' && line[1] == '\n')
      break;

    init.getLexer().load(line);
    if (init.getParser().parse() == 0) {
      auto tree = init.getTypedAST().build(astRoot);
      if (analyze) {
        auto analyzedTree = init.getTypedAST().analyze(tree);
        init.getTypedAST().print(analyzedTree.root);
      } else
        init.getTypedAST().print(tree.root);

      auto id = init.getDiag().getLastMsgID();
      if (isValid(id)) {
        for (size_t i = 0; i < id; ++i)
          printf("%s\n", init.getDiag().getDiagStr(i).c_str());
      }
      init.getDiag().clear();
    }
  }

  return 0;
}
