//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <cuda/std/chrono>

// duration

// duration& operator*=(const rep& rhs);

#include <cuda/std/cassert>
#include <cuda/std/chrono>

#include "test_macros.h"

__host__ __device__ constexpr bool test_constexpr()
{
  cuda::std::chrono::seconds s(3);
  s *= 5;
  return s.count() == 15;
}

int main(int, char**)
{
  {
    cuda::std::chrono::nanoseconds ns(3);
    ns *= 5;
    assert(ns.count() == 15);
  }

  static_assert(test_constexpr(), "");

  return 0;
}
