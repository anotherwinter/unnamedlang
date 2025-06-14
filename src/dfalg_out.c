#include "dfalg_out.h"
#define ROWS_COUNT 5
#define COLS_COUNT 11
static DFALG_AcceptingType stateTypes[] = { ERROR, NUMBER, NUMBER, ERROR, NUMBER, };
static int transitionTable[] = {
1, -1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 
1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
-1, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
4, -1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
4, -1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
};
int transition(int state, char symbol) {
if (state < 0 || state >= ROWS_COUNT)
return -1;
switch (symbol) {
case '.':
return transitionTable[state*COLS_COUNT+1];
case '0':
return transitionTable[state*COLS_COUNT+10];
case '1':
return transitionTable[state*COLS_COUNT+9];
case '2':
return transitionTable[state*COLS_COUNT+8];
case '3':
return transitionTable[state*COLS_COUNT+7];
case '4':
return transitionTable[state*COLS_COUNT+6];
case '5':
return transitionTable[state*COLS_COUNT+5];
case '6':
return transitionTable[state*COLS_COUNT+4];
case '7':
return transitionTable[state*COLS_COUNT+3];
case '8':
return transitionTable[state*COLS_COUNT+2];
case '9':
return transitionTable[state*COLS_COUNT+0];
default:
return -1;
}
}
DFALG_AcceptingType stateType(int state) {
if (state < 0 || state >= ROWS_COUNT)
 return ERROR;
return stateTypes[state];
}