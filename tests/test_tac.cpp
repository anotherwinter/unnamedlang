#include "init_tests.h"
#include <cstring>
#include <fstream>
#include <gtest/gtest.h>

extern ASTNode* astRoot;

int
main(int argc, char* argv[])
{
  // read from file if filename specified
  if (argc >= 2) {
    std::ifstream file = std::ifstream{ argv[1] };

    std::ostringstream oss;
    oss << file.rdbuf();

    // init.getParser().set_debug_level(1);
    init.getLexer().load(oss.str());
    if (init.getParser().parse() == 0) {
      auto tree = init.getTypedAST().build(astRoot);
      auto analyzedTree = init.getTypedAST().analyze(tree);
      init.getTACBuilder().build(analyzedTree);
      init.getTACBuilder().print();

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
