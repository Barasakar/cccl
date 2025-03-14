//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <chrono>
// class month_weekday;

// constexpr month_weekday
//   operator/(const month& m, const weekday_indexed& wdi) noexcept;
// Returns: {m, wdi}.
//
// constexpr month_weekday
//   operator/(int m, const weekday_indexed& wdi) noexcept;
// Returns: month(m) / wdi.
//
// constexpr month_weekday
//   operator/(const weekday_indexed& wdi, const month& m) noexcept;
// Returns: m / wdi. constexpr month_weekday
//
// constexpr month_weekday
//   operator/(const weekday_indexed& wdi, int m) noexcept;
// Returns: month(m) / wdi.

//
// [Example:
// constexpr auto mwd = February/Tuesday[3]; // mwd is the third Tuesday of February of an as yet unspecified year
//      static_assert(mwd.month() == February);
//      static_assert(mwd.weekday_indexed() == Tuesday[3]);
// —end example]

#include <cuda/std/cassert>
#include <cuda/std/chrono>
#include <cuda/std/type_traits>

#include "test_comparisons.h"
#include "test_macros.h"

int main(int, char**)
{
  using month_weekday   = cuda::std::chrono::month_weekday;
  using month           = cuda::std::chrono::month;
  using weekday         = cuda::std::chrono::weekday;
  using weekday_indexed = cuda::std::chrono::weekday_indexed;

  constexpr weekday Tuesday = cuda::std::chrono::Tuesday;
  constexpr month February  = cuda::std::chrono::February;

  { // operator/(const month& m, const weekday_indexed& wdi) (and switched)
    static_assert(noexcept(February / Tuesday[2]));
    static_assert(cuda::std::is_same_v<month_weekday, decltype(February / Tuesday[2])>);
    static_assert(noexcept(Tuesday[2] / February));
    static_assert(cuda::std::is_same_v<month_weekday, decltype(Tuesday[2] / February)>);

    //  Run the example
    {
      constexpr month_weekday wdi = February / Tuesday[3];
      static_assert(wdi.month() == February, "");
      static_assert(wdi.weekday_indexed() == Tuesday[3], "");
    }

    for (int i = 1; i <= 12; ++i)
    {
      for (unsigned j = 0; j <= 6; ++j)
      {
        for (unsigned k = 1; k <= 5; ++k)
        {
          month m(i);
          weekday_indexed wdi = weekday{j}[k];
          month_weekday mwd1  = m / wdi;
          month_weekday mwd2  = wdi / m;
          assert(mwd1.month() == m);
          assert(mwd1.weekday_indexed() == wdi);
          assert(mwd2.month() == m);
          assert(mwd2.weekday_indexed() == wdi);
          assert(mwd1 == mwd2);
        }
      }
    }
  }

  { // operator/(int m, const weekday_indexed& wdi) (and switched)
    static_assert(noexcept(2 / Tuesday[2]));
    static_assert(cuda::std::is_same_v<month_weekday, decltype(2 / Tuesday[2])>);
    static_assert(noexcept(Tuesday[2] / 2));
    static_assert(cuda::std::is_same_v<month_weekday, decltype(Tuesday[2] / 2)>);

    //  Run the example
    {
      constexpr month_weekday wdi = 2 / Tuesday[3];
      static_assert(wdi.month() == February, "");
      static_assert(wdi.weekday_indexed() == Tuesday[3], "");
    }

    for (int i = 1; i <= 12; ++i)
    {
      for (unsigned j = 0; j <= 6; ++j)
      {
        for (unsigned k = 1; k <= 5; ++k)
        {
          weekday_indexed wdi = weekday{j}[k];
          month_weekday mwd1  = i / wdi;
          month_weekday mwd2  = wdi / i;
          assert(mwd1.month() == month(i));
          assert(mwd1.weekday_indexed() == wdi);
          assert(mwd2.month() == month(i));
          assert(mwd2.weekday_indexed() == wdi);
          assert(mwd1 == mwd2);
        }
      }
    }
  }

  return 0;
}
