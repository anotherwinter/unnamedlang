#include "../../src/ast.h"
#include "../../src/bnf.tab.h"
#include "../../src/eval_ast.h"
#include "../../src/lexer.h"

extern ASTNode* astRoot;

int main() {
    lexerInit();

    char line[128];
    while (true) {
        printf("> ");
        fgets(line, sizeof(line), stdin);
        if (line[0] == 'q' && line[1] == '\n') {
            break;
        }
        lexerSetText(line);
        if (yyparse() == 0) {
            printASTRoot(astRoot);
            freeNode(astRoot);
        } else {
            printf("parse fail\n");
        }
    }

    return 0;
}
