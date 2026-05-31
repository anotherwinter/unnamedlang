#pragma once
#include "middle/tac.h"

MIR::TACValue
foldBoolBool(MIR::OpCode op,
             const MIR::TACValue* lhs,
             const MIR::TACValue* rhs);

MIR::TACValue
foldNumberNumber(MIR::OpCode op,
                 const MIR::TACValue* lhs,
                 const MIR::TACValue* rhs);