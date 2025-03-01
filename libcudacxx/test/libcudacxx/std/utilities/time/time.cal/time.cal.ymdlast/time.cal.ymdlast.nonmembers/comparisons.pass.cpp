//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <chrono>
// class year_month_day_last;

// constexpr bool operator==(const year_month_day_last& x, const year_month_day_last& y) noexcept;
//   Returns: x.year() == y.year() && x.month_day_last() == y.month_day_last().
//
// constexpr bool operator< (const year_month_day_last& x, const year_month_day_last& y) noexcept;
//   Returns:
//      If x.year() < y.year(), returns true.
//      Otherwise, if x.year() > y.year(), returns false.
//      Otherwise, returns x.month_day_last() < y.month_day_last()

#include <cuda/std/cassert>
#include <cuda/std/chrono>
#include <cuda/std/type_traits>

#include "test_comparisons.h"
#include "test_macros.h"

int main(int, char**)
{
  using year                = cuda::std::chrono::year;
  using month               = cuda::std::chrono::month;
  using month_day_last      = cuda::std::chrono::month_day_last;
  using year_month_day_last = cuda::std::chrono::year_month_day_last;

  AssertComparisonsAreNoexcept<year_month_day_last>();
  AssertComparisonsReturnBool<year_month_day_last>();

  constexpr month January  = cuda::std::chrono::January;
  constexpr month February = cuda::std::chrono::February;

  static_assert(testComparisons(year_month_day_last{year{1234}, month_day_last{January}},
                                year_month_day_last{year{1234}, month_day_last{January}},
                                true,
                                false),
                "");

  //  different month
  static_assert(testComparisons(year_month_day_last{year{1234}, month_day_last{January}},
                                year_month_day_last{year{1234}, month_day_last{February}},
                                false,
                                true),
                "");

  //  different year
  static_assert(testComparisons(year_month_day_last{year{1234}, month_day_last{January}},
                                year_month_day_last{year{1235}, month_day_last{January}},
                                false,
                                true),
                "");

  //  different month
  static_assert(testComparisons(year_month_day_last{year{1234}, month_day_last{January}},
                                year_month_day_last{year{1234}, month_day_last{February}},
                                false,
                                true),
                "");

  //  different year and month
  static_assert(testComparisons(year_month_day_last{year{1234}, month_day_last{February}},
                                year_month_day_last{year{1235}, month_day_last{January}},
                                false,
                                true),
                "");

  //  same year, different months
  for (unsigned i = 1; i < 12; ++i)
  {
    for (unsigned j = 1; j < 12; ++j)
    {
      assert((testComparisons(year_month_day_last{year{1234}, month_day_last{month{i}}},
                              year_month_day_last{year{1234}, month_day_last{month{j}}},
                              i == j,
                              i < j)));
    }
  }

  //  same month, different years
  for (int i = 1000; i < 20; ++i)
  {
    for (int j = 1000; j < 20; ++j)
    {
      assert((testComparisons(year_month_day_last{year{i}, month_day_last{January}},
                              year_month_day_last{year{j}, month_day_last{January}},
                              i == j,
                              i < j)));
    }
  }

  return 0;
}
