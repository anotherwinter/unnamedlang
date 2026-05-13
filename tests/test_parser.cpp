#include "init_tests.h"
#include <gtest/gtest.h>

extern ASTNode* astRoot;

int
main()
{
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
    if (init.getParser().parse() == 0)
      printASTRoot(astRoot);
  }

  return 0;
}
