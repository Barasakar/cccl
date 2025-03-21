//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <chrono>
// class year_month_day;

// constexpr bool ok() const noexcept;
//  Returns: m_.ok() && y_.ok().

#include <cuda/std/cassert>
#include <cuda/std/chrono>
#include <cuda/std/type_traits>

#include "test_macros.h"

int main(int, char**)
{
  using year           = cuda::std::chrono::year;
  using month          = cuda::std::chrono::month;
  using day            = cuda::std::chrono::day;
  using year_month_day = cuda::std::chrono::year_month_day;

  constexpr month January = cuda::std::chrono::January;

  static_assert(noexcept(cuda::std::declval<const year_month_day>().ok()));
  static_assert(cuda::std::is_same_v<bool, decltype(cuda::std::declval<const year_month_day>().ok())>);

  static_assert(!year_month_day{year{-32768}, month{}, day{}}.ok(), ""); // All three bad

  static_assert(!year_month_day{year{-32768}, January, day{1}}.ok(), ""); // Bad year
  static_assert(!year_month_day{year{2019}, month{}, day{1}}.ok(), ""); // Bad month
  static_assert(!year_month_day{year{2019}, January, day{}}.ok(), ""); // Bad day

  static_assert(!year_month_day{year{-32768}, month{}, day{1}}.ok(), ""); // Bad year & month
  static_assert(!year_month_day{year{2019}, month{}, day{}}.ok(), ""); // Bad month & day
  static_assert(!year_month_day{year{-32768}, January, day{}}.ok(), ""); // Bad year & day

  static_assert(year_month_day{year{2019}, January, day{1}}.ok(), ""); // All OK

  //  Some months have a 31st
  static_assert(year_month_day{year{2020}, month{1}, day{31}}.ok(), "");
  static_assert(!year_month_day{year{2020}, month{2}, day{31}}.ok(), "");
  static_assert(year_month_day{year{2020}, month{3}, day{31}}.ok(), "");
  static_assert(!year_month_day{year{2020}, month{4}, day{31}}.ok(), "");
  static_assert(year_month_day{year{2020}, month{5}, day{31}}.ok(), "");
  static_assert(!year_month_day{year{2020}, month{6}, day{31}}.ok(), "");
  static_assert(year_month_day{year{2020}, month{7}, day{31}}.ok(), "");
  static_assert(year_month_day{year{2020}, month{8}, day{31}}.ok(), "");
  static_assert(!year_month_day{year{2020}, month{9}, day{31}}.ok(), "");
  static_assert(year_month_day{year{2020}, month{10}, day{31}}.ok(), "");
  static_assert(!year_month_day{year{2020}, month{11}, day{31}}.ok(), "");
  static_assert(year_month_day{year{2020}, month{12}, day{31}}.ok(), "");

  //  Everyone except FEB has a 30th
  static_assert(year_month_day{year{2020}, month{1}, day{30}}.ok(), "");
  static_assert(!year_month_day{year{2020}, month{2}, day{30}}.ok(), "");
  static_assert(year_month_day{year{2020}, month{3}, day{30}}.ok(), "");
  static_assert(year_month_day{year{2020}, month{4}, day{30}}.ok(), "");
  static_assert(year_month_day{year{2020}, month{5}, day{30}}.ok(), "");
  static_assert(year_month_day{year{2020}, month{6}, day{30}}.ok(), "");
  static_assert(year_month_day{year{2020}, month{7}, day{30}}.ok(), "");
  static_assert(year_month_day{year{2020}, month{8}, day{30}}.ok(), "");
  static_assert(year_month_day{year{2020}, month{9}, day{30}}.ok(), "");
  static_assert(year_month_day{year{2020}, month{10}, day{30}}.ok(), "");
  static_assert(year_month_day{year{2020}, month{11}, day{30}}.ok(), "");
  static_assert(year_month_day{year{2020}, month{12}, day{30}}.ok(), "");

  static_assert(!year_month_day{year{2019}, cuda::std::chrono::February, day{29}}.ok(), ""); // Not a leap year
  static_assert(year_month_day{year{2020}, cuda::std::chrono::February, day{29}}.ok(), ""); // Ok; 2020 is a leap year

  for (unsigned i = 0; i <= 50; ++i)
  {
    year_month_day ym{year{2019}, January, day{i}};
    assert(ym.ok() == day{i}.ok());
  }

  for (unsigned i = 0; i <= 50; ++i)
  {
    year_month_day ym{year{2019}, month{i}, day{12}};
    assert(ym.ok() == month{i}.ok());
  }

  const int ymax = static_cast<int>(year::max());
  for (int i = ymax - 100; i <= ymax + 100; ++i)
  {
    year_month_day ym{year{i}, January, day{12}};
    assert(ym.ok() == year{i}.ok());
  }

  return 0;
}
