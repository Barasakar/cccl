//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <chrono>
// class month_day_last;

// constexpr bool operator==(const month_day& x, const month_day& y) noexcept;
//   Returns: x.month() == y.month()
//
// constexpr bool operator< (const month_day& x, const month_day& y) noexcept;
//   Returns: x.month() < y.month()

#include <cuda/std/cassert>
#include <cuda/std/chrono>
#include <cuda/std/type_traits>

#include "test_comparisons.h"
#include "test_macros.h"

int main(int, char**)
{
  using month          = cuda::std::chrono::month;
  using month_day_last = cuda::std::chrono::month_day_last;

  AssertComparisonsAreNoexcept<month_day_last>();
  AssertComparisonsReturnBool<month_day_last>();

  static_assert(testComparisonsValues<month_day_last>(month{1}, month{1}), "");
  static_assert(testComparisonsValues<month_day_last>(month{1}, month{2}), "");

  //  same day, different months
  for (unsigned i = 1; i < 12; ++i)
  {
    for (unsigned j = 1; j < 12; ++j)
    {
      assert((testComparisonsValues<month_day_last>(month{i}, month{j})));
    }
  }

  return 0;
}
