// #pragma once
// #include "runtime.h"
// #include <cstdint>
// #include <string>
// #include <unordered_map>

// enum class ContextFlags : uint16_t
// {
//   None = 0,
//   // if in global context
//   Global = 1 << 0,
//   // if inside of a function
//   Function = 1 << 1,
//   // if inside of a method (method also counts as a function)
//   Method = 1 << 2,
//   // if inside of a breakable construct (switch, whl)
//   Breakable = 1 << 3,
//   // if broken inside breakable
//   Broken = 1 << 4,
//   // if has unhandled exception
//   Exception = 1 << 5,
// };

// class Context
// {
// public:
//   Context(Runtime& runtime,
//           const std::unordered_map<std::string, Identifier>& table,
//           ContextFlags flags,
//           Context* prev);
//   ~Context();
//   inline void* alloc(size_t n, size_t align) { return _bump.alloc(n, align); }
//   inline void setLastEval(Identifier obj) { _lastEval = obj; }
//   inline Identifier getLastEval() { return _lastEval; }
//   inline ContextFlags getFlags() { return _flags; }
//   inline void setFlag(ContextFlags flag)
//   {
//     _flags =
//       static_cast<ContextFlags>(to_underlying(_flags) | to_underlying(flag));
//   }
//   inline void resetFlag(ContextFlags flag)
//   {
//     _flags =
//       static_cast<ContextFlags>(to_underlying(_flags) & ~to_underlying(flag));
//   }
//   inline Context* getPrev() { return _prev; }

//   // get class info which is tied to current context
//   inline ClassInfo* getClassInfo() { return _clsInfo; }

//   // set ex handler, could be lambda or ordinary function, 0 parameters
//   // signature, return void
//   void setExHandler(ObjectHeader handler);
//   Identifier* lookup(const char* name);
//   bool define(const char* name, Identifier id);
//   void returnFromFn();

// private:
//   Runtime& _runtime;
//   // symbolic table of current context named objects
//   std::map<std::string, Identifier>* _table;
//   ContextFlags _flags;
//   Identifier _lastEval;

//   // current class instance class info
//   ClassInfo* _clsInfo;

//   Context* _prev;

//   // jmp buffer for returning from function
//   jmp_buf _env;
//   BumpAlloc _bump;
// };