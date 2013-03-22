/*
 *  Copyright 2008-2012 NVIDIA Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#pragma once

#include <thrust/detail/config.h>
#include <thrust/system/detail/sequential/tag.h>
#include <cstdlib> // for free
#include <thrust/detail/raw_pointer_cast.h>

namespace thrust
{
namespace system
{
namespace detail
{
namespace sequential
{


template<typename DerivedPolicy, typename Pointer>
inline __host__ __device__
void free(sequential::execution_policy<DerivedPolicy> &, Pointer ptr)
{
#if !defined(__CUDA_ARCH__) || (__CUDA_ARCH__ > 200)
  std::free(thrust::raw_pointer_cast(ptr));
#endif
} // end mallc()


} // end sequential
} // end detail
} // end system
} // end thrust


