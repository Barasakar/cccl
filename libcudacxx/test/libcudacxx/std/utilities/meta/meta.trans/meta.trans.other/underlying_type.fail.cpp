//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
, C++ 14,
  c++ 17
// type_traits

// underlying_type
// Mandates: enum must not be an incomplete enumeration type.

#include <cuda/std/climits>
#include <cuda/std/type_traits>

#include "test_macros.h"

  enum E1 {
    E1Zero,
    E1One,
    E1Two = sizeof(cuda::std::underlying_type<E1>::type)
  }; // expected-error@type_traits:* {{cannot determine underlying type of incomplete enumeration type 'E1'}}

//  None of these are incomplete.
//  Scoped enums have an underlying type of 'int' unless otherwise specified
//  Unscoped enums with a specified underlying type become complete as soon as that type is specified.
// enum E2 : char            { E2Zero, E2One, E2Two = sizeof(cuda::std::underlying_type<E2>::type) };
// enum class E3             { E3Zero, E3One, E3Two = sizeof(cuda::std::underlying_type<E3>::type) };
// enum struct E4 : unsigned { E4Zero, E4One, E4Two = sizeof(cuda::std::underlying_type<E4>::type) };
// enum struct E5            { E5Zero, E5One, E5Two = sizeof(cuda::std::underlying_type<E5>::type) };
// enum class E6 : unsigned  { E6Zero, E6One, E6Two = sizeof(cuda::std::underlying_type<E6>::type) };

// These error messages will have to change if clang ever gets fixed. But at least they're being rejected.
enum E7 : cuda::std::underlying_type_t<E7>
{
}; // expected-error {{use of undeclared identifier 'E7'}}
enum class E8 : cuda::std::underlying_type_t<E8>
{
}; // expected-error {{use of undeclared identifier 'E8'}}
enum struct E9 : cuda::std::underlying_type_t<E9>
{
}; // expected-error {{use of undeclared identifier 'E9'}}

int main(int, char**)
{
  return 0;
}
