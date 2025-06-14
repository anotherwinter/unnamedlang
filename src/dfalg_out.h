#pragma once
#define INITIAL_STATE 0
typedef enum {
ERROR = -1,
NUMBER,
} DFALG_AcceptingType;
int transition(int state, char symbol);
DFALG_AcceptingType stateType(int state);
