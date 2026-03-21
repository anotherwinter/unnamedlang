#pragma once
#include <cstdint>
#include <type_traits>

enum class Modifier : uint16_t
{
  None = 0,
  Hidden = 1 << 0,
  Static = 1 << 1,
  Const = 1 << 2,
};

template<typename E>
constexpr auto
to_underlying(E e) noexcept
{
  return static_cast<std::underlying_type_t<E>>(e);
}