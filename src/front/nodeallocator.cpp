#include "front/nodeallocator.h"

using namespace HIR;

NodeAllocator::NodeAllocator(Diagnostics& diag)
  : _diag(diag)
  , _arena({})
{
}