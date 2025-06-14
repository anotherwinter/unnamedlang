#pragma once
#include <string>

typedef struct
{
  std::string msg;
  ulong line;
  ulong col;
} ErrorInfo;