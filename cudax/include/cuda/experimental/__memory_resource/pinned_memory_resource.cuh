//===----------------------------------------------------------------------===//
//
// Part of CUDA Experimental in CUDA C++ Core Libraries,
// under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// SPDX-FileCopyrightText: Copyright (c) 2024 NVIDIA CORPORATION & AFFILIATES.
//
//===----------------------------------------------------------------------===//

#ifndef _CUDA__MEMORY_RESOURCE_CUDA_PINNED_MEMORY_RESOURCE_H
#define _CUDA__MEMORY_RESOURCE_CUDA_PINNED_MEMORY_RESOURCE_H

#include <cuda/std/detail/__config>

#if defined(_CCCL_IMPLICIT_SYSTEM_HEADER_GCC)
#  pragma GCC system_header
#elif defined(_CCCL_IMPLICIT_SYSTEM_HEADER_CLANG)
#  pragma clang system_header
#elif defined(_CCCL_IMPLICIT_SYSTEM_HEADER_MSVC)
#  pragma system_header
#endif // no system header

#if _CCCL_CUDA_COMPILER(CLANG)
#  include <cuda_runtime.h>
#  include <cuda_runtime_api.h>
#endif // _CCCL_CUDA_COMPILER(CLANG)

#include <cuda/__memory_resource/properties.h>
#include <cuda/std/__concepts/concept_macros.h>
#include <cuda/std/__cuda/api_wrapper.h>
#include <cuda/std/detail/libcxx/include/stdexcept>

#include <cuda/experimental/__memory_resource/memory_resource_base.cuh>
#include <cuda/experimental/__memory_resource/pinned_memory_pool.cuh>

// Trigger a rebuild of the file

//! @file
//! The \c pinned_memory_resource class provides a memory resource that allocates pinned memory.
namespace cuda::experimental
{

#if _CCCL_CUDACC_AT_LEAST(12, 6)

//! @rst
//! .. _cudax-memory-resource-async:
//!
//! Stream ordered memory resource
//! ------------------------------
//!
//! ``pinned_memory_resource`` uses `cudaMallocFromPoolAsync / cudaFreeAsync
//! <https://docs.nvidia.com/cuda/cuda-runtime-api/group__CUDART__MEMORY__POOLS.html>`__ for allocation/deallocation. A
//! ``pinned_memory_resource`` is a thin wrapper around a \c cudaMemPool_t.
//!
//! .. warning::
//!
//!    ``pinned_memory_resource`` does not own the pool and it is the responsibility of the user to ensure that the
//!    lifetime of the pool exceeds the lifetime of the ``pinned_memory_resource``.
//!
//! @endrst
class pinned_memory_resource : public __memory_resource_base
{
private:
  //! @brief  Returns the default ``cudaMemPool_t`` for host pinned memory.
  //! @throws cuda_error if retrieving the default ``cudaMemPool_t`` fails.
  //! @returns The default memory pool for host pinned memory.
  _CCCL_NODISCARD static ::cudaMemPool_t __get_default_sysmem_pool()
  {
    static pinned_memory_pool __default_pool{};
    return __default_pool.get();
  }

public:
  //! @brief Default constructs the pinned_memory_resource using the default \c cudaMemPool_t for host pinned memory.
  //! @throws cuda_error if retrieving the default \c cudaMemPool_t fails.
  pinned_memory_resource()
      : __memory_resource_base(__get_default_sysmem_pool())
  {}

  //! @brief  Constructs the pinned_memory_resource from a \c cudaMemPool_t.
  //! @param __pool The \c cudaMemPool_t used to allocate memory.
  explicit pinned_memory_resource(::cudaMemPool_t __pool) noexcept
      : __memory_resource_base(__pool)
  {}

  //! @brief  Constructs the pinned_memory_resource from a \c pinned_memory_pool by calling get().
  //! @param __pool The \c pinned_memory_pool used to allocate memory.
  explicit pinned_memory_resource(pinned_memory_pool& __pool) noexcept
      : __memory_resource_base(__pool.get())
  {}

#  ifndef _CCCL_DOXYGEN_INVOKED // Do not document
  //! @brief Enables the \c device_accessible property
  friend constexpr void get_property(pinned_memory_resource const&, device_accessible) noexcept {}
  //! @brief Enables the \c host_accessible property
  friend constexpr void get_property(pinned_memory_resource const&, host_accessible) noexcept {}
#  endif // _CCCL_DOXYGEN_INVOKED
};
#else // _CCCL_CUDACC_BELOW(12, 6)

//! @brief pinned_memory_resource uses `cudaMallocHost` / `cudaFreeHost` for allocation / deallocation.
class pinned_memory_resource
{
public:
  constexpr pinned_memory_resource() noexcept {}

  //! @brief Allocate host memory of size at least \p __bytes.
  //! @param __bytes The size in bytes of the allocation.
  //! @param __alignment The requested alignment of the allocation.
  //! @throw std::invalid_argument in case of invalid alignment or \c cuda::cuda_error of the returned error code.
  //! @return Pointer to the newly allocated memory
  _CCCL_NODISCARD void* allocate(const size_t __bytes,
                                 const size_t __alignment = _CUDA_VMR::default_cuda_malloc_host_alignment) const
  {
    // We need to ensure that the provided alignment matches the minimal provided alignment
    if (!__is_valid_alignment(__alignment))
    {
      _CUDA_VSTD::__throw_invalid_argument("Invalid alignment passed to pinned_memory_resource::allocate.");
    }

    void* __ptr{nullptr};
    _CCCL_TRY_CUDA_API(::cudaMallocHost, "Failed to allocate memory with cudaMallocHost.", &__ptr, __bytes);
    return __ptr;
  }

  //! @brief Allocate host memory of size at least \p __bytes.
  //! @param __bytes The size in bytes of the allocation.
  //! @param __alignment The requested alignment of the allocation.
  //! @param __stream Stream on which to perform allocation. Currently ignored
  //! @throws std::invalid_argument In case of invalid alignment.
  //! @throws cuda::cuda_error If an error code was return by the cuda api call.
  //! @returns Pointer to the newly allocated memory.
  _CCCL_NODISCARD void* allocate_async(const size_t __bytes, const size_t __alignment, const ::cuda::stream_ref __stream)
  {
    (void) __stream;
    return allocate(__bytes, __alignment);
  }

  //! @brief Allocate host memory of size at least \p __bytes.
  //! @param __bytes The size in bytes of the allocation.
  //! @param __stream Stream on which to perform allocation.
  //! @throws cuda::cuda_error If an error code was return by the cuda api call.
  //! @returns Pointer to the newly allocated memory.
  _CCCL_NODISCARD void* allocate_async(const size_t __bytes, const ::cuda::stream_ref __stream)
  {
    (void) __stream;
    return allocate(__bytes);
  }

  //! @brief Deallocate memory pointed to by \p __ptr.
  //! @param __ptr Pointer to be deallocated. Must have been allocated through a call to `allocate`.
  //! @param __bytes The number of bytes that was passed to the `allocate` call that returned \p __ptr.
  //! @param __alignment The alignment that was passed to the `allocate` call that returned \p __ptr.
  void deallocate(
    void* __ptr, const size_t, const size_t __alignment = _CUDA_VMR::default_cuda_malloc_host_alignment) const noexcept
  {
    // We need to ensure that the provided alignment matches the minimal provided alignment
    _CCCL_ASSERT(__is_valid_alignment(__alignment), "Invalid alignment passed to pinned_memory_resource::deallocate.");
    _CCCL_ASSERT_CUDA_API(::cudaFreeHost, "pinned_memory_resource::deallocate failed", __ptr);
    (void) __alignment;
  }

  //! @brief Deallocate memory pointed to by \p __ptr.
  //! @param __ptr Pointer to be deallocated. Must have been allocated through a call to `allocate_async`.
  //! @param __bytes The number of bytes that was passed to the `allocate_async` call that returned \p __ptr.
  //! @param __alignment The alignment that was passed to the `allocate_async` call that returned \p __ptr.
  //! @param __stream A stream that has a stream ordering relationship with the stream used in the
  //! <a href="https://docs.nvidia.com/cuda/cuda-runtime-api/group__CUDART__MEMORY__POOLS.html">allocate_async</a> call
  //! that returned \p __ptr.
  //! @note The pointer passed to `deallocate_async` must not be in use in a stream other than \p __stream.
  //! It is the caller's responsibility to properly synchronize all relevant streams before calling `deallocate_async`.
  void deallocate_async(void* __ptr, const size_t __bytes, const size_t __alignment, const ::cuda::stream_ref __stream)
  {
    deallocate(__ptr, __bytes);
    (void) __alignment;
    (void) __stream;
  }

  //! @brief Deallocate memory pointed to by \p __ptr.
  //! @param __ptr Pointer to be deallocated. Must have been allocated through a call to `allocate_async`.
  //! @param __bytes The number of bytes that was passed to the `allocate_async` call that returned \p __ptr.
  //! @param __stream A stream that has a stream ordering relationship with the stream used in the
  //! <a href="https://docs.nvidia.com/cuda/cuda-runtime-api/group__CUDART__MEMORY__POOLS.html">allocate_async</a> call
  //! that returned \p __ptr.
  //! @note The pointer passed to `deallocate_async` must not be in use in a stream other than \p __stream.
  //! It is the caller's responsibility to properly synchronize all relevant streams before calling `deallocate_async`.
  void deallocate_async(void* __ptr, size_t __bytes, const ::cuda::stream_ref __stream)
  {
    deallocate(__ptr, __bytes);
    (void) __stream;
  }

  //! @brief Equality comparison with another \c pinned_memory_resource.
  //! @param __other The other \c pinned_memory_resource.
  //! @return Whether both \c pinned_memory_resource were constructed with the same flags.
  _CCCL_NODISCARD constexpr bool operator==([[maybe_unused]] pinned_memory_resource const& __other) const noexcept
  {
    return true;
  }
#  if _CCCL_STD_VER <= 2017
  //! @brief Equality comparison with another \c pinned_memory_resource.
  //! @param __other The other \c pinned_memory_resource.
  //! @return Whether both \c pinned_memory_resource were constructed with different flags.
  _CCCL_NODISCARD constexpr bool operator!=([[maybe_unused]] pinned_memory_resource const& __other) const noexcept
  {
    return false;
  }
#  endif // _CCCL_STD_VER <= 2017

#  ifndef _CCCL_DOXYGEN_INVOKED // Do not document
  template <class _Resource>
  _CCCL_NODISCARD bool __equal_to(_Resource const& __rhs) const noexcept
  {
    if constexpr (has_property<_Resource, device_accessible>)
    {
      return resource_ref<device_accessible>{*const_cast<pinned_memory_resource*>(this)}
          == __cudax::__as_resource_ref<device_accessible>(const_cast<_Resource&>(__rhs));
    }
    else if constexpr (has_property<_Resource, host_accessible>)
    {
      return resource_ref<host_accessible>{*const_cast<pinned_memory_resource*>(this)}
          == __cudax::__as_resource_ref<host_accessible>(const_cast<_Resource&>(__rhs));
    }
    else
    {
      return false;
    }
  }
#    if _CCCL_STD_VER >= 2020
  //! @brief Equality comparison between a \c pinned_memory_resource and another resource
  //! @param __rhs The resource to compare to
  //! @return If the underlying types are equality comparable, returns the result of equality comparison of both
  //! resources. Otherwise, returns false.
  template <class _Resource>
    requires _CUDA_VMR::__different_resource<pinned_memory_resource, _Resource>
  _CCCL_NODISCARD bool operator==(_Resource const& __rhs) const noexcept
  {
    return this->__equal_to(__rhs);
  }
#    else // ^^^ C++20 ^^^ / vvv C++17
  template <class _Resource>
  _CCCL_NODISCARD_FRIEND auto operator==(pinned_memory_resource const& __lhs, _Resource const& __rhs) noexcept
    _CCCL_TRAILING_REQUIRES(bool)(_CUDA_VMR::__different_resource<pinned_memory_resource, _Resource>)
  {
    return __lhs.__equal_to(__rhs);
  }

  template <class _Resource>
  _CCCL_NODISCARD_FRIEND auto operator==(_Resource const& __lhs, pinned_memory_resource const& __rhs) noexcept
    _CCCL_TRAILING_REQUIRES(bool)(_CUDA_VMR::__different_resource<pinned_memory_resource, _Resource>)
  {
    return __rhs.__equal_to(__lhs);
  }

  template <class _Resource>
  _CCCL_NODISCARD_FRIEND auto operator!=(pinned_memory_resource const& __lhs, _Resource const& __rhs) noexcept
    _CCCL_TRAILING_REQUIRES(bool)(_CUDA_VMR::__different_resource<pinned_memory_resource, _Resource>)
  {
    return !__lhs.__equal_to(__rhs);
  }

  template <class _Resource>
  _CCCL_NODISCARD_FRIEND auto operator!=(_Resource const& __lhs, pinned_memory_resource const& __rhs) noexcept
    _CCCL_TRAILING_REQUIRES(bool)(_CUDA_VMR::__different_resource<pinned_memory_resource, _Resource>)
  {
    return !__rhs.__equal_to(__lhs);
  }
#    endif // _CCCL_STD_VER <= 2017

  //! @brief Enables the \c device_accessible property
  friend constexpr void get_property(pinned_memory_resource const&, device_accessible) noexcept {}
  //! @brief Enables the \c host_accessible property
  friend constexpr void get_property(pinned_memory_resource const&, host_accessible) noexcept {}
#  endif // _CCCL_DOXYGEN_INVOKED

  //! @brief Checks whether the passed in alignment is valid
  static constexpr bool __is_valid_alignment(const size_t __alignment) noexcept
  {
    return __alignment <= _CUDA_VMR::default_cuda_malloc_host_alignment
        && (_CUDA_VMR::default_cuda_malloc_host_alignment % __alignment == 0);
  }
};

#endif // _CCCL_CUDACC_AT_LEAST(12, 6)

static_assert(_CUDA_VMR::async_resource_with<pinned_memory_resource, device_accessible>, "");
static_assert(_CUDA_VMR::async_resource_with<pinned_memory_resource, host_accessible>, "");

} // namespace cuda::experimental

#endif //_CUDA__MEMORY_RESOURCE_CUDA_PINNED_MEMORY_RESOURCE_H
