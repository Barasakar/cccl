//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// bitset<N>& reset(); // constexpr since C++23

#include <cuda/std/bitset>
#include <cuda/std/cassert>
#include <cuda/std/cstddef>

#include "test_macros.h"

TEST_NV_DIAG_SUPPRESS(186)

template <cuda::std::size_t N>
__host__ __device__ constexpr void test_reset_all()
{
  cuda::std::bitset<N> v;
  v.set();
  v.reset();
  for (cuda::std::size_t i = 0; i < v.size(); ++i)
  {
    {
      assert(!v[i]);
    }
  }
}

__host__ __device__ constexpr bool test()
{
  test_reset_all<0>();
  test_reset_all<1>();
  test_reset_all<31>();
  test_reset_all<32>();
  test_reset_all<33>();
  test_reset_all<63>();
  test_reset_all<64>();
  test_reset_all<65>();
  test_reset_all<1000>();

  return true;
}

int main(int, char**)
{
  test();
  static_assert(test(), "");

  return 0;
}
