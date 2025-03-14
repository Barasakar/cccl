//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <chrono>
// class year;

// constexpr year& operator+=(const years& d) noexcept;
// constexpr year& operator-=(const years& d) noexcept;

#include <cuda/std/cassert>
#include <cuda/std/chrono>
#include <cuda/std/type_traits>

#include "test_macros.h"

template <typename Y, typename Ys>
__host__ __device__ constexpr bool testConstexpr()
{
  Y y1{1};
  if (static_cast<int>(y1 += Ys{1}) != 2)
  {
    return false;
  }
  if (static_cast<int>(y1 += Ys{2}) != 4)
  {
    return false;
  }
  if (static_cast<int>(y1 += Ys{8}) != 12)
  {
    return false;
  }
  if (static_cast<int>(y1 -= Ys{1}) != 11)
  {
    return false;
  }
  if (static_cast<int>(y1 -= Ys{2}) != 9)
  {
    return false;
  }
  if (static_cast<int>(y1 -= Ys{8}) != 1)
  {
    return false;
  }
  return true;
}

int main(int, char**)
{
  using year  = cuda::std::chrono::year;
  using years = cuda::std::chrono::years;

  static_assert(noexcept(cuda::std::declval<year&>() += cuda::std::declval<years>()));
  static_assert(noexcept(cuda::std::declval<year&>() -= cuda::std::declval<years>()));

  static_assert(cuda::std::is_same_v<year&, decltype(cuda::std::declval<year&>() += cuda::std::declval<years>())>);
  static_assert(cuda::std::is_same_v<year&, decltype(cuda::std::declval<year&>() -= cuda::std::declval<years>())>);

  static_assert(testConstexpr<year, years>(), "");

  for (int i = 10000; i <= 10020; ++i)
  {
    year year(i);
    assert(static_cast<int>(year += years{10}) == i + 10);
    assert(static_cast<int>(year) == i + 10);
    assert(static_cast<int>(year -= years{9}) == i + 1);
    assert(static_cast<int>(year) == i + 1);
  }

  return 0;
}
