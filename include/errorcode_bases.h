#pragma once
#include <cstdint>

using ErrorCode = uint16_t;
static_assert(sizeof(ErrorCode) == 2, "DiagID must be 2 bytes");

#define STUB_ERR 0

#define LEX_ERR_BASE 100
#define PARS_ERR_BASE 200
#define SEMA_ERR_BASE 300