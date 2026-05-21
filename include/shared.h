#pragma once
#include <cstdint>
#include <type_traits>

enum class Modifier : uint16_t
{
  None = 0,
  Hidden = 1 << 0,
  Static = 1 << 1,
  Const = 1 << 2,
  Local = 1 << 3,
};

template<typename E>
constexpr auto
to_underlying(E e) noexcept
{
  return static_cast<std::underlying_type_t<E>>(e);
}

template<class... Ts>
struct Overloaded : Ts... {
  using Ts::operator()...;
};

template<class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;